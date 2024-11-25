
// CPP includes
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>


// C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// ROOT includes
#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TRint.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TBranch.h>
#include <TH1.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TPad.h>
#include <TStyle.h>
#include <TKey.h>
#include <TLegend.h>
#include <TText.h>
#include <TPaveText.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TEnv.h>
#include <TDatabasePDG.h>
#include "TObjString.h"
#include "TRandom3.h"
#include "TObjArray.h"
#include "TFrame.h"

using namespace std;


// Structure for Protojet
struct Protojet
{
    public:
    double kT,eta,phi,y;

    void Set(double kT_, double eta_, double phi_, double y_)
    {
        kT=kT_;
        eta=eta_;
        phi=phi_;
        y=y_;
    }

};

// Structure for Jet
struct Jet
{
    public:
    double kT,eta,phi,y; // R?

    void Set(double kT_, double eta_, double phi_, double y_)
    {
        kT=kT_;
        eta=eta_;
        phi=phi_;
        y=y_;
    }

};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


// kt Algorithm

// Calculates minimum distance of all diB and dij
vector<double> minDist(vector<Protojet> pj_vect)
{
    vector<Protojet> protojet_vector = pj_vect;
    double R = 0.4; // To be define better

    vector<double> diB_vect;
    vector<vector<double>> dij_vect;

    // Begin ith Protojet loop
    for(int i=0; i<protojet_vector.size(); i++)
    {
        Protojet iProJet = protojet_vector.at(i);

        double kT_i = iProJet.kT;
        double eta_i = iProJet.eta;
        double phi_i = iProJet.phi;
        double y_i = iProJet.y;

        double d_iB = kT_i*kT_i;

        diB_vect.push_back(d_iB);

        // Begin jth Protojet loop
        for(int j=i+1; j<protojet_vector.size(); j++)
        {
            // cout << "MD" << " i : " << i << " ; j : " << j << endl;
            Protojet jProJet = protojet_vector.at(j);

            double kT_j = jProJet.kT;
            double eta_j = jProJet.eta;
            double phi_j = jProJet.phi;
            double y_j = jProJet.y;

            double d_jB = kT_j*kT_j;

            double min_dB;
            if(d_iB<d_jB) min_dB = d_iB;
            else min_dB = d_jB;

            double delta_ij_2 = ( ((eta_i - eta_j)*(eta_i - eta_j)) + ((phi_i - phi_j)*(phi_i - phi_j)) );
            double d_ij = min_dB*delta_ij_2/(R*R);

            vector<double> ij_Pair;
            ij_Pair.push_back(d_ij);
            ij_Pair.push_back(i);
            ij_Pair.push_back(j);
            // cout << "i = " << i << " ; j = " << j << endl;
            dij_vect.push_back(ij_Pair);

        } // End of jth Protojet loop

    } // End of ith Protojet loop

    // cout << diB_vect.size() << endl;
    double diB_min = diB_vect.at(0);
    double diB_min_i = 0;
    for(int k=1; k<diB_vect.size(); k++)
    {
        if(diB_vect.at(k) < diB_min)
        {
            diB_min = diB_vect.at(k);
            diB_min_i = k;
        }
    }

    // cout << dij_vect.size() << endl;
    double dij_min = dij_vect.at(0).at(0);
    double dij_min_i = dij_vect.at(0).at(1);
    double dij_min_j = dij_vect.at(0).at(2);
    for(int l=1; l<dij_vect.size(); l++)
    {
        if(dij_vect.at(l).at(0) < dij_min)
        {
            dij_min = dij_vect.at(l).at(0);
            dij_min_i = dij_vect.at(l).at(1);
            dij_min_j = dij_vect.at(l).at(2);

        }
    }


    vector<double> minVals;
    double d_min;
    double i_min;
    double j_min;

    if(diB_min < dij_min)
    {
        d_min = diB_min;
        i_min = diB_min_i;
        j_min = -1;
    }

    else
    {
        d_min = dij_min; // If diB_min >= dij_min, d_min = dij_min
        if(dij_min_i==dij_min_j) cout << "ERROR i = j" << endl;
        i_min = dij_min_i;
        j_min = dij_min_j;
    }

    minVals.push_back(d_min);
    minVals.push_back(i_min);
    minVals.push_back(j_min);

    return minVals;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


// anti-kt Algorithm

// Calculates minimum distance of all diB and dij
vector<double> minDist_antikt(vector<Protojet> pj_vect)
{
    vector<Protojet> protojet_vector = pj_vect;
    double R = 0.4; // To be define better

    vector<double> diB_vect;
    vector<vector<double>> dij_vect;

    // Begin ith Protojet loop
    for(int i=0; i<protojet_vector.size(); i++)
    {
        Protojet iProJet = protojet_vector.at(i);

        double kT_i = iProJet.kT;
        double eta_i = iProJet.eta;
        double phi_i = iProJet.phi;
        double y_i = iProJet.y;

        double d_iB = 1/(kT_i*kT_i);

        diB_vect.push_back(d_iB);

        // Begin jth Protojet loop
        for(int j=i+1; j<protojet_vector.size(); j++)
        {
            // cout << "MD" << " i : " << i << " ; j : " << j << endl;
            Protojet jProJet = protojet_vector.at(j);

            double kT_j = jProJet.kT;
            double eta_j = jProJet.eta;
            double phi_j = jProJet.phi;
            double y_j = jProJet.y;

            double d_jB = 1/(kT_j*kT_j);

            double min_dB;
            if(d_iB<d_jB) min_dB = d_iB;
            else min_dB = d_jB;

            double delta_ij_2 = ( ((eta_i - eta_j)*(eta_i - eta_j)) + ((phi_i - phi_j)*(phi_i - phi_j)) );
            double d_ij = min_dB*delta_ij_2/(R*R);

            vector<double> ij_Pair;
            ij_Pair.push_back(d_ij);
            ij_Pair.push_back(i);
            ij_Pair.push_back(j);
            // cout << "i = " << i << " ; j = " << j << endl;
            dij_vect.push_back(ij_Pair);

        } // End of jth Protojet loop

    } // End of ith Protojet loop

    // cout << diB_vect.size() << endl;
    double diB_min = diB_vect.at(0);
    double diB_min_i = 0;
    for(int k=1; k<diB_vect.size(); k++)
    {
        if(diB_vect.at(k) < diB_min)
        {
            diB_min = diB_vect.at(k);
            diB_min_i = k;
        }
    }

    // cout << dij_vect.size() << endl;
    double dij_min = dij_vect.at(0).at(0);
    double dij_min_i = dij_vect.at(0).at(1);
    double dij_min_j = dij_vect.at(0).at(2);
    for(int l=1; l<dij_vect.size(); l++)
    {
        if(dij_vect.at(l).at(0) < dij_min)
        {
            dij_min = dij_vect.at(l).at(0);
            dij_min_i = dij_vect.at(l).at(1);
            dij_min_j = dij_vect.at(l).at(2);

        }
    }


    vector<double> minVals;
    double d_min;
    double i_min;
    double j_min;

    if(diB_min < dij_min)
    {
        d_min = diB_min;
        i_min = diB_min_i;
        j_min = -1;
    }

    else
    {
        d_min = dij_min; // If diB_min >= dij_min, d_min = dij_min
        if(dij_min_i==dij_min_j) cout << "ERROR i = j" << endl;
        i_min = dij_min_i;
        j_min = dij_min_j;
    }

    minVals.push_back(d_min);
    minVals.push_back(i_min);
    minVals.push_back(j_min);

    return minVals;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


Protojet mergedProtojets(Protojet i_pj, Protojet j_pj)
{
    Protojet pj_i = i_pj;
    Protojet pj_j = j_pj;

    double kT_new = pj_i.kT + pj_j.kT;
    double eta_new = ( ( pj_i.kT * pj_i.eta ) + ( pj_j.kT * pj_j.eta ) ) / kT_new;
    double phi_new = ( ( pj_i.kT * pj_i.phi ) + ( pj_j.kT * pj_j.phi ) ) / kT_new;
    double y_new = ( ( pj_i.kT * pj_i.y ) + ( pj_j.kT * pj_j.y ) ) / kT_new;

    Protojet merged_pj;
    merged_pj.Set(kT_new, eta_new, phi_new, y_new);

    return merged_pj;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// Apply kt and anti-kt: flag system?
// pz of jets
// rapidity instead of pseudo-rapidity (ATLAS uses PR)


// Try different R --> 0.4 (ATLAS), 0.1, other in range 0.1 - 0.4? Smaller than 0.1?
// Plot values of diB, also dij?

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


void JetAlgorithm(const char* filename, const char* treename, const char* outputfilename)
{
    // Open the file
    TFile* inputFile = TFile::Open(filename,"READ");
    if (!inputFile) { return; }
    TTree* var_tree;
    var_tree = (TTree*) inputFile->Get(treename);

    TFile *fnew = new TFile(outputfilename,"recreate");
    TTree *event_tree = var_tree->CloneTree(0);

    int nJets = 0;

    // Jet properties
    vector<double> phiJet;
    // vector<double> pzJet; // think about this
    vector<double> ptJet;
    vector<double> PRJet;
    vector<double> RJet;
    TBranch *b_nJets = event_tree->Branch("nJets",&nJets);
    TBranch *b_phiJet = event_tree->Branch("phiJet",&phiJet);
    // TBranch *b_pzJet = event_tree->Branch("pzJet",&pzJet);
    TBranch *b_ptJet = event_tree->Branch("ptJet",&ptJet);
    TBranch *b_PRJet = event_tree->Branch("PRJet",&PRJet);
    TBranch *b_RJet = event_tree->Branch("RJet",&RJet);

    int event_flag_CC, nCharged;
    var_tree->SetBranchAddress("event_flag_CC", &event_flag_CC);
    var_tree->SetBranchAddress("nCharged", &nCharged);


    vector<double> *PhiHadron = 0;
    vector<double> *ptHadron = 0;
    vector<double> *PRHadron = 0;
    vector<double> *RHadron = 0;

    TBranch *b_PhiHadron = 0;
    TBranch *b_ptHadron = 0;
    TBranch *b_PRHadron = 0;
    TBranch *b_RHadron = 0;

    var_tree->SetBranchAddress("PhiHadron", &PhiHadron, &b_PhiHadron);
    var_tree->SetBranchAddress("ptHadron", &ptHadron, &b_ptHadron);
    var_tree->SetBranchAddress("PRHadron", &PRHadron, &b_PRHadron);
    var_tree->SetBranchAddress("RHadron", &RHadron, &b_RHadron);

    int nentries = var_tree->GetEntries();
    // int nentries = 10000;
    cout << "Number of entries: " << nentries << endl << endl;

    for(int n=0; n<nentries; n++)
    {
        if (n%5000==0) cout << "Event number: " << n << endl;

        phiJet.clear();
        ptJet.clear();
        PRJet.clear();
        RJet.clear();

        var_tree->GetEntry(n);
        // event_tree->Fill();


        if(event_flag_CC==1&&nCharged>1)
        {
            // nCharged = number of charged hadrons + charged lepton
            int nChargedHadrons = nCharged - 1;
            // cout << "nChargedHadrons: " << nChargedHadrons << endl;

            // Make vector of Protojets
            vector<Protojet> protojet_vect;

            for(int v=0; v<nChargedHadrons; v++)
            {
                Protojet hadron_protojet;
                hadron_protojet.Set(ptHadron->at(v), PRHadron->at(v), PhiHadron->at(v), RHadron->at(v));
                protojet_vect.push_back(hadron_protojet);
            }

            // Make vector of Jets
            vector<Jet> jet_vect;

            // Jet reclustering algorithm
            for(int m=0; m<1000; m++)
            {
                if(protojet_vect.size() == 0) break;

                if(protojet_vect.size() == 1)
                {
                    Protojet last_protojet = protojet_vect.at(0);
                    Jet last_jet;
                    last_jet.Set(last_protojet.kT, last_protojet.eta, last_protojet.phi, last_protojet.y);

                    // Add to Jet vector
                    jet_vect.push_back(last_jet);

                    // Remove last protojet from protojet vector
                    protojet_vect.erase(protojet_vect.begin());
                    continue;
                }

                vector<double> mD = minDist(protojet_vect);

                // Is it a jet or protojet
                int Jet_flag;
                if(mD.at(2) == -1) // dmin is diB
                {
                    Jet_flag = 1;
                }
                else  // dmin is dij
                {
                    Jet_flag = 0;
                }


                // dmin is diB
                if(Jet_flag)
                {
                    int index = mD.at(1);
                    Protojet protojet_Jet = protojet_vect.at(index);
                    Jet new_jet;
                    new_jet.Set(protojet_Jet.kT, protojet_Jet.eta, protojet_Jet.phi, protojet_Jet.y);

                    // Add to Jet vector
                    jet_vect.push_back(new_jet);

                    // Remove from protojet vector
                    protojet_vect.erase(protojet_vect.begin() + index);
                }

                // dmin is dij
                else
                {
                    int index_i = mD.at(1);
                    int index_j = mD.at(2);
                    Protojet protojet_i = protojet_vect.at(index_i);
                    Protojet protojet_j = protojet_vect.at(index_j);

                    Protojet new_protojet = mergedProtojets(protojet_i, protojet_j);

                    // Removes merged protojets from protojet vector
                    protojet_vect.erase(protojet_vect.begin() + index_i);
                    protojet_vect.erase(protojet_vect.begin() + (index_j - 1) ); // After ith value is removed, value at initial jth position shifts to (j-1)th position

                    // Adds new merged protojet to protojet vector
                    protojet_vect.push_back(new_protojet);
                }

            } // End Jet reclustering algorithm



            nJets = jet_vect.size();
            // b_nJets->Fill();

            for(int x=0; x<nJets; x++)
            {
                // Jet Jetx;
                // Jetx = jet_vect.at(x);

                phiJet.push_back(jet_vect.at(x).phi);
                // pzJet; // think about this
                ptJet.push_back(jet_vect.at(x).kT);
                PRJet.push_back(jet_vect.at(x).eta);
                RJet.push_back(jet_vect.at(x).y);

                // b_phiJet->Fill();
                // b_ptJet->Fill();
                // b_PRJet->Fill();
                // b_RJet->Fill();
            }

            event_tree->Fill();


        } // End of CC==1 if

        // upd("FASER_numu_W184_FLAT.tree_jets.root", "T", jet_vect)

        else continue;

    }// End of event loop




    // Write Ttrees
    fnew->cd();
    event_tree->Print();
    fnew->Write();
    fnew->Close();

}

void JetC(){
    // JetAlgorithm("../ReadData/GENIESim/FASERnu.FK.2010GENIE.200000.gfaser.13.SubV.nu14.reco.variable_tree.root", "T", "FASERnu.FK.2010GENIE.200000.gfaser.13.SubV.nu14.reco.variable_tree.jets.root");
    // JetAlgorithm("../ReadData/GENIESim/FASERnu.FK.PYTHIADefault.100000.gfaser.13.SubV.nu14.reco.variable_tree.root", "T", "FASERnu.FK.PYTHIADefault.100000.gfaser.13.SubV.nu14.reco.variable_tree.jets.root");
    // JetAlgorithm("../ReadData/GENIESim/FASERnu.FK.NUX.100000.gfaser.13.SubV.nu14.reco.variable_tree.root", "T", "FASERnu.FK.NUX.100000.gfaser.13.SubV.nu14.reco.variable_tree.jets.root");
    // JetAlgorithm("../ReadData/GENIESim/FASERnu.FK.HERMES.100000.gfaser.13.SubV.nu14.reco.variable_tree.root", "T", "FASERnu.FK.HERMES.100000.gfaser.13.SubV.nu14.reco.variable_tree.jets.root");
    // JetAlgorithm("../ReadData/GENIESim/FASERnu.FK.2021GlobalFit.100000.gfaser.13.SubV.nu14.reco.variable_tree.root", "T", "FASERnu.FK.2021GlobalFit.100000.gfaser.13.SubV.nu14.reco.variable_tree.jets.root");

    // JetAlgorithm("../Monochrome/gfaser_variable_tree/FASERnu.2010GENIE.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.root", "T_2010GENIE", "FASERnu.2010GENIE.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.jets.root");
    // JetAlgorithm("../Monochrome/gfaser_variable_tree/FASERnu.PYTHIADefault.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.root", "T_PYTHIADefault", "FASERnu.PYTHIADefault.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.jets.root");
    // JetAlgorithm("../Monochrome/gfaser_variable_tree/FASERnu.NUX.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.root", "T_NUX", "FASERnu.NUX.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.jets.root");
    // JetAlgorithm("../Monochrome/gfaser_variable_tree/FASERnu.HERMES.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.root", "T_HERMES", "FASERnu.HERMES.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.jets.root");
    // JetAlgorithm("../Monochrome/gfaser_variable_tree/FASERnu.2021GlobalFit.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.root", "T_2021GlobalFit", "FASERnu.2021GlobalFit.14.1000741840.1000GeV.100000.gfaser.truth.variable_tree.jets.root");
}



