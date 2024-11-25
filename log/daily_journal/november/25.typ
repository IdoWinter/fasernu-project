#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 25, month: 11, year: 2024), doc)

Summary of Today's Work:

- Setup and Initial Runs: Successfully set up the environment and ran initial tests using the jet algorithm.
- Data Visualization: Plotted data showing the behavior of jet numbers across different radii, finding a decrease with increasing radii. No optimal cutoff radius identified yet (See @anti_kt_num_jets_v_radius).
- Custom Jet Algorithm: Implemented a custom algorithm (See @new_algo_missing_particles):
  - Assumes jets' energy and momentum align with CC event characteristics (e.g., negative momentum relative to leptons, energy summing to neutrino energy).
  - Issues Identified:
    - Multiple neutrinos and leptons in a single event are problematic, disrupting algorithm reliability.

#figure(image("data/anti_kt_num_jets_v_radius.png")) <anti_kt_num_jets_v_radius>

#figure(image("data/new_algo_missing_particles.png")) <new_algo_missing_particles>

Next Steps:

- Debug code to resolve multiple neutrino/lepton event handling.
- Determine an optimal jet radius for the custom algorithm:
  - Evaluate physical significance.
  - Plot relevant metrics to assess jet completeness and structure.
  - Find radius which saturates the number of included hadrons (or understand why one does not exist).
