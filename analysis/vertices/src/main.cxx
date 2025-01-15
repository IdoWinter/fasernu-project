#include <iostream>
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "simulation/VertexSimulator.hpp"
#include "algorithm/VertexFinder.hpp"
#include <algorithm>


void runSimulation() {
    VertexSimulator simulator(Float_t(5e3), Int_t(10), Int_t(20));
    SimulationData simulationData = simulator.simulate();
    std::cout << "Generated " << simulationData.vertices.size() << " vertices" << std::endl;
    std::cout << "Generated " << simulationData.unrelatedTracks.size() << " unrelated tracks" << std::endl;

    auto tracks = simulationData.toTracksData();
    std::cout << "Generated " << tracks.size() << " tracks" << std::endl;

    // print the locations of the vertices
    for (auto &vertex : simulationData.vertices)
    {
        std::cout << "Vertex at (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }


    VertexFinder finder(tracks);
    auto vertices = finder.findVertices();
    std::cout << "Found " << vertices.size() << " vertices" << std::endl;
    
    for (auto &vertex : vertices)
    {
        std::cout << "Vertex at (" << vertex.first << ", " << vertex.second << ")" << std::endl;
    }
}

std::unordered_map<Float_t, std::vector<Track>> parseDataFromRootFile(const std::string& filename)
{
    TFile *file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie())
    {
        std::cerr << "Failed to open file " << filename << std::endl;
        return std::unordered_map<Float_t, std::vector<Track>>();
    }

    // load the data from the file
    // in tracks.sf.{eX, eY, eZ, eTX, eTY}
    TTreeReader tracks("tracks", file);
    // TTreeReaderValue<Int_t> sf_(tracks, "sf");

    TTreeReaderArray<Float_t> eX(tracks, "sf.eX");
    TTreeReaderArray<Float_t> eY(tracks, "sf.eY");
    TTreeReaderArray<Float_t> eZ(tracks, "sf.eZ");
 
    Long64_t nEntries = tracks.GetEntries();
    Long64_t iterations = 0;
    std::unordered_map<Float_t, std::vector<Track>> filmToOriginatingTracks;
    while (tracks.Next())
    {
        iterations++;
        int minZIndex = 0;
        Float_t minZ = eZ[0];
        for (int i = 1; i < eZ.GetSize(); i++)
        {
            if (eZ[i] < minZ)
            {
                minZ = eZ[i];
                minZIndex = i;
            }
        }
        if (eX.GetSize() < minZIndex || eY.GetSize() < minZIndex)
        {
            std::cerr << "Invalid data at entry " << tracks.GetCurrentEntry() << std::endl;
            continue;
        }
        Float_t sf_eX = eX[minZIndex];
        Float_t sf_eY = eY[minZIndex];
        Float_t sf_eZ = eZ[minZIndex];
        if (!std::isfinite(sf_eX) || !std::isfinite(sf_eY) || !std::isfinite(sf_eZ))
        {
            std::cerr << "Invalid data at entry " << tracks.GetCurrentEntry() << std::endl;
            continue;
        }
        if (filmToOriginatingTracks.find(sf_eZ) == filmToOriginatingTracks.end())
        {
            filmToOriginatingTracks[sf_eZ] = std::vector<Track>();
        }
        Track track;
        track.eX.push_back(sf_eX);
        track.eY.push_back(sf_eY);
        track.eZ.push_back(sf_eZ);
        filmToOriginatingTracks[sf_eZ].push_back(track);
    }
    std::cout << "There are " << filmToOriginatingTracks.size() << " unique Z values" << std::endl;
    return filmToOriginatingTracks;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];

    auto zValueToTracks = parseDataFromRootFile(filename);
    for (auto &pair : zValueToTracks)
    {
        std::cout << "Processing film at z=" << pair.first << std::endl;
        VertexFinder finder(pair.second);
        auto vertices = finder.findVertices();
        std::cout << "Found " << vertices.size() << " vertices" << std::endl;
        for (auto &vertex : vertices)
        {
            // std::cout << "Vertex at (" << vertex.first << ", " << vertex.second << ")" << std::endl;
        }
    }



    return 0;
}
