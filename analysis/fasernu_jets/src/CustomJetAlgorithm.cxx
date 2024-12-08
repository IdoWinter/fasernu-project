#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include "Helpers.h"
#include "CustomJetAlgorithm.h"
#include <memory>
#include <TMath.h>

CustomJetAlgorithm::CustomJetAlgorithm() 
: m_lepton(nullptr), m_neutrino(nullptr), m_candidate_jet_particles(), m_output_jet_particles(),
  m_isValid(true)
{
}

CustomJetAlgorithm::~CustomJetAlgorithm()
{
}

void CustomJetAlgorithm::addParticle(Particle* particle)
{
    switch (particle->pdgc)
    {
    case 11:
    case 13:
    case 15:
    case -11:
    case -13:
    case -15:
        add_lepton(particle);
        break;
    case 12:
    case 14:
    case 16:
    case -12:
    case -14:
    case -16:
        add_neutrino(particle);
        break;
    default:
        break;
    }

    if (abs(particle->pdgc) >= 100)
    {
        add_jet_particle(particle);
    }
}

void CustomJetAlgorithm::add_lepton(Particle* lepton)
{
    if (m_lepton != nullptr)
    {
        // std::cerr << "Found multiple leptons in event" << std::endl;
        // std::cerr << "Already have " << m_lepton->name << std::endl
        //  << "And got a " << lepton->name << std::endl;
        m_isValid = false;
        return;
    }
    m_lepton = lepton;
}

void CustomJetAlgorithm::add_neutrino(Particle* neutrino)
{
    if (m_neutrino != nullptr)
    {
        // std::cerr << "Found multiple neutrinos in event" << std::endl;
        // std::cerr << "Already have " << m_neutrino->name << std::endl
                //   << "And got a " << neutrino->name << std::endl;
        m_isValid = false;
        return;
    }
    m_neutrino = neutrino;
}

void CustomJetAlgorithm::add_jet_particle(Particle* particle)
{
    m_candidate_jet_particles.push_back(particle);
}


double CustomJetAlgorithm::findActualRadius()
{
    if (!m_isValid) {
        return -1;
    }
    if (m_lepton == nullptr)
    {
        std::cerr << "Missing lepton in event" << std::endl;
        return -1;
    }

    if (m_neutrino == nullptr) {
        std::cerr << "Missing neutrino in event" << std::endl;
        return -1;
    }

    if (m_candidate_jet_particles.size() == 0) {
        m_isValid = false;
        std::cerr << "Missing jet particles in event" << std::endl;
        return -1;
    }

    TLorentzVector *expected_jet_vector = new TLorentzVector();
    expected_jet_vector->SetPxPyPzE(
        -m_lepton->momentum->Px(),
        -m_lepton->momentum->Py(),
        m_neutrino->momentum->Pz() - m_lepton->momentum->Pz(),
        m_neutrino->momentum->E() - m_lepton->momentum->E());
    
    // std::cout << "lepton is: " << *m_lepton << std::endl;
    // std::cout << "neutrino is: " << *m_neutrino << std::endl;
    

    double expected_jet_rapidity = expected_jet_vector->PseudoRapidity();
    double expected_jet_phi = expected_jet_vector->Phi();

    // std::cout << "Expected Jet Vector:" << std::endl;
    // std::cout << "Px: " << expected_jet_vector->Px() << std::endl;
    // std::cout << "Py: " << expected_jet_vector->Py() << std::endl;
    // std::cout << "Pz: " << expected_jet_vector->Pz() << std::endl;
    // std::cout << "E: " << expected_jet_vector->E() << std::endl;

    // std::cout << "Jet rapidity: " << expected_jet_rapidity << std::endl
    //           << "Jet phi: " << expected_jet_phi << std::endl;

    double max_distance = 0;
    double totalPt = 0;

    std::vector<std::pair<double, Particle*>> particle_distances;

    for (auto particle : m_candidate_jet_particles)
    {
        totalPt += particle->momentum->Pt();
        double delta_rapidity = particle->momentum->PseudoRapidity() - expected_jet_rapidity;
        double delta_phi = particle->momentum->Phi() - expected_jet_phi;
        double delta_r = sqrt(delta_rapidity * delta_rapidity + delta_phi * delta_phi);
        if (abs(particle->pdgc) < 1e3)
        {
            // meson
            this->m_meson_total_distance += delta_r;
            this->m_meson_total_count++;
        } else if (abs(particle->pdgc) < 1e6)
        {
            // baryon
            this->m_baryon_total_distance += delta_r;
            this->m_baryon_total_count++;
            
        }
        particle_distances.push_back(std::make_pair(delta_r, particle));
    }

    // sort from closest to farthest
    std::sort(particle_distances.begin(), particle_distances.end(), [](std::pair<double, Particle*> a, std::pair<double, Particle*> b) {
        return a.first < b.first;
    });

    double currentPt = 0;
    for (auto particle_distance : particle_distances)
    {
        m_output_jet_particles.push_back(particle_distance.second);
        currentPt += particle_distance.second->momentum->Pt();
        double current_distance = particle_distance.first;
        if (currentPt > totalPt * 0.8)
        {
            return current_distance;
        }
    }

    // should not be possible since the energy should reach 90% before the last particle
    this->m_isValid = false;
    return -1;
}


