#pragma once

#include "data/tracks.hpp"
#include "visualization/TrackClusterVisualizer.hpp"
#include "TCanvas.h"

class TrackIntersectionVisualizer : public TrackClusterVisualizer {
public:
    TrackIntersectionVisualizer(const std::string& fileName);
    ~TrackIntersectionVisualizer();
    void drawCluster(const std::vector<Track>& clusters) override;
    void initialize() override;
    void finalize() override;
private:
    TCanvas* canvas;
    void extendTrack(const Track& track, std::vector<Float_t>& extX, std::vector<Float_t>& extY, std::vector<Float_t>& extZ);
};