#import "../../templates/journal.typ": *
#import "../../shorthands.typ": *
#show: doc => journal_entry(date: datetime(day: 24, month: 11, year: 2024), doc)

Meeting summary:

Enrique gave me a file with results of a simulation of neutrinos interacting with the #Fasernu detector. 

using a jet algorithm called anti-kT, we find a large number of jets from the neutrino interactions. We expect to see a small number of jets, so we might want to edit the radius parameter of the jet algorithm.

We also expect all of the hadrons to be included in the jets, so if we miss some of them, it's a sign that the radius might be too small.

Next Steps:
- Run the analysis with a varying radius parameter to see how the number of jets and the number of included hadrons (and more...) change.
- Create a plot similar to the one in this #link("https://gsalam.web.cern.ch/gsalam/repository/talks/2015-SaoPaulo-lecture4.pdf")[
  #set text(fill: red)
  link
] (page 33)
