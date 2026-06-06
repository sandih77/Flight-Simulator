#pragma once
#include <string>
using namespace std;

class Avion
{
private:
    string m_nom;
    float m_vitesseX;
    float m_vitesseY;
    float m_altitude;
    float m_distancePiste;
    float m_vitesseDecrochage;
    float m_gammaX;
    float m_gammaY;

public:
    Avion(string nom, float vitesseX, float vitesseY, float altitude, float distancePiste, float vitesseDecrochage, float gammaX, float gammaY);

    string getNom() const;
    void setNom(string nom);
    float getVitesseX() const;
    void setVitesseX(float vitesseX);
    float getVitesseY() const;
    void setVitesseY(float vitesseY);
    float getAltitude() const;
    void setAltitude(float altitude);
    float getDistancePiste() const;
    void setDistancePiste(float distancePiste);
    float getVitesseDecrochage() const;
    void setVitesseDecrochage(float vitesseDecrochage);
    float getGammaX() const;
    void setGammaX(float gammaX);
    float getGammaY() const;
    void setGammaY(float gammaY);

    static Avion chargerDepuisFichier(const string &cheminFichier);
};