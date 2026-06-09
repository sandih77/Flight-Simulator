#include "../includes/service/FlightSimulatorService.hpp"

FlightSimulatorService::FlightSimulatorService(Avion avion, Piste piste)
    : m_avion(avion), m_piste(piste),
      m_etat(SimulationState::EN_VOL)
{
}
static float clamp(float v, float min, float max)
{
    return (v < min) ? min : (v > max) ? max
                                       : v;
}
void FlightSimulatorService::setAccelerationX(float ax)
{
    float gammaX = m_avion.getGammaX();

    m_accelerationX = clamp(ax, -gammaX, gammaX);
}

float FlightSimulatorService::getAccelerationX()
{
    return m_accelerationX;
}

float FlightSimulatorService::getAccelerationY()
{
    return m_accelerationY;
}

void FlightSimulatorService::setAccelerationY(float ay)
{
    float gammaY = m_avion.getGammaY();

    m_accelerationY = clamp(ay, -gammaY, gammaY);
}

void FlightSimulatorService::evaluerEtat()
{
    const float distancePiste = m_avion.getDistancePiste();
    const float altitude = m_avion.getAltitude();
    const float vx = m_avion.getVitesseX();
    const float vy = m_avion.getVitesseY();
    const float longueurPiste = m_piste.getLongueur();

    bool surPiste = (distancePiste >= 0.0f) && (distancePiste <= longueurPiste);

    if (altitude > 0)
    {
        if (vx < m_avion.getVitesseDecrochage())
        {
            m_etat = SimulationState::DECROCHAGE;
            return;
        }
        m_etat = SimulationState::EN_VOL;
        return;
    }

    if (altitude <= 0)
    {
        if (!surPiste)
        {
            m_etat = SimulationState::CRASH;
            return;
        }

        if (vy > 0.0f && vx > m_avion.getVitesseDecrochage())
        {
            m_etat = SimulationState::EN_VOL;
            return;
        }

        if (vx > 5.0f)
        {
            m_etat = SimulationState::SUR_PISTE;
        }
        else
        {
            m_etat = SimulationState::ATTERI;
        }
    }
}

const Avion &FlightSimulatorService::getAvion() const
{
    return m_avion;
}

const Piste &FlightSimulatorService::getPiste() const
{
    return m_piste;
}

SimulationState FlightSimulatorService::getEtat() const
{
    return m_etat;
}

bool FlightSimulatorService::estTermine() const
{
    return m_etat == SimulationState::CRASH || m_etat == SimulationState::DECROCHAGE || m_etat == SimulationState::ATTERI;
}

void FlightSimulatorService::update(float deltaTime)
{
    if (estTermine())
        return;

    m_avion.setVitesseX(m_avion.getVitesseX() + m_accelerationX * deltaTime);
    m_avion.setVitesseY(m_avion.getVitesseY() + m_accelerationY * deltaTime);

    float deplacementX = (m_avion.getVitesseX() / 3.6f) * deltaTime;
    float deplacementY = m_avion.getVitesseY() * deltaTime;

    bool surPiste = m_etat == SimulationState::SUR_PISTE;
    float nouvelleAltitude = m_avion.getAltitude() + deplacementY;

    if (surPiste && m_avion.getVitesseX() <= 0)
        nouvelleAltitude = 0.0f;

    m_avion.setDistancePiste(m_avion.getDistancePiste() - deplacementX);
    m_avion.setAltitude(nouvelleAltitude);

    evaluerEtat();
}