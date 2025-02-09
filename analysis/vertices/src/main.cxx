#include <iostream>
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "simulation/VertexSimulator.hpp"
#include "algorithm/VertexFinder.hpp"
#include "visualization/TrackIntersectionVisualizer.hpp"
#include "visualization/TrackLengthVisualizer.hpp"
#include "TPolyLine3D.h"
#include "TH3D.h"
#include "TView.h"
#include <algorithm>
#include <Acts/Vertexing/IterativeVertexFinder.hpp>

void runSimulation()
{
    int maxTracksPerVertex = 20;
    double densityOfTracks = 5e3;
    VertexSimulator simulator(Float_t(densityOfTracks), Int_t(10), Int_t(maxTracksPerVertex));
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

    VertexFinder finder(tracks, 50);
    auto vertices = finder.findClusters(sqrt(1.0 / densityOfTracks / 6), maxTracksPerVertex / 2);
    std::cout << "Found " << vertices.size() << " vertices" << std::endl;
}

std::map<Float_t, std::vector<Track>> parseDataFromRootFile(const std::string &filename)
{
    TFile *file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie())
    {
        std::cerr << "Failed to open file " << filename << std::endl;
        return std::map<Float_t, std::vector<Track>>();
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
    std::map<Float_t, std::vector<Track>> filmToOriginatingTracks;
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
        track.eX.reserve(eX.GetSize());
        track.eY.reserve(eY.GetSize());
        track.eZ.reserve(eZ.GetSize());
        track.eX.push_back(eX[minZIndex]);
        track.eY.push_back(eY[minZIndex]);
        track.eZ.push_back(eZ[minZIndex]);
        for (int i = 0; i < eX.GetSize(); i++)
        {
            if (i == minZIndex)
            {
                continue;
            }
            track.eX.push_back(eX[i]);
            track.eY.push_back(eY[i]);
            track.eZ.push_back(eZ[i]);
        }
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
    Float_t maxDist = 200;

    auto histogram = new TH1F("hDensity", "Minimum distances between points", 500, 0, maxDist);
    auto numOfVertices = new TH1F("numOfVertices", "Number of vertices", 20, 0, 20);
    auto numPointsPerVertex = new TH1I("numPointsPerVertex", "Number of points per vertex", 50, 0, 50);
    
    std::vector<std::unique_ptr<TrackClusterVisualizer>> visualizers;
    visualizers.push_back(std::make_unique<TrackIntersectionVisualizer>("track-intersection-canvas"));
    // visualizers.push_back(std::make_unique<TrackLengthVisualizer>("track-length-canvas"));
    
    std::vector<std::vector<Track>> allClusters;
    // iterate over the z values, skipping the first 3
    for (auto it = std::next(zValueToTracks.begin(), 3); it != zValueToTracks.end(); ++it)
    {
        auto &pair = *it;
        std::cout << "Processing film at z=" << pair.first << " which has " << pair.second.size() << " tracks beginning on it" << std::endl;
        VertexFinder finder(pair.second, maxDist);
        auto clusters = finder.findClusters(20, 10);
        for (auto &cluster : clusters)
        {
            bool longTrack = false;
            for (auto &track : cluster)
            {
                auto dx = track.eX.back() - track.eX.front();
                auto dy = track.eY.back() - track.eY.front();
                auto dz = track.eZ.back() - track.eZ.front();
                auto length = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (length > 20000)
                {
                    longTrack = true;
                    break;
                }
            }
            if (longTrack)
            {
                allClusters.push_back(cluster);
            }
        }
    }

    for (auto &visualizer : visualizers)
    {
        visualizer->initialize();
        visualizer->drawClusters(allClusters);
        visualizer->finalize();
    }


    return 0;
}
