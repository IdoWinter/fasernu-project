
#pragma once

#include "data/tracks.hpp"
#include <vector>


class TrackClusterVisualizer {
public:
    virtual ~TrackClusterVisualizer() = default;
    virtual void drawCluster(const std::vector<Track>& clusters) = 0;

    void drawClusters(const std::vector<std::vector<Track>>& clusters) {
        for (auto& cluster : clusters) {
            drawCluster(cluster);
        }
    }

    virtual void initialize() = 0;
    virtual void finalize() = 0;
};