#include <vector>
#include <iostream>
#include "CustomJetAlgorithm.h"
#include <utility>


std::pair<int, int> count_mesons_and_baryons(std::vector<Particle*> particles) {
    int nMesons = 0;
    int nHadrons = 0;
    for (auto particle : particles) {
        if (abs(particle->pdgc) >= 100 && abs(particle->pdgc) < 1000) {
            nMesons++;
        }
        if (abs(particle->pdgc) >= 1000 && abs(particle->pdgc) < 1e6) {
            nHadrons++;
        }
    }
    return std::make_pair(nMesons, nHadrons);
}

