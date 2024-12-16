#ifndef GENIE_ANA_H_
#define GENIE_ANA_H_

#include "FaserNu_Jets/GenieEvent.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "FaserNu_Jets/CustomJetAlgorithm.h"

#include <string>
class GenieAna
{

public: 
   GenieAna();
   ~GenieAna();
   void init();
   void process();
   void close();

   void setInput(std::string input);
   void setOutput(std::string output);
protected:
   void createRegimeHistograms();
   void createRegimeHistogram(std::string name, std::shared_ptr<TH1D> (*createHistogramFunction)()); 
   void analyzeSingleEvent(int i, std::function<void(double, int, int, double, double, CustomJetAlgorithm*)> cb);

   void fillHistograms(double radius, int nBaryons, int nMesons, double E_baryons, double E_mesons, CustomJetAlgorithm* jetAlgorithm);

   void inspectSingleEvent(double radius, int nBaryons, int nMesons, double E_baryons, double E_mesons, CustomJetAlgorithm *jetAlgorithm);

   GenieEvent* m_genieEvent; 
   std::string m_inputFile;
   std::string m_outputFile;

   std::vector<TH1D*> m_histos;
   double m_min_radius = 0.1;
   double m_max_radius = 8;
   double m_radius_step = 0.1;

   TH1D* h_process;
   TH1D* h_nJets;
   TH1D* h_incomingE;
   TH1D* h_outgoing_leptonE;
   TH1D* h_outgoing_leptonEfrac;
   TH1D* h_outgoing_mesonEfrac;
   TH1D* h_outgoing_baryonEfrac;
   TH1D* h_nOutgoing;
   TH1D* h_nOutgoingBaryons;
   TH1D* h_nOutgoingMesons;
   TH1D* h_radius_of_jet_in_event;
   TH1D* h_missing_energy_in_jet;
   TH1D* h_missing_momentum_in_jet;
   TH2F* h_energy_containment_per_radius;

   double m_minimum_energy_containment = 0.6;
   TH1D* h_delta_pt;
   TH1D* h_delta_alpha_t;
   TH1D* h_delta_phi_t;

   std::map<std::string, std::vector<std::shared_ptr<TH1D>>> h_regime_histograms; 

   TTree* m_tree;
   TFile* f_results;
};

#endif
