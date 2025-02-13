#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 10, month: 2, year: 2025), doc)

We ended up changing to algorithm to use a density based algorithm called DBSCAN. This algorithm is more robust to noise and outliers, and is able to find clusters of points in the data. It is also $n log(n)$.

It looks like the algorithm works well, and setting $"minPts"=10$, $epsilon=20$ we get $approx 1000$  clusters for the data.

We then select only the clusters which have at least 1 long track (for the muon). After this we get $approx 350$ clusters.

The next step is to try and run a Kalman Filter algorithm on each cluster that we found, and to see if we can estimate the location of the vertex and the originating tracks.

Since it is complicated to run implement the Kalman Filter algorithm, we want to use the "ACTS" framework in order to do it.

I am currently trying to understand how to parse the data into the relevant format for the ACTS framework.