TLorentzVector CustomJetAlgorithm::getJetVector()
{
    TLorentzVector jet_vector;
    for (auto particle : m_candidate_jet_particles)
    {
        jet_vector += *particle->momentum;
    }
    return jet_vector;
}

TLorentzVector CustomJetAlgorithm::getExpectedJetVector() {
    TLorentzVector expectedJetVector;
    expectedJetVector.SetPxPyPzE(
        -m_lepton->momentum->Px(),
        -m_lepton->momentum->Py(),
        m_neutrino->momentum->Pz() - m_lepton->momentum->Pz(),
        m_neutrino->momentum->E() - m_lepton->momentum->E());
    return expectedJetVector;
}


TVector2 CustomJetAlgorithm::getDeltaPt() {
    
    auto jet = getJetVector();
    auto jetPt = TVector2(jet.Px(), jet.Py());
    auto leptonPt = TVector2(m_lepton->momentum->Px(), m_lepton->momentum->Py());
    return jetPt + leptonPt;
}

double CustomJetAlgorithm::getDeltaAlphaT() {
    auto deltaPt2D = getDeltaPt();
    TVector3 deltaPt(deltaPt2D.X(), deltaPt2D.Y(), 0);
    auto leptonPt = TVector3(m_lepton->momentum->Px(), m_lepton->momentum->Py(), 0);
    return TMath::ACos(-leptonPt.Dot(deltaPt)/(leptonPt.Mag() * deltaPt.Mag())) * 180 / TMath::Pi();
}

double CustomJetAlgorithm::getDeltaPhi() {
    auto jetVector = getJetVector();
    TVector3 jetPt(jetVector.Px(), jetVector.Py(), 0);
    auto leptonPt = TVector3(m_lepton->momentum->Px(), m_lepton->momentum->Py(), 0);
    return TMath::ACos(-leptonPt.Dot(jetPt)/(leptonPt.Mag() * jetPt.Mag())) * 180 / TMath::Pi();

}

std::vector<Particle*> CustomJetAlgorithm::getMissingParticles()
{
    std::vector<Particle*> missing_particles;
    for (auto particle : m_candidate_jet_particles)
    {
        if (std::find(m_output_jet_particles.begin(), m_output_jet_particles.end(), particle) == m_output_jet_particles.end())
        {
            missing_particles.push_back(particle);
        }
    }
    return missing_particles;
}

double CustomJetAlgorithm::getAverageBaryonDistance()
{
    return this->m_baryon_total_distance / this->m_baryon_total_count;
}

double CustomJetAlgorithm::getAverageMesonDistance()
{
    return this->m_meson_total_distance / this->m_meson_total_count;
}

bool CustomJetAlgorithm::isValid()
{
    return m_isValid;
}
