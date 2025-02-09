#pragma once
#include "visualization/TrackClusterVisualizer.hpp"
#include "TH1F.h"
#include "TCanvas.h"

class TrackLengthVisualizer : public TrackClusterVisualizer {
public:
    explicit TrackLengthVisualizer(const char* canvasName);
    void drawCluster(const std::vector<Track>& cluster) override;
    void initialize() override;
    void finalize() override;

private:
    TCanvas* m_canvas;
    TH1F* m_histogram;
    const char* m_canvasName;
};