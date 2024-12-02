#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *

#show: doc => journal_entry(date: datetime(day: 1, month: 12, year: 2024), doc)


Work done today:
- Changed the way we calculate the radius of the jet. Now we are looking at the radius which contains X% of the energy of the jet (See @90-containment for a 90% containment)
- Since we found this interesting pattern which suggests 2 regimes, we decided to look at them separately and plot some variables. Interestingly, we found that the number of baryons in the jet is different in each regime (See @baryons). This suggests that the "large radius" events are events in which the neutrino is breaking the nucleus, which causes the jet to have more baryons. This is interesting because it suggests that we can use the number of baryons to classify the events.
- We graphed the mesons and baryons' average distance, and bizarrely found that the baryons are closer to the jet axis than the mesons (See @baryon-distance and @meson-distance).

#figure(image("./data/radius_90_containment.svg")) <90-containment>

#figure(image("data/number_of_baryons.png"), caption: "Top: small radius regime, Bottom: large radius regime") <baryons>

#figure(image("data/baryon_distance.png"), caption: "The average baryon distances") <baryon-distance>
#figure(image("data/meson_distance.png"), caption: "The average meson distances") <meson-distance>
