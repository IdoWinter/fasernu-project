#include "data/tracks.hpp"

struct SimulationData {
    struct Vertex {
        Float_t x;
        Float_t y;
        Float_t z;
        std::vector<Track> tracks;
    };

    std::vector<Vertex> vertices;
    std::vector<Track> unrelatedTracks;

    std::vector<Track> toTracksData() {
        auto tracks = std::vector<Track>();
        tracks.reserve(vertices.size() * 10 + unrelatedTracks.size());
        tracks.insert(tracks.begin(), unrelatedTracks.begin(), unrelatedTracks.end());
        for (auto &vertex : vertices) {
            tracks.insert(tracks.end(), vertex.tracks.begin(), vertex.tracks.end());
        }
        return tracks;
    }
};

class VertexSimulator
{
public:
    VertexSimulator(Float_t densityOfTracks, Int_t nMaxVertices, Int_t nMaxTracksPerVertex);
    SimulationData simulate();

private:
    Float_t m_densityOfTracks;
    Int_t m_nMaxVertices;
    Int_t m_nMaxTracksPerVertex;
};