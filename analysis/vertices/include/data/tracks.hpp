#pragma once

#include "TFile.h"
#include "TMatrixD.h"

struct Track {
    std::vector<Float_t> eX;
    std::vector<Float_t> eY;
    std::vector<Float_t> eZ;
    std::vector<Float_t> eTX;
    std::vector<Float_t> eTY;
    std::vector<Float_t> eP;
    TMatrixD covariance;
};
