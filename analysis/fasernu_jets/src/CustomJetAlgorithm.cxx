#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include "Helpers.h"

CustomJetAlgorithm::CustomJetAlgorithm()
{
}

CustomJetAlgorithm::~CustomJetAlgorithm()
{
}

void CustomJetAlgorithm::add_particle(Particle* particle)
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

    if (particle->pdgc > 100 || particle->pdgc < -100)
    {
        add_jet_particle(particle);
    }
}

void CustomJetAlgorithm::add_lepton(Particle* lepton)
{
    if (m_lepton != nullptr)
    {
        std::cerr << "Found multiple leptons in event" << std::endl;
        return;
    }
    m_lepton = lepton;
}

void CustomJetAlgorithm::add_neutrino(Particle* neutrino)
{
    if (m_neutrino != nullptr)
    {
        std::cerr << "Found multiple neutrinos in event" << std::endl;
        return;
    }
    m_neutrino = neutrino;
}

void CustomJetAlgorithm::add_jet_particle(Particle* particle)
{
    m_candidate_jet_particles.push_back(particle);
}

void CustomJetAlgorithm::process()
{
    if (m_lepton == nullptr || m_neutrino == nullptr || m_candidate_jet_particles.size() == 0)
    {
        std::cerr << "Missing particles in event" << std::endl;
        return;
    }
    TLorentzVector *expected_jet_vector = new TLorentzVector();
    expected_jet_vector->SetPxPyPzE(-m_lepton->momentum->Px(), -m_lepton->momentum->Py(), m_neutrino->momentum->Pz() - m_lepton->momentum->Pz(), m_neutrino->momentum->E() - m_lepton->momentum->E());

    double expected_jet_rapidity = expected_jet_vector->Rapidity();
    double expected_jet_phi = expected_jet_vector->Phi();

    for (auto particle : m_candidate_jet_particles)
    {
        double delta_rapidity = particle->momentum->Rapidity() - expected_jet_rapidity;
        double delta_phi = particle->momentum->Phi() - expected_jet_phi;
        double delta_r = sqrt(delta_rapidity * delta_rapidity + delta_phi * delta_phi);
        if (delta_r < m_expected_radius)
        {
            m_output_jet_particles.push_back(particle);
        }
    }
}

void CustomJetAlgorithm::set_expected_radius(double radius)
{
    m_expected_radius = radius;
}

TLorentzVector CustomJetAlgorithm::get_jet_vector()
{
    TLorentzVector jet_vector;
    for (auto particle : m_output_jet_particles)
    {
        jet_vector += *particle->momentum;
    }
    return jet_vector;
}

std::vector<Particle*> CustomJetAlgorithm::get_missing_particles()
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
