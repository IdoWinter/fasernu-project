#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 1, month: 12, year: 2024), doc)

Work done today:

- Most of the "double" neutrino events were just bugs in the program, but there are still some events which have this. I am filtering out these events for now as there's not many of them.
- For every event, I am searching for the farthest particle from the location where the jet should be. This can be seen in 

// #image("./data/most_distant_particle.svg")