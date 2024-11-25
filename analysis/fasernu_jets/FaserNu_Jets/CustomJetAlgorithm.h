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
};

class CustomJetAlgorithm
{

public: 
   CustomJetAlgorithm();
   ~CustomJetAlgorithm();
   void add_particle(Particle* particle);
   
   void process();
   void set_expected_radius(double radius);

   TLorentzVector get_jet_vector();

   std::vector<Particle*> get_missing_particles();

   protected:
   void add_lepton(Particle *lepton);
   void add_neutrino(Particle* neutrino);
   void add_jet_particle(Particle* particle);

    Particle* m_lepton;
    Particle* m_neutrino;
    std::vector<Particle*> m_candidate_jet_particles;

    double m_expected_radius = 0.4;

    std::vector<Particle*> m_output_jet_particles;
};

#endif