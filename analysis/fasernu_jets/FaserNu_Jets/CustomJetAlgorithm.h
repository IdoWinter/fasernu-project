#ifndef CUSTOM_JET_ALGORITHM_H_
#define CUSTOM_JET_ALGORITHM_H_


#include "FaserNu_Jets/GenieEvent.h"
#include "TTree.h"
#include "TH1D.h"
#include <TLorentzVector.h>
#include <cmath>

#include <string>



struct Particle {
    TLorentzVector *momentum;
    int pdgc;
    std::string name;

    Particle(TLorentzVector *momentum, int pdgc, std::string name) : momentum(momentum), name(name), pdgc(pdgc) 
    {}

    friend std::ostream& operator<<(std::ostream& os, const Particle& p) {
       os << "Particle(name: " << p.name
        << ", px: " << p.momentum->Px()
        << ", py: " << p.momentum->Py()
        << ", pz: " << p.momentum->Pz()
        << ", energy: " << p.momentum->E()
       << ")";
        return os;
    }
};

class CustomJetAlgorithm
{

public: 
   CustomJetAlgorithm();
   ~CustomJetAlgorithm();
   void addParticle(Particle* particle);
   
   double findActualRadius();
   void setExpectedRadius(double radius);

   TLorentzVector getJetVector();

   std::vector<Particle*> getMissingParticles();

   bool isValid();

   protected:
   void add_lepton(Particle *lepton);
   void add_neutrino(Particle* neutrino);
   void add_jet_particle(Particle* particle);

    Particle* m_lepton;
    Particle* m_neutrino;
    std::vector<Particle*> m_candidate_jet_particles;

    double m_expected_radius = 0.4;

    std::vector<Particle*> m_output_jet_particles;
    bool m_isValid;
};

#endif