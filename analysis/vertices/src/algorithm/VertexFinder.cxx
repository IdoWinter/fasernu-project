#include "algorithm/VertexFinder.hpp"

#include <queue>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Polygon_2.h>


#define NOISE -1
#define UNCLASSIFIED 0

VertexFinder::VertexFinder(std::vector<Track> &tracks, Float_t maxDist)
{
    for (auto &track : tracks)
    {
        points.x.push_back(track.eX[0]);
        points.y.push_back(track.eY[0]);
        points.clusterID.push_back(UNCLASSIFIED);
        points.visited.push_back(false);
    }
    // create a name for the histogram using the number of tracks
    m_density = new TH1F(("hDensity" + std::to_string(tracks.size())).c_str(), "Local Density Distribution", 500, 0, maxDist);
}

VertexFinder::~VertexFinder()
{}

std::vector<std::vector<std::pair<Float_t, Float_t>>> VertexFinder::findClusters(double epsilon, int minPts)
{
    const double epsilonSq = epsilon * epsilon;
    const double cellSize = epsilon / std::sqrt(2);
    auto grid = createGrid(cellSize);

    int clusterID = 0;
    for (int i = 0; i < points.x.size(); ++i) {
        if (points.clusterID[i] != UNCLASSIFIED) {
            continue;
        }
        std::vector<int> neighbors = regionQuery(i, grid, epsilonSq, cellSize);
        if (neighbors.size() < minPts) {
            points.clusterID[i] = NOISE;
            continue;
        } 
        clusterID++;
        points.clusterID[i] = clusterID;
        std::queue<int> expandQueue;
        for (int j : neighbors) {
            if (j != i) {
                expandQueue.push(j);
            }
        }
        while (!expandQueue.empty()) {
            int currentIdx = expandQueue.front();
            expandQueue.pop();
            // std::cout << "Current expand queue size: " << expandQueue.size() << std::endl;
            if (points.clusterID[currentIdx] == NOISE) {
                points.clusterID[currentIdx] = clusterID;
            }
            if (points.clusterID[currentIdx] != UNCLASSIFIED) {
                continue;
            }
            points.clusterID[currentIdx] = clusterID;
            std::vector<int> newNeighbors = regionQuery(currentIdx, grid, epsilonSq, cellSize);
            if (newNeighbors.size() >= minPts) {
                for (int j : newNeighbors) {
                    expandQueue.push(j);
                }
            }
        }
    }

    std::map<int, std::vector<std::pair<Float_t, Float_t>>> clusterMap;
    for (int i = 0; i < points.x.size(); ++i) {
        if (points.clusterID[i] > 0) {
            clusterMap[points.clusterID[i]].push_back(std::make_pair(points.x[i], points.y[i]));
        }
    }

    std::vector<std::vector<std::pair<Float_t, Float_t>>> clusters;
    for (auto &cluster : clusterMap) {
        clusters.push_back(cluster.second);
    }

    return clusters;
}

std::vector<int> VertexFinder::regionQuery(int pointIndex, const Grid& grid, 
                            double epsilonSq, double cellSize) {
    std::vector<int> neighbors;
    const double x = points.x[pointIndex];
    const double y = points.y[pointIndex];
    const int cell_x = static_cast<int>(x / cellSize);
    const int cell_y = static_cast<int>(y / cellSize);

    // Check adjacent 3x3 cells
    for(int dx = -1; dx <= 1; ++dx) {
        for(int dy = -1; dy <= 1; ++dy) {
            const Cell current_cell(cell_x + dx, cell_y + dy);
            const auto it = grid.find(current_cell);
            if(it != grid.end()) {
                for(int j : it->second) {
                    const double dx_p = x - points.x[j];
                    const double dy_p = y - points.y[j];
                    if(dx_p*dx_p + dy_p*dy_p <= epsilonSq) {
                        neighbors.push_back(j);
                    }
                }
            }
        }
    }
    return neighbors;
}


Grid VertexFinder::createGrid(double cellSize) {
    Grid grid;
    // Assign points to grid cells
    for (int i = 0; i < points.x.size(); ++i)
    {
        int x = static_cast<int>(points.x[i] / cellSize);
        int y = static_cast<int>(points.y[i] / cellSize);
        grid[Cell(x, y)].push_back(i);
    }
    std::cout << "Grid size: " << grid.size() << std::endl;
    return grid;

}
void VertexFinder::fillHistogram()
{
    const double epsilon = 1000;
    const double epsilonSq = epsilon * epsilon;
    const double cellSize = epsilon / std::sqrt(2);
    auto grid = createGrid(cellSize);
    for (int i = 0; i < points.x.size(); ++i)
    {
        auto neighbors = regionQuery(i, grid, epsilonSq, cellSize);
        double min_dist_sq = std::numeric_limits<double>::max();
        for (auto j: neighbors)
        {
            if (i == j)
                continue;
            double dist_x = points.x[i] - points.x[j];
            double dist_y = points.y[i] - points.y[j];
            double dist_sq = dist_x * dist_x + dist_y * dist_y;
            if (dist_sq < min_dist_sq)
            {
                min_dist_sq = dist_sq;
            }
        }

        m_density->Fill(min_dist_sq);
    }
}
