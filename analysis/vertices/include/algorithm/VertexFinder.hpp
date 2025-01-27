#pragma once

#include "data/tracks.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <set>
#include "TH1F.h"


typedef std::pair<int, int> Cell;

struct CellHash
{
    size_t operator()(const std::pair<int, int> &cell) const
    {
        return std::hash<int>()(cell.first) ^ std::hash<int>()(cell.second);
    }
};

using Grid = std::unordered_map<Cell, std::vector<int>, CellHash>;

class VertexFinder {
public:
  VertexFinder(std::vector<Track>& tracks, Float_t maxDist);
  ~VertexFinder();

  std::vector<std::vector<std::pair<Float_t, Float_t>>> findClusters(double epsilon, int minPts);
  void fillHistogram();
  TH1F* getDensityHistogram() { return m_density; }
private:
    std::vector<int> regionQuery(int pointIndex, const Grid& grid, 
                            double epsilonSq, double cellSize);

    Grid createGrid(double cellSize);

    TH1F* m_density;

    // Use SOA for better cache locality
    struct Points {
        std::vector<Float_t> x;
        std::vector<Float_t> y;
        std::vector<int> clusterID; // -1 for noise, 0 for unclassified
        std::vector<bool> visited;
    } points;
};