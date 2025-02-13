#pragma once

#include "data/tracks.hpp"
#include <Acts/Vertexing/Vertex.hpp>
// #include <Acts/Bound

class VertexKalmanFilter
{
public:
    VertexKalmanFilter(std::vector<std::vector<Track>> &trackClusters);
    ~VertexKalmanFilter();

    // std::vector<Acts::Vertex> filterTracks();
    std::unique_ptr<Acts::FreeTrackParameters> convertTrackToActs(const Track &track);

private:
    std::vector<std::vector<Track>> &m_trackClusters;
};