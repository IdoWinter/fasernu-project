#ifndef GENIE_EVENT_H_
#define GENIE_EVENT_H_

#include "TH1D.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class GenieEvent
{
public:
  GenieEvent();
  ~GenieEvent();
  
   void init(TTree* t);
  

   Double_t        vx;
   Double_t        vy;
   Double_t        vz;
   Int_t           n;
   vector<string>  *name;
   vector<int>     *pdgc;
   vector<int>     *status;
   vector<int>     *firstMother;
   vector<int>     *lastMother;
   vector<int>     *firstDaughter;
   vector<int>     *lastDaughter;
   vector<double>  *px;
   vector<double>  *py;
   vector<double>  *pz;
   vector<double>  *E;
   vector<double>  *m;
   vector<double>  *M;
   vector<double>  *Charge;

   // List of branches
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_n;   //!
   TBranch        *b_name;   //!
   TBranch        *b_pdgc;   //!
   TBranch        *b_status;   //!
   TBranch        *b_firstMother;   //!
   TBranch        *b_lastMother;   //!
   TBranch        *b_firstDaughter;   //!
   TBranch        *b_lastDaughter;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_E;   //!
   TBranch        *b_m;   //!
   TBranch        *b_M;   //!
   TBranch        *b_Charge;   //!

};
#endif
