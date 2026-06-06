#include "../includes/model/Piste.hpp"

Piste::Piste(const std::string &nom, float longueur) {
    m_nom = nom;
    m_longueur = longueur;
}

std::string Piste::getNom() const {
    return m_nom;
}

float Piste::getLongueur() const {
    return m_longueur;
}

void Piste::setLongueur(float longueur) {
    m_longueur = longueur;
}

void Piste::setNom(const std::string &nom) {
    m_nom = nom;
}