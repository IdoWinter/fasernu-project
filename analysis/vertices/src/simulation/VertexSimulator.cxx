#include "simulation/VertexSimulator.hpp"
#include "TRandom.h"

VertexSimulator::VertexSimulator(Float_t densityOfTracks, Int_t nMaxVertices, Int_t nMaxTracksPerVertex)
{
    // number of tracks per cm^2
    this->m_densityOfTracks = densityOfTracks;
    this->m_nMaxVertices = nMaxVertices;
    this->m_nMaxTracksPerVertex = nMaxTracksPerVertex;
}


#define SIZE_OF_FILM 3.5

SimulationData VertexSimulator::simulate()
{
    // Generate the number of vertices Poisson-distributed (min 1)
    TRandom random = TRandom();
    // seed with the current time
    random.SetSeed();
    int nVertices = random.Poisson(m_nMaxVertices);
    Double_t sigmaAngle = 0.075; // milliradians

    SimulationData data;
    for (int i = 0; i < nVertices; i++)
    {
        // Uniformly generate the position of the vertex.
        Double_t xVertex = i/10;
        Double_t yVertex = i/10;
        // the films are 1mm apart.
        Double_t zVertex = random.Uniform(-0.1, 0);

        // Generate the number of tracks Poisson-distributed (min 1)
        Int_t nTracks = random.Poisson(m_nMaxTracksPerVertex);

        std::vector<Track> tracks;
        tracks.reserve(nTracks);
        for (int j = 0; j < nTracks; j++)
        {
            // Generate the angles of the track. They are taken such that tan(theta)<1.5
            Double_t theta_x = random.Gaus(0, sigmaAngle);
            Double_t theta_y = random.Gaus(0, sigmaAngle);

            // track origin
            Double_t x0 =  xVertex - zVertex * tan(theta_x);
            Double_t y0 =  yVertex - zVertex * tan(theta_y);
            Track track;
            track.eX.push_back(x0);
            track.eY.push_back(y0);
            track.eZ.push_back(0);
            tracks.push_back(track);
        }

        data.vertices.push_back(SimulationData::Vertex{
            .x = static_cast<Float_t>(xVertex),
            .y = static_cast<Float_t>(yVertex),
            .z = static_cast<Float_t>(zVertex),
            .tracks = tracks
        });
    }

    auto mNumOfUnrelatedTracks = random.Poisson(m_densityOfTracks * (SIZE_OF_FILM * SIZE_OF_FILM));
    for (int i = 0; i < mNumOfUnrelatedTracks; i++)
    {
        Track track;
        track.eX.push_back(random.Uniform(0, 25));
        track.eY.push_back(random.Uniform(0, 25));
        track.eZ.push_back(0);
        data.unrelatedTracks.push_back(track);
    }

    return data;
}