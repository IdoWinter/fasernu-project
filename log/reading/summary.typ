#import "../templates/physics_homework.typ": *
#import "../shorthands.typ": *
#import "@preview/unify:0.6.1": qty, unit
#set document(author: ("Ido Winter"))

#show: doc => homework_doc(title: "Project Summary and Discussions", authors: (
  (name: "Ido Winter", id: 207583360),
), doc)


= #Fasernu detector

#Fasernu is a detector located in front of the beam collision point of the LHC (in front of ATLAS).

#Fasernu is located in such a way that the particles that reach it are long lived and weakly interacting particles, such as neutrinos, which are not detected by the ATLAS detector (due to its location and the interaction strength of the neutrino).

#Fasernu is built of a series of layers. Each layer has a #qty(1, "mm") of tungsten and an emulsion film with length ~#qty(200, "micro meter"). There are 1000 layers.

The emulsion films are taken out and developed each 3 months.

