#include "FaserNu_Jets/GenieAna.h"
#include "FaserNu_Jets/GenieEvent.h"
#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "fastjet/PseudoJet.hh"
#include <iostream>
#include <vector>
#include <numeric>
#include "GenieAna.h"

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

    createRegimeHistograms();

    TFile *f_in = TFile::Open(m_inputFile.c_str());
    f_in->GetObject("gFaser", m_tree);
    m_genieEvent = new GenieEvent();
    m_genieEvent->init(m_tree);
}

void GenieAna::createRegimeHistograms()
{
    createRegimeHistogram("nBaryons", []() {
        return std::make_shared<TH1D>("nBaryons", "Number of Baryons; Baryons; N", 200, 0, 200);
    });
    createRegimeHistogram("nMesons", []() {
        return std::make_shared<TH1D>("nMesons", "Number of Mesons; Mesons; N", 200, 0, 200);
    });

    // Plot the energy of the jet
    createRegimeHistogram("jetEnergy", []() {
        return std::make_shared<TH1D>("jetEnergy", "Jet Energy; Energy [GeV]; N/50 GeV", 100, 0, 5000);
    });

    // Plot the energy of the mesons
    createRegimeHistogram("mesonEnergy", []() {
        return std::make_shared<TH1D>("mesonEnergy", "Meson Energy; Energy [GeV]; N/50 GeV", 100, 0, 5000);
    });

    // Plot the energy of the baryons
    createRegimeHistogram("baryonEnergy", []() {
        return std::make_shared<TH1D>("baryonEnergy", "Baryon Energy; Energy [GeV]; N/50 GeV", 100, 0, 300);
    });

    // Plot the percentage of baryon energy in relation to the total energy
    createRegimeHistogram("baryonEnergyFraction", []() {
        return std::make_shared<TH1D>("baryonEnergyFraction", "Baryon Energy Fraction; Energy Fraction; N", 102, -0.1, 1.1);
    });

    // Plot the average distance of baryons from the jet axis
    createRegimeHistogram("averageBaryonDistance", []() {
        return std::make_shared<TH1D>("averageBaryonDistance", "Average Baryon Distance; Distance; N", 100, 0, 10);
    });

    // Plot the average distance of mesons from the jet axis
    createRegimeHistogram("averageMesonDistance", []() {
        return std::make_shared<TH1D>("averageMesonDistance", "Average Meson Distance; Distance; N", 100, 0, 10);
    });

}

void GenieAna::createRegimeHistogram(std::string name, std::shared_ptr<TH1D> (*createHistogramFunction)() ) 
{
    h_regime_histograms[name] = std::vector<std::shared_ptr<TH1D>>();
    for (int i = 0; i < 2; i++)
    {
        h_regime_histograms[name].push_back(createHistogramFunction());
    }
}

void GenieAna::process()
{
    CustomJetAlgorithm *jet_algorithm;


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

        fillHistograms(radius, nBaryons, nMesons, E_baryons, E_mesons, jet_algorithm);
    }
}

void GenieAna::fillHistograms(double radius, int nBaryons, int nMesons, double E_baryons, double E_mesons, CustomJetAlgorithm* jetAlgorithm)
{
    float first_regime_cutoff = 2.5;
    float second_regime_initial = 4.3;
    int regime = -1;
    if (radius < first_regime_cutoff)
    {
        regime = 0;
    }
    else if (radius > second_regime_initial)
    {
        regime = 1;
    }

    if (regime < 0)
    {
        return;
    }

    auto hist = h_regime_histograms.at("nBaryons").at(regime);
    hist->Fill(nBaryons);
    hist = h_regime_histograms.at("nMesons").at(regime);
    hist->Fill(nMesons);
    hist = h_regime_histograms.at("jetEnergy").at(regime);
    hist->Fill(jetAlgorithm->getJetVector().E());
    hist = h_regime_histograms.at("mesonEnergy").at(regime);
    hist->Fill(E_mesons);
    hist = h_regime_histograms.at("baryonEnergy").at(regime);
    hist->Fill(E_baryons);
    hist = h_regime_histograms.at("baryonEnergyFraction").at(regime);
    hist->Fill(E_baryons / (jetAlgorithm->getJetVector().E()));
    
    hist = h_regime_histograms.at("averageBaryonDistance").at(regime);
    hist->Fill(jetAlgorithm->getAverageBaryonDistance());
    hist = h_regime_histograms.at("averageMesonDistance").at(regime);
    hist->Fill(jetAlgorithm->getAverageMesonDistance());
    
}
void GenieAna::close()
{
    f_results->cd();
    TCanvas *canvas = new TCanvas("canvas", "Canvas for Histograms", 800, 600);
    
    for (auto h : m_histos)
    {
        h->Draw();
        
    }

    for (auto regime : h_regime_histograms)
    {
        for (auto hist : regime.second)
        {
            hist->Draw();
        }
    }
    
    canvas->SaveAs("output.svg");

    std::string pdfFileName = "RegimeHistograms.pdf";
    bool firstPage = true;

    for (auto it = h_regime_histograms.begin(); it != h_regime_histograms.end(); ++it)
    {
        auto regime = *it;

        // Create a canvas for the current regime
        std::string canvasName = "canvas_" + regime.first;
        TCanvas* canvas = new TCanvas(canvasName.c_str(), canvasName.c_str(), 800, 600);

        int nHistograms = regime.second.size();
        canvas->Divide(1, nHistograms); // Divide canvas into nHistograms pads vertically

        int padNumber = 1;
        for (auto hist : regime.second)
        {
            canvas->cd(padNumber);
            hist->Draw();
            padNumber++;
        }

        // Save canvas to a PDF file, creating a new page for each regime
        if (firstPage)
        {
            canvas->Print((pdfFileName + "(").c_str()); // Open the PDF file
            firstPage = false;
        }
        else
        {
            canvas->Print(pdfFileName.c_str()); // Add to the PDF file
        }

        // Close the PDF file after the last regime
        if (std::next(it) == h_regime_histograms.end())
        {
            canvas->Print((pdfFileName + ")").c_str()); // Close the PDF file
        }

        delete canvas;
    }
}
