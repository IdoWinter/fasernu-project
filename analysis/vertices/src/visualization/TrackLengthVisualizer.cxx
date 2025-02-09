#include "visualization/TrackLengthVisualizer.hpp"
#include <cmath>

TrackLengthVisualizer::TrackLengthVisualizer(const char* canvasName) 
    : m_canvasName(canvasName), m_canvas(nullptr), m_histogram(nullptr) {}

void TrackLengthVisualizer::initialize() {
    m_canvas = new TCanvas(m_canvasName, "Track Lengths", 800, 600);
    m_histogram = new TH1F("hTrackLengths", "Track Lengths;Length [mm];Entries", 100, 0, 100000);
}

void TrackLengthVisualizer::drawCluster(const std::vector<Track>& cluster) {
    for (const auto& track : cluster) {
        double dx = track.eX.back() - track.eX.front();
        double dy = track.eY.back() - track.eY.front();
        double dz = track.eZ.back() - track.eZ.front();
        double length = std::sqrt(dx*dx + dy*dy + dz*dz);
        m_histogram->Fill(length);
    }
}

void TrackLengthVisualizer::finalize() {
    m_canvas->cd();
    m_histogram->Draw();
    m_canvas->Update();
    m_canvas->SaveAs("track-lengths.pdf");
}