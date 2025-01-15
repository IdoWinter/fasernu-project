#include <iostream>
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "simulation/VertexSimulator.hpp"
#include "algorithm/VertexFinder.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];

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

    return 0;
}

void parseDataFromRootFile(const std::string& filename)
{
    TFile *file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie())
    {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }

    // load the data from the file
    // in tracks.sf.{eX, eY, eZ, eTX, eTY}
    TTreeReader tracks("tracks", file);
    // TTreeReaderValue<Int_t> sf_(tracks, "sf");

    TTreeReaderArray<Float_t> eX(tracks, "sf.eX");
    TTreeReaderArray<Float_t> eY(tracks, "sf.eY");
 
    // std::cout << typeid(K::FT).name() << std::endl;
    // std::vector<Point> points;
    // Long64_t nEntries = tracks.GetEntries();
    // Long64_t iterations = 0;
    // while (tracks.Next())
    // {
    //     iterations++;
    //     if (eX.GetSize() < 1 || eY.GetSize() < 1)
    //     {
    //         std::cout << "Skipping entry " << tracks.GetCurrentEntry() << std::endl;
    //         continue;
    //     }
    //     Float_t sf_eX = eX[0];     
    //     Float_t sf_eY = eY[0];
    //     if (!std::isfinite(sf_eX) || !std::isfinite(sf_eY))
    //     {
    //         std::cerr << "Invalid data at entry " << tracks.GetCurrentEntry() << std::endl;
    //         continue;
    //     }
    //     points.emplace_back(static_cast<float>(sf_eX), static_cast<float>(sf_eY));
    // }
}
