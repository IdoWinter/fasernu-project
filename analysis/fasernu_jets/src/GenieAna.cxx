#include "FaserNu_Jets/GenieAna.h"
#include "FaserNu_Jets/GenieEvent.h"
#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "fastjet/PseudoJet.hh"
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
    // m_histos.push_back(h_process);

    h_nJets = new TH1D("NJets", "NJets", 15, 0, 15);
    // m_histos.push_back(h_nJets);

    h_incomingE = new TH1D("Incoming E", "Incoming E; [GeV]; N/50 GeV", 100, 0, 5000);
    // m_histos.push_back(h_incomingE);
    h_nOutgoing = new TH1D("Outgoing N", "Outgoing N; Particles; N", 200, 0, 200);
    // m_histos.push_back(h_nOutgoing);
    h_nOutgoingBaryons = new TH1D("Outgoing N B", "Outgoing N B; Baryons; N", 200, 0, 200);
    // m_histos.push_back(h_nOutgoingBaryons);
    h_nOutgoingMesons = new TH1D("Outgoing N M", "Outgoing N M; Mesons; N", 200, 0, 1);
    // m_histos.push_back(h_nOutgoingMesons);
    h_outgoing_leptonE = new TH1D("Outgoing Lepton E", "Outgoing Lepton E; [GeV]; N/50 GeV", 100, 0, 5000);
    // m_histos.push_back(h_outgoing_leptonE);
    h_outgoing_leptonEfrac = new TH1D("Outgoing Lepton Efrac", "Outgoing Lepton Efrac", 102, -0.1, 1.1);
    // m_histos.push_back(h_outgoing_leptonEfrac);
    h_outgoing_mesonEfrac = new TH1D("Outgoing Meson Efrac", "Outgoing Meson Efrac", 102, -0.1, 1.1);
    // m_histos.push_back(h_outgoing_mesonEfrac);
    h_outgoing_baryonEfrac = new TH1D("Outgoing Baryon Efrac", "Outgoing Baryon Efrac", 102, -0.1, 1.1);
    // m_histos.push_back(h_outgoing_baryonEfrac);

    h_radius_of_jet_in_event = new TH1D("Radius of jet", "Radius of jet; radius; number of events", int((m_max_radius - m_min_radius) / m_radius_step), m_min_radius, m_max_radius);
    m_histos.push_back(h_radius_of_jet_in_event);

    TFile *f_in = TFile::Open(m_inputFile.c_str());
    f_in->GetObject("gFaser", m_tree);
    m_genieEvent = new GenieEvent();
    m_genieEvent->init(m_tree);
}

void GenieAna::process()
{
    CustomJetAlgorithm *jet_algorithm;
    double R = 0.5;
    std::cout << "Processing radius " << R << std::endl;
    int nEvents = m_tree->GetEntries();
    for (int i = 0; i < nEvents; i++)
    {
        // std::cout << "Processing item " << i << std::endl;
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

        jet_algorithm = new CustomJetAlgorithm();
        jet_algorithm->setExpectedRadius(R);

        for (int i_part = 0; i_part < m_genieEvent->pdgc->size(); i_part++)
        {
            auto pdgc = m_genieEvent->pdgc->at(i_part);
            auto status = m_genieEvent->status->at(i_part);
            auto E = m_genieEvent->E->at(i_part);
            auto px = m_genieEvent->px->at(i_part);
            auto py = m_genieEvent->py->at(i_part);
            auto pz = m_genieEvent->pz->at(i_part);
            auto name = m_genieEvent->name->at(i_part);

            if (status > 1)
            {
                continue; // this is not a final state particle
            }

            if (pdgc >= 10e6)
            {
                // This is the nucleus.
                continue;
            }

            jet_algorithm->addParticle(new Particle(
                new TLorentzVector(px, py, pz, E),
                pdgc,
                name));

            if (abs(pdgc) < 20)
            {
                h_outgoing_leptonE->Fill(E);
                h_outgoing_leptonEfrac->Fill(E / incomingE);
                E_lepton = E;
                h_process->Fill(abs(pdgc) % 2);
                px_transfered -= px;
                py_transfered -= py;
                pz_transfered -= pz;
                E_transfered -= E;
            }

            if (abs(pdgc) > 100 && abs(pdgc) < 1000)
            {
                nMesons++;
                E_mesons += E;
            }
            if (abs(pdgc) > 1000 && abs(pdgc) < 1e6)
            {
                nBaryons++;
                E_baryons += E;
            }
        }

        // Perform jet clustering with current radius R
        auto radius = jet_algorithm->findActualRadius();

        // Store the number of jets for this event
        auto jet_vector = jet_algorithm->getJetVector();

        if (!jet_algorithm->isValid())
        {
            continue;
        }

        h_radius_of_jet_in_event->Fill(radius);
    }
}

void GenieAna::close()
{
    f_results->cd();
    TCanvas *canvas = new TCanvas("canvas", "Canvas for Histograms", 800, 600);
    
    for (auto h : m_histos)
    {
        h->Draw();
        
    }
    
    canvas->SaveAs("output.svg");
}
