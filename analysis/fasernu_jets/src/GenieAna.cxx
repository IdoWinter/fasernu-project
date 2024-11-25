#include "FaserNu_Jets/GenieAna.h"
#include "FaserNu_Jets/GenieEvent.h"
#include "fastjet/ClusterSequence.hh"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include <iostream>
#include <vector>
#include <numeric>

GenieAna::GenieAna()
{
}

GenieAna::~GenieAna()
{
}

void GenieAna::setInput(std::string in)
{
	m_inputFile = in;
}

void GenieAna::setOutput(std::string out)
{
	m_outputFile = out;
}
void GenieAna::init()
{
    f_results = TFile::Open("results.root", "RECREATE");

    h_process = new TH1D("Process", "Process", 2, 0, 2); // 0-1 charged, 1-2 neutral current
	m_histos.push_back(h_process);

	h_nJets = new TH1D("NJets", "NJets", 15, 0, 15); 
    m_histos.push_back(h_nJets);

	h_incomingE = new TH1D("Incoming E", "Incoming E; [GeV]; N/50 GeV", 100, 0, 5000);
    m_histos.push_back(h_incomingE);
    h_nOutgoing = new TH1D("Outgoing N", "Outgoing N; Particles; N", 200, 0, 200);
    m_histos.push_back(h_nOutgoing);
    h_nOutgoingBaryons = new TH1D("Outgoing N B", "Outgoing N B; Baryons; N", 200, 0, 200);
    m_histos.push_back(h_nOutgoingBaryons);
    h_nOutgoingMesons = new TH1D("Outgoing N M", "Outgoing N M; Mesons; N", 200, 0, 1);
    m_histos.push_back(h_nOutgoingMesons);
    h_outgoing_leptonE = new TH1D("Outgoing Lepton E", "Outgoing Lepton E; [GeV]; N/50 GeV", 100, 0, 5000);
    m_histos.push_back(h_outgoing_leptonE);
    h_outgoing_leptonEfrac = new TH1D("Outgoing Lepton Efrac", "Outgoing Lepton Efrac", 102, -0.1, 1.1);
    m_histos.push_back(h_outgoing_leptonEfrac);
	h_outgoing_mesonEfrac = new TH1D("Outgoing Meson Efrac", "Outgoing Meson Efrac", 102, -0.1, 1.1);
	m_histos.push_back(h_outgoing_mesonEfrac);
    h_outgoing_baryonEfrac = new TH1D("Outgoing Baryon Efrac", "Outgoing Baryon Efrac", 102, -0.1, 1.1);
    m_histos.push_back(h_outgoing_baryonEfrac);

    h_avg_num_jets = new TH1D("h_avg_num_jets", "Average Number of Jets vs Radius;Radius;Average Number of Jets", (m_max_radius - m_min_radius) / m_radius_step, m_min_radius, m_max_radius);
    m_histos.push_back(h_avg_num_jets);

    h_std_dev_num_jets = new TH1D("h_std_dev_num_jets", "Standard Deviation of Number of Jets vs Radius;Radius;Standard Deviation", (m_max_radius - m_min_radius) / m_radius_step, m_min_radius, m_max_radius);
    m_histos.push_back(h_std_dev_num_jets);




    TFile* f_in =  TFile::Open(m_inputFile.c_str());
    f_in->GetObject("gFaser",m_tree);
    m_genieEvent = new GenieEvent();
    m_genieEvent->init(m_tree);
}

void GenieAna::process()
{
    for (double R = m_min_radius; R <= m_max_radius; R += m_radius_step) {
        std::vector<int> num_jets_per_event;
        int nEvents = m_tree->GetEntries();
        for (int i = 0; i < nEvents; i++) {


            m_tree->GetEntry(i);
            auto incomingE = m_genieEvent->E->at(0);
            auto px_incoming = m_genieEvent->px->at(0);
            auto py_incoming = m_genieEvent->py->at(0);
            auto pz_incoming = m_genieEvent->pz->at(0);

            h_incomingE->Fill(incomingE);

            int nMesons = 0;
            int nBaryons = 0;
            double E_mesons = 0;
            double E_baryons = 0;
            double E_lepton = 0;
            double px_lepton = 0;
            double py_lepton = 0;
            double pz_lepton = 0;
            double px_neutrino = 0;
            double py_neutrino = 0;
            double pz_neutrino = 0;

            double px_transfered = px_incoming;
            double py_transfered = py_incoming;
            double pz_transfered = pz_incoming;
            double E_transfered = incomingE;
            std::vector<fastjet::PseudoJet> jet_inputs;

            for (int i_part = 0; i_part < m_genieEvent->pdgc->size(); i_part++) {
                auto pdgc = m_genieEvent->pdgc->at(i_part);
                auto status = m_genieEvent->status->at(i_part);
                auto E = m_genieEvent->E->at(i_part);
                auto px = m_genieEvent->px->at(i_part);
                auto py = m_genieEvent->py->at(i_part);
                auto pz = m_genieEvent->pz->at(i_part);
                
                if (status != 1)
                {
                    continue; // this is not a final state particle
                }
                if (abs(pdgc) < 20)
                {
                    h_outgoing_leptonE->Fill(E);
                    h_outgoing_leptonEfrac->Fill(E / incomingE);
                    E_lepton = E;
                    h_process->Fill( abs(pdgc) % 2);
                    px_transfered -= px;
                    py_transfered -= py;
                    pz_transfered -= pz;
                    E_transfered  -= E;
                
                }
                
                if (abs(pdgc) > 100 && abs(pdgc) < 1000)
                {
                    nMesons++;
                    E_mesons += E;
                    auto meson_input = fastjet::PseudoJet(px, py, pz, E);
                    jet_inputs.push_back(meson_input);

                }
                if (abs(pdgc) > 1000 && abs(pdgc) < 1e6)
                {
                    nBaryons++;
                    E_baryons += E;
                }
            }

            // Perform jet clustering with current radius R
            fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
            fastjet::ClusterSequence cs(jet_inputs, jet_def);
            std::vector<fastjet::PseudoJet> jets = cs.inclusive_jets();

            // Store the number of jets for this event
            num_jets_per_event.push_back(jets.size());
        }

        // Calculate average number of jets and standard deviation
        double sum = std::accumulate(num_jets_per_event.begin(), num_jets_per_event.end(), 0.0);
        double mean = sum / num_jets_per_event.size();

        double sq_sum = std::inner_product(num_jets_per_event.begin(), num_jets_per_event.end(), num_jets_per_event.begin(), 0.0);
        double std_dev = std::sqrt(sq_sum / num_jets_per_event.size() - mean * mean);
        h_avg_num_jets->Fill(R, mean);
        h_std_dev_num_jets->Fill(R, std_dev);
    }

}

void GenieAna::close()
{
   f_results->cd();
   TCanvas* canvas = new TCanvas("canvas", "Canvas for Histograms", 800, 600);
    // Open the PDF file
    canvas->Print("output.pdf[");
    for (auto h : m_histos)
    { 
    	h->Draw();
    	canvas->Print("output.pdf");
	}
    canvas->Print("output.pdf]");
}
