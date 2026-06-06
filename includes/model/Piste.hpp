#pragma once
#include <string>
using namespace std;

class Piste
{
private:
    string m_nom;
    float m_longueur;

public:
    Piste(const string &nom, float longueur);
    string getNom() const;
    float getLongueur() const;
    void setLongueur(float longueur);
    void setNom(const string &nom);
};