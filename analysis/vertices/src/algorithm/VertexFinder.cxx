#include "algorithm/VertexFinder.hpp"

#include <queue>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Polygon_2.h>

typedef K::Point_2 Point;
typedef Delaunay::Edge Edge;
typedef Delaunay::Face_handle Face_handle;
typedef CGAL::Polygon_2<K> Polygon;

double calculateFaceArea(Face_handle &face)
{
    auto v1 = face->vertex(0)->point();
    auto v2 = face->vertex(1)->point();
    auto v3 = face->vertex(2)->point();

    // Using the formula for the area of a triangle given its vertices
    return std::abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2);
}

VertexFinder::VertexFinder(std::vector<Track> &tracks) : dt()
{
    for (auto &track : tracks)
    {
        dt.insert(Point(track.eX[0], track.eY[0]));
    }
}

VertexFinder::~VertexFinder()
{
}

std::vector<std::pair<Float_t, Float_t>> VertexFinder::findVertices()
{
    auto vertices = std::vector<std::pair<Float_t, Float_t>>();
    auto facesWithAreas = std::vector<std::pair<Face_handle, double>>();
    double totalArea = 0;
    for (auto edge = dt.finite_edges_begin(); edge != dt.finite_edges_end(); edge++)
    {
        auto face = edge->first;
        if (!dt.is_infinite(face))
        {
            auto area = calculateFaceArea(face);
            totalArea += area;
            facesWithAreas.push_back(std::make_pair(face, area));
        }
    }
    std::sort(facesWithAreas.begin(), facesWithAreas.end(), [](const std::pair<Face_handle, double> &a, const std::pair<Face_handle, double> &b)
              { return a.second < b.second; });

    double averageDensity = dt.number_of_vertices() / totalArea;

    std::unordered_set<Vertex_handle> visited;
    for (auto &faceWithArea: facesWithAreas)
    {
        auto face = faceWithArea.first;
        auto area = faceWithArea.second;
        std::unordered_set<Vertex_handle> faceNeighbours;
        double x_value = 0;
        double y_value = 0;
        int weight = 0;
        bool skip = false;
        for (int i = 0; i < 3; ++i)
        {
            auto point = face->vertex(i);
            if (visited.find(point) != visited.end())
            {
                skip = true;
                break;
            }
            visited.insert(point);
            auto pointNeighbours = findNeighbours(point, 1);
            for (auto neighbour : pointNeighbours)
            {
                faceNeighbours.insert(neighbour);
            }
        }
        if (skip)
        {
            continue;
        }
        auto density = calculateDensity(faceNeighbours);
        // The density needs to be very high for this to be considered a vertex
        if (density < 50 * averageDensity)
        {
            // this is an area that we rule out as a vertex
            // we visited all the points in this area
            for (auto neighbour : faceNeighbours)
            {
                visited.insert(neighbour);
            }
            continue;
        }
        for (auto neighbour : faceNeighbours)
        {
            visited.insert(neighbour);
            x_value = (x_value * weight + neighbour->point().x()) / (weight + 1);
            y_value = (y_value * weight + neighbour->point().y()) / (weight + 1);
            weight++;
        }
        if (weight != 0)
        {
            // std::cout << "Density: " << density << std::endl;
            vertices.push_back(std::make_pair(x_value, y_value));
        }
    }

    std::cout << "Average density: " << averageDensity << std::endl;

    return vertices;
}

std::unordered_set<Vertex_handle> VertexFinder::findNeighbours(Vertex_handle v, unsigned int depth)
{
    if (depth == 0)
    {
        return {};
    }

    std::queue<std::pair<Vertex_handle, int>> to_visit; // Queue of (vertex, current depth)
    std::unordered_set<Vertex_handle> visited;
    std::unordered_set<Vertex_handle> result;

    // BFS to find neighbors at depth n
    to_visit.emplace(v, 0);
    visited.insert(v);

    while (!to_visit.empty())
    {
        Vertex_handle current = to_visit.front().first;
        int current_depth = to_visit.front().second;
        to_visit.pop();

        // Stop if we've exceeded the desired depth
        if (current_depth == depth)
        {
            result.insert(current);
            continue;
        }

        // Add neighbors to the queue
        Delaunay::Vertex_circulator circulator = dt.incident_vertices(current);
        Delaunay::Vertex_circulator end = circulator;
        do
        {
            if (!dt.is_infinite(circulator) && visited.find(circulator) == visited.end())
            {
                visited.insert(circulator);
                to_visit.emplace(circulator, current_depth + 1);
            }
        } while (++circulator != end);
    }

    return result;
}

double VertexFinder::calculateDensity(std::unordered_set<Vertex_handle> &vertices)
{
    std::vector<Point> subsetPoints = std::vector<Point>();
    for (auto vertex : vertices)
    {
        subsetPoints.push_back(vertex->point());
    }
    std::vector<Point> convexHullPoints = std::vector<Point>();

    CGAL::convex_hull_2(subsetPoints.begin(), subsetPoints.end(), std::back_inserter(convexHullPoints));

    Polygon convexHull(convexHullPoints.begin(), convexHullPoints.end());

    double area = convexHull.area();
    return vertices.size() / area;
}
