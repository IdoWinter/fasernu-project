#include "TTree.h"
#include "FaserNu_Jets/GenieEvent.h" 

GenieEvent::GenieEvent()
{
}

GenieEvent::~GenieEvent()
{
}

void GenieEvent::init(TTree* t)
{
// Set object pointer
   name = 0;
   pdgc = 0;
   status = 0;
   firstMother = 0;
   lastMother = 0;
   firstDaughter = 0;
   lastDaughter = 0;
   px = 0;
   py = 0;
   pz = 0;
   E = 0;
   m = 0;
   M = 0;
   Charge = 0;
   // Set branch addresses and branch pointers
   
   if (!t) return;
   TTree* fChain = t;
   fChain->SetBranchAddress("vx", &vx, &b_vx);
   fChain->SetBranchAddress("vy", &vy, &b_vy);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("n", &n, &b_n);
   fChain->SetBranchAddress("name", &name, &b_name);
   fChain->SetBranchAddress("pdgc", &pdgc, &b_pdgc);
   fChain->SetBranchAddress("status", &status, &b_status);
   fChain->SetBranchAddress("firstMother", &firstMother, &b_firstMother);
   fChain->SetBranchAddress("lastMother", &lastMother, &b_lastMother);
   fChain->SetBranchAddress("firstDaughter", &firstDaughter, &b_firstDaughter);
   fChain->SetBranchAddress("lastDaughter", &lastDaughter, &b_lastDaughter);
   fChain->SetBranchAddress("px", &px, &b_px);
   fChain->SetBranchAddress("py", &py, &b_py);
   fChain->SetBranchAddress("pz", &pz, &b_pz);
   fChain->SetBranchAddress("E", &E, &b_E);
   fChain->SetBranchAddress("m", &m, &b_m);
   fChain->SetBranchAddress("M", &M, &b_M);
   fChain->SetBranchAddress("Charge", &Charge, &b_Charge);
}

