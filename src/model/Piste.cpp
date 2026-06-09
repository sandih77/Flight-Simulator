#include "../includes/model/Piste.hpp"

Piste::Piste(const std::string &nom, float longueur, float departX) {
    m_nom = nom;
    m_longueur = longueur;
    m_departX = departX;
}

std::string Piste::getNom() const {
    return m_nom;
}

float Piste::getLongueur() const {
    return m_longueur;
}

float Piste::getDepartX() const {
    return m_departX;
}

void Piste::setDepartX(float departX) {
    m_departX = departX;
}

void Piste::setLongueur(float longueur) {
    m_longueur = longueur;
}

void Piste::setNom(const std::string &nom) {
    m_nom = nom;
}