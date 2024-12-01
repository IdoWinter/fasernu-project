#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 1, month: 12, year: 2024), doc)

Work done today:

- Most of the work today was to familiarize myself with the data and setting up a good workflow. I fixed some bugs and started to look at what would be a good way to find a variable which is correlated with the radius.
- Most of the "double" neutrino events were just bugs in the program, but there are still some events which have this. I am filtering out these events for now as there's not many of them.
- For every event, I am searching for the farthest particle from the location where the jet should be. This can be seen in @most_distant_particle.
- We can see that the average "radius" (if we want to include all particles in the jet) is 6, and to "capture" all of the cases, we would want to have a radius of about 12.
- Now that I have the "radius" of the jet for each event I can start to search for correlated variables.
- Need to understand from Enrique what data we will actually have access to in order to determine the possible variables to look at (since we won't have the jet vector for the real data).


#figure(image("./data/most_distant_particle.svg")) <most_distant_particle>