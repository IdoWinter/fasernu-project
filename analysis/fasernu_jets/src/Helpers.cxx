#include "Helpers.h"
#include <TMath.h>

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

double ComputeRij(Particle *p1, Particle *p2)
{
    double delta_rapidity = p1->momentum->PseudoRapidity() - p2->momentum->PseudoRapidity();
    double delta_phi = p1->momentum->Phi() - p2->momentum->Phi();
    return TMath::Sqrt(TMath::Sq(delta_rapidity) + TMath::Sq(delta_phi));
}

TVectorD linspace(double start, double end, int numPoints)
{
    TVectorD values(numPoints);
    if (numPoints < 2) {
        values[0] = start; // Single value if only 1 point
        return values;
    }

    double step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        values[i] = start + i * step;
    }

    return values;
}


TVectorD calcFunc(TVectorD x, std::function<double(double)> f)
{
    TVectorD y(x.GetNoElements());
    for (int i = 0; i < x.GetNoElements(); i++)
    {
        y[i] = f(x[i]);
    }
    return y;
}
