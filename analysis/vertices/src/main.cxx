#include <iostream>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/draw_triangulation_2.h>
#include "simulation/VertexSimulator.hpp"

typedef CGAL::Simple_cartesian<float> K;
typedef K::Point_2 Point;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];

    VertexSimulator simulator(Float_t(1e6), Int_t(10), Int_t(50));
    SimulationData simulationData = simulator.simulate();
    std::cout << "Generated " << simulationData.vertices.size() << " vertices" << std::endl;
    std::cout << "Generated " << simulationData.unrelatedTracks.size() << " unrelated tracks" << std::endl;

    auto tracks = simulationData.toTracksData();
    std::cout << "Generated " << tracks.size() << " tracks" << std::endl;

    Delaunay dt;
    for (auto &track : tracks)
    {
        dt.insert(Point(track.eX[0], track.eY[0]));   
    }

    // print the locations of the vertices
    for (auto &vertex : simulationData.vertices)
    {
        std::cout << "Vertex at (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }

    // find the smallest edge in the triangulation

    auto minEdge = dt.finite_edges_begin();
    for (auto eit = dt.finite_edges_begin(); eit != dt.finite_edges_end(); eit++)
    {
        if (dt.segment(eit).squared_length() < dt.segment(minEdge).squared_length())
        {
            minEdge = eit;
        }
    }

    std::cout << "Smallest edge: " << dt.segment(minEdge).squared_length() << " at points " << dt.segment(minEdge).source() << " and " << dt.segment(minEdge).target() << std::endl;

    // connect the two points of the smallest edge and delete the previous edge
    auto v1 = dt.segment(minEdge).source();
    auto v2 = dt.segment(minEdge).target();
    auto newVertex = Point((v1.x() + v2.x()) / 2, (v1.y() + v2.y()) / 2);
    dt.remove(dt.insert(v1));
    dt.remove(dt.insert(v2));
    dt.insert(newVertex);
    


    

    // CGAL::draw(dt);
    

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
 
    std::cout << typeid(K::FT).name() << std::endl;
    std::vector<Point> points;
    Long64_t nEntries = tracks.GetEntries();
    Long64_t iterations = 0;
    while (tracks.Next())
    {
        iterations++;
        if (eX.GetSize() < 1 || eY.GetSize() < 1)
        {
            std::cout << "Skipping entry " << tracks.GetCurrentEntry() << std::endl;
            continue;
        }
        Float_t sf_eX = eX[0];     
        Float_t sf_eY = eY[0];
        if (!std::isfinite(sf_eX) || !std::isfinite(sf_eY))
        {
            std::cerr << "Invalid data at entry " << tracks.GetCurrentEntry() << std::endl;
            continue;
        }
        points.emplace_back(static_cast<float>(sf_eX), static_cast<float>(sf_eY));
    }
}
