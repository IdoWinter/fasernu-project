#pragma once

#include "data/tracks.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include "TTree.h"
#include <set>


typedef CGAL::Simple_cartesian<float> K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Vertex_handle Vertex_handle;


class VertexFinder {
public:
  VertexFinder(std::vector<Track>& tracks);
  ~VertexFinder();

  std::vector<std::pair<Float_t, Float_t>> findVertices();
private:
    std::unordered_set<Vertex_handle> findNeighbours(Vertex_handle v, unsigned int depth=2);
    double calculateDensity(std::unordered_set<Vertex_handle>& vertices);

    Delaunay dt;
};