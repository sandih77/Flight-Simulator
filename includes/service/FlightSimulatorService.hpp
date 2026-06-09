#include <string>
#pragma once
using namespace std;
#include "model/Avion.hpp"
#include "model/Piste.hpp"
#include "model/SimulationState.hpp"

class FlightSimulatorService
{
private:
    Avion m_avion;
    Piste m_piste;
    SimulationState m_etat;
    float m_accelerationX = 0.0f;
    float m_accelerationY = 0.0f;

public:
    FlightSimulatorService(Avion avion, Piste piste);

    float getAccelerationX();
    float getAccelerationY();

    void setAccelerationX(float ax);
    void setAccelerationY(float ay);

    void update(float deltaTime);

    void evaluerEtat();

    const Avion &getAvion() const;
    const Piste &getPiste() const;
    SimulationState getEtat() const;
    bool estTermine() const;
};