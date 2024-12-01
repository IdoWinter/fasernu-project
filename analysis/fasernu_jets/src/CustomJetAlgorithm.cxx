#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include "Helpers.h"
#include "CustomJetAlgorithm.h"

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
    for (auto particle : m_candidate_jet_particles)
    {
        double delta_rapidity = particle->momentum->PseudoRapidity() - expected_jet_rapidity;
        double delta_phi = particle->momentum->Phi() - expected_jet_phi;
        double delta_r = sqrt(delta_rapidity * delta_rapidity + delta_phi * delta_phi);
        if (delta_r > max_distance)
        {
            max_distance = delta_r;
        }
    }
    return max_distance;
}

void CustomJetAlgorithm::setExpectedRadius(double radius)
{
    m_expected_radius = radius;
}

TLorentzVector CustomJetAlgorithm::getJetVector()
{
    TLorentzVector jet_vector;
    for (auto particle : m_output_jet_particles)
    {
        jet_vector += *particle->momentum;
    }
    return jet_vector;
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

bool CustomJetAlgorithm::isValid()
{
    return m_isValid;
}
