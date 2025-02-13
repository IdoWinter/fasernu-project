#include "algorithm/VertexKalmanFilter.hpp"

#include <Acts/Definitions/Algebra.hpp>
#include <Acts/Definitions/TrackParametrization.hpp>

std::unique_ptr<Acts::FreeTrackParameters> VertexKalmanFilter::convertTrackToActs(const Track &track)
{
    Float_t eX = track.eX[0];
    Float_t eY = track.eY[0];
    Float_t eZ = track.eZ[0];
    Float_t eP = track.eP[0];


    // Build the measured position vector.
    Acts::Vector3 position(eX, eY, eZ);

    // Determine the momentum direction from slopes.
    // Here we use the direction (eTX, eTY, 1) and normalize it.
    Acts::Vector3 direction(track.eTX[0], track.eTY[0], 1.0);
    direction.normalize();
    
    // Full momentum vector
    Acts::Vector3 momentum = direction * track.eP[0];

    // Compute the ACTS parameters.
    const double phi   = std::atan2(momentum.y(), momentum.x());
    const double d0    = -(eX * std::sin(phi) - eY * std::cos(phi));
    const double z0    = eZ;
    const double theta = std::acos(momentum.z() / eP);
    const double qOverP= 1 / eP;


    Acts::FreeSymMatrix actsCov;
    if (track.covariance.GetNcols() == 5){
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                actsCov(i, j) = (track.covariance)(i, j);
    } else {
        actsCov.setZero();
    }

    Acts::FreeVector parameters;
    parameters[Acts::FreeIndices::eFreePos0] = position.x();
    parameters[Acts::FreeIndices::eFreePos1] = position.y();
    parameters[Acts::FreeIndices::eFreePos2] = position.z();
    parameters[Acts::FreeIndices::eFreeTime] = 0.0;
    parameters[Acts::FreeIndices::eFreeDir0] = direction.x();
    parameters[Acts::FreeIndices::eFreeDir1] = direction.y();
    parameters[Acts::FreeIndices::eFreeDir2] = direction.z();
    parameters[Acts::FreeIndices::eFreeQOverP] = qOverP;

    return std::make_unique<Acts::FreeTrackParameters>(parameters, actsCov);
}