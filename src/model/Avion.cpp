#include "../includes/model/Avion.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

Avion::Avion(string nom, float vitesseX, float vitesseY, float altitude,
             float distancePiste, float vitesseDecrochage, float gammaX, float gammaY)
    : m_nom(nom),
      m_vitesseX(vitesseX),
      m_vitesseY(vitesseY),
      m_altitude(altitude),
      m_distancePiste(distancePiste),
      m_vitesseDecrochage(vitesseDecrochage),
      m_gammaX(gammaX),
      m_gammaY(gammaY)
{
}

string Avion::getNom() const
{
    return m_nom;
}

void Avion::setNom(string nom)
{
    m_nom = nom;
}

float Avion::getVitesseX() const
{
    return m_vitesseX;
}

void Avion::setVitesseX(float vitesseX)
{
    m_vitesseX = vitesseX;
}

float Avion::getVitesseY() const
{
    return m_vitesseY;
}

void Avion::setVitesseY(float vitesseY)
{
    m_vitesseY = vitesseY;
}

float Avion::getAltitude() const
{
    return m_altitude;
}

void Avion::setAltitude(float altitude)
{
    m_altitude = altitude;
}

float Avion::getDistancePiste() const
{
    return m_distancePiste;
}

void Avion::setDistancePiste(float distancePiste)
{
    m_distancePiste = distancePiste;
}

float Avion::getVitesseDecrochage() const
{
    return m_vitesseDecrochage;
}

void Avion::setVitesseDecrochage(float vitesseDecrochage)
{
    m_vitesseDecrochage = vitesseDecrochage;
}

float Avion::getGammaX() const
{
    return m_gammaX;
}

void Avion::setGammaX(float gammaX)
{
    m_gammaX = gammaX;
}

float Avion::getGammaY() const
{
    return m_gammaY;
}

void Avion::setGammaY(float gammaY)
{
    m_gammaY = gammaY;
}

Avion Avion::chargerDepuisFichier(const string &cheminFichier)
{
    float vx = 900.0f, vy = 1000.0f, alt = 1000.0f, dist = 70000.0f;
    float decroche = 200.0f, gx = 10.0f, gy = 5.0f;
    string nom = "Boeing 747";

    ifstream fichier(cheminFichier);
    if (!fichier.is_open())
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << cheminFichier << endl;
        return Avion(nom, vx, vy, alt, dist, decroche, gx, gy); 
    }

    string line;
    while (getline(fichier, line))
    {
        stringstream ss(line);
        string key;
        float value;
        if (getline(ss, key, ':'))
        {
            ss >> value;
            if (key == "nom")
                nom = line.substr(line.find(':') + 1);
            else if (key == "vitesseX")
                vx = value;
            else if (key == "vitesseY")
                vy = value;
            else if (key == "altitude")
                alt = value;
            else if (key == "distancePiste")
                dist = value;
            else if (key == "vitesseDecrochage")
                decroche = value;
            else if (key == "gammaX")
                gx = value;
            else if (key == "gammaY")
                gy = value;
        }
    }

    fichier.close();
    std::cout << "Configuration chargee avec succes depuis " << cheminFichier << " !" << std::endl;

    return Avion(nom, vx, vy, alt, dist, decroche, gx, gy);
}