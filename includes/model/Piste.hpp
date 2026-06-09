#pragma once
#include <string>
using namespace std;

class Piste
{
private:
    string m_nom;
    float m_departX;
    float m_longueur;

public:
    Piste(const string &nom, float longueur, float departX);
    string getNom() const;
    float getLongueur() const;
    float getDepartX() const;
    void setLongueur(float longueur);
    void setNom(const string &nom);
    void setDepartX(float departX);
};