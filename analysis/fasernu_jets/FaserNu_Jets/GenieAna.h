#ifndef GENIE_ANA_H_
#define GENIE_ANA_H_

#include "FaserNu_Jets/GenieEvent.h"
#include "TTree.h"
#include "TH1D.h"

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

   GenieEvent* m_genieEvent; 
   std::string m_inputFile;
   std::string m_outputFile;

   std::vector<TH1D*> m_histos;
   double m_min_radius = 0.1;
   double m_max_radius = 3.0;
   double m_radius_step = 0.05;

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
   TH1D* h_avg_num_jets;
   TH1D* h_std_dev_num_jets;
   TTree* m_tree;
   TFile* f_results;
};

#endif
