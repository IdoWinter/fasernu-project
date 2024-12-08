#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *


#show: doc => journal_entry(date: datetime(day: 5, month: 12, year: 2024), doc)


Meeting summary:
- Some Muons reach #Fasernu from the LHC as well. We can use the data about these muons and their interaction (through $gamma$) with the nucleus to calibrate our algorithm for the neutrino interactions, as it should be largely the same (we still expect to see wide jets).
- Discussed the paper "Measurement of nuclear effects in neutrino interactions with minimal dependence on neutrino energy" [PHYSICAL REVIEW C 94, 015503]. We will try to use $delta (p_T), delta alpha_T, delta phi.alt_T$ and see if they are indeed independent of the neutrino energy.

Work done today:
- Plotted the variables in our simulation, and found that it's consistent with the paper (to some point) (See  @delta_pt, @delta_alpha_t, @delta_phi_t).

#figure(image("data/delta_pt.svg"), caption: [$delta p_T$]) <delta_pt>

#figure(image("data/delta_alpha_t.svg"), caption: [$delta alpha_T$]) <delta_alpha_t>

#figure(image("data/delta_phi_t.svg"), caption: [$delta phi.alt_T$]) <delta_phi_t>
