#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 15, month: 1, year: 2025), doc)

We started to work on locating vertices from the data of the tracks.

The current implementation of the algorithm uses Delaunay triangulation to find the closest points, and iterates over all faces from smallest to largest. If the neighbourhood of a face passes certain criteria, we assume that there is a vertex and calculate its position using a weighted average of the neighbourhood points.

This works well on simulated data, and the next step is to run it on the real data.

To do this, we triangulate over tracks which begin on the same film, so the triangulation is done on a smaller subset of the data.