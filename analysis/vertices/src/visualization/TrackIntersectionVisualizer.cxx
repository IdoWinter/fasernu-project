#include "visualization/TrackIntersectionVisualizer.hpp"

#include <iostream>
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TApplication.h"
#include "TPolyLine3D.h"
#include "TH3D.h"
#include "TView.h"
#include <algorithm>


TrackIntersectionVisualizer::TrackIntersectionVisualizer(const std::string &fileName)
{
    canvas = new TCanvas("track-intersection-canvas", "3D tracks", 800, 600);
}

TrackIntersectionVisualizer::~TrackIntersectionVisualizer()
{
    delete canvas;
}

void TrackIntersectionVisualizer::extendTrack(const Track& track, std::vector<Float_t>& extX, std::vector<Float_t>& extY, std::vector<Float_t>& extZ) {
    if (track.eX.size() < 2) return; // Need at least 2 points

    // Get initial point (vertex) and direction vector
    Float_t x0 = track.eX[0], y0 = track.eY[0], z0 = track.eZ[0];
    Float_t x1 = track.eX.back(), y1 = track.eY.back(), z1 = track.eZ.back();

    Float_t dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;

    // Compute track length
    Float_t trackLength = sqrt(dx*dx + dy*dy + dz*dz);

    // Choose extension based on track length
    Float_t extensionFactor = std::min(trackLength * 0.3f, 5000.0f);

    extX.push_back(x0);
    extY.push_back(y0);
    extZ.push_back(z0);

    // Extend in both directions
    for (Float_t t = -1; t <= 0; t += 0.05f) {
        extX.push_back(x0 + t * dx);
        extY.push_back(y0 + t * dy);
        extZ.push_back(z0 + t * dz);
    }
}


void TrackIntersectionVisualizer::drawCluster(const std::vector<Track> &cluster)
{

    // Find min/max for eX, eY, eZ
    Float_t xmin = FLT_MAX, xmax = -FLT_MAX;
    Float_t ymin = FLT_MAX, ymax = -FLT_MAX;
    Float_t zmin = FLT_MAX, zmax = -FLT_MAX;
    for (auto &track : cluster)
    {
        std::vector<Float_t> extX, extY, extZ;
        extendTrack(track, extX, extY, extZ);
        if (!track.eX.empty())
        {
            xmin = std::min(xmin, *std::min_element(track.eX.begin(), track.eX.end()));
            xmax = std::max(xmax, *std::max_element(track.eX.begin(), track.eX.end()));
            if (!extX.empty())
            {
                xmin = std::min(xmin, *std::min_element(extX.begin(), extX.end()));
                xmax = std::max(xmax, *std::max_element(extX.begin(), extX.end()));
            }
        }
        if (!track.eY.empty())
        {
            ymin = std::min(ymin, *std::min_element(track.eY.begin(), track.eY.end()));
            ymax = std::max(ymax, *std::max_element(track.eY.begin(), track.eY.end()));
            if (!extY.empty())
            {
                ymin = std::min(ymin, *std::min_element(extY.begin(), extY.end()));
                ymax = std::max(ymax, *std::max_element(extY.begin(), extY.end()));
            }
        }
        if (!track.eZ.empty())
        {
            zmin = std::min(zmin, *std::min_element(track.eZ.begin(), track.eZ.end()));
            zmax = std::max(zmax, *std::max_element(track.eZ.begin(), track.eZ.end()));
            if (!extZ.empty())
            {
                zmin = std::min(zmin, *std::min_element(extZ.begin(), extZ.end()));
                zmax = std::max(zmax, *std::max_element(extZ.begin(), extZ.end()));
            }
        }
    }

    xmin -= 100;
    xmax += 100;
    ymin -= 100;
    ymax += 100;
    zmin -= 100;
    zmax += 100;
    canvas->Clear();
    TView *view = TView::CreateView(1);
    view->SetRange(xmin, ymin, zmin, xmax, ymax, zmax);
    for (auto &track : cluster)
    {
        std::vector<Float_t> extX, extY, extZ;
        extendTrack(track, extX, extY, extZ);
        // Check that sizes are consistent
        size_t nPoints = track.eX.size();
        TPolyLine3D *poly = new TPolyLine3D(nPoints);
        for (size_t i = 0; i < nPoints; ++i)
        {
            poly->SetNextPoint(track.eX[i], track.eY[i], track.eZ[i]);
        }

        poly->SetLineColor(kBlue);
        poly->Draw("same");

        TPolyLine3D *polyExt = new TPolyLine3D(extX.size());
        for (size_t i = 0; i < extX.size(); ++i)
        {
            polyExt->SetNextPoint(extX[i], extY[i], extZ[i]);
        }
        polyExt->SetLineColor(kRed);
        polyExt->Draw("same");
    }
    canvas->Print("vertices_attempts.pdf");
}

void TrackIntersectionVisualizer::initialize()
{
    
    canvas->Print("vertices_attempts.pdf[");

}

void TrackIntersectionVisualizer::finalize()
{
    canvas->Print("vertices_attempts.pdf]");
}