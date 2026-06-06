#include <iostream>
#include "model/Avion.hpp"
#include <QApplication>
#include "ui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    Avion avion1 = Avion::chargerDepuisFichier("data/avion.txt");
    std::cout << "Avion: " << avion1.getNom() << ", Vitesse: " << avion1.getVitesseDecrochage() << " km/h" << std::endl;
    // QApplication app(argc, argv);
    // MainWindow mainWindow;
    // mainWindow.show();
    return 0;
}