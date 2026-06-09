#include <QApplication>
#include <iostream>
#include "model/Avion.hpp"
#include "model/Piste.hpp"
#include "service/FlightSimulatorService.hpp"
#include "ui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Avion avion1 = Avion::chargerDepuisFichier("data/avion.txt");
    std::cout << "Avion charge : " << avion1.getNom() << " | Vitesse decrochage: " << avion1.getVitesseDecrochage() << " km/h" << std::endl;

    Piste pistePrincipale("Piste 09R", 1000.0f, 0.0f);

    FlightSimulatorService serviceVol(avion1, pistePrincipale);

    MainWindow mainWindow(serviceVol);
    mainWindow.show();

    return app.exec();
}