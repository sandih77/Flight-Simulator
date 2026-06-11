#include "../includes/ui/MainWindow.hpp"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

MainWindow::MainWindow(const FlightSimulatorService &service, QWidget *parent)
    : QWidget(parent),
      m_service(service)
{
    this->resize(800, 600);
    this->setWindowTitle("Flight Simulator");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();

    QHBoxLayout *topInfoLayout = new QHBoxLayout();
    m_labelStats = new QLabel(this);
    m_labelEtat = new QLabel(this);

    QFont font("Arial", 11, QFont::Bold);
    m_labelStats->setFont(font);
    m_labelEtat->setFont(font);

    topInfoLayout->addWidget(m_labelStats);
    topInfoLayout->addStretch();
    topInfoLayout->addWidget(m_labelEtat);
    mainLayout->addLayout(topInfoLayout);

    QHBoxLayout *viewLayout = new QHBoxLayout();
    m_btnVueDroite = new QPushButton("Vue droite", this);
    m_btnVueGauche = new QPushButton("Vue gauche", this);
    m_btnVueDerriere = new QPushButton("Vue derrière", this);
    m_btnScreenShot = new QPushButton("Screenshot", this);

    m_btnVueDroite->setCheckable(true);
    m_btnVueGauche->setCheckable(true);
    m_btnVueDerriere->setCheckable(true);
    m_btnVueDroite->setChecked(true);

    viewLayout->addWidget(m_btnVueDroite);
    viewLayout->addWidget(m_btnVueGauche);
    viewLayout->addWidget(m_btnVueDerriere);
    viewLayout->addWidget(m_btnScreenShot);
    mainLayout->addLayout(viewLayout);

    connect(m_btnVueDroite, &QPushButton::clicked, [this]()
            {
        m_vue = VueCamera::DROITE;
        m_btnVueDroite->setChecked(true);
        m_btnVueGauche->setChecked(false);
        m_btnVueDerriere->setChecked(false);
        this->update(); });
    connect(m_btnVueGauche, &QPushButton::clicked, [this]()
            {
        m_vue = VueCamera::GAUCHE;
        m_btnVueDroite->setChecked(false);
        m_btnVueGauche->setChecked(true);
        m_btnVueDerriere->setChecked(false);
        this->update(); });
    connect(m_btnVueDerriere, &QPushButton::clicked, [this]()
            {
        m_vue = VueCamera::DERRIERE;
        m_btnVueDroite->setChecked(false);
        m_btnVueGauche->setChecked(false);
        m_btnVueDerriere->setChecked(true);
        this->update(); });

    connect(m_btnScreenShot, &QPushButton::clicked, [this]() {
        ecrireDansFichier();
    });

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    m_btnPlusVx = new QPushButton("Vitesse X +", this);
    m_btnMoinsVx = new QPushButton("Vitesse X -", this);
    m_btnPlusVy = new QPushButton("Vitesse Y +", this);
    m_btnMoinsVy = new QPushButton("Vitesse Y -", this);

    buttonsLayout->addWidget(m_btnMoinsVx);
    buttonsLayout->addWidget(m_btnPlusVx);
    buttonsLayout->addWidget(m_btnMoinsVy);
    buttonsLayout->addWidget(m_btnPlusVy);

    m_btnPausePlay = new QPushButton("Pause", this);
    buttonsLayout->addWidget(m_btnPausePlay);

    connect(m_btnPausePlay, &QPushButton::clicked, [this]()
            {
        m_isPaused = !m_isPaused;
        if (m_isPaused) {
            m_timer->stop();
            m_btnPausePlay->setText("Play");
        } else {
            m_timer->start(16);
            m_btnPausePlay->setText("Pause");
        } });

    mainLayout->addLayout(buttonsLayout);

    connect(m_btnPlusVx, &QPushButton::clicked, [this]()
            { m_service.setAccelerationX(m_service.getAccelerationX() + 10.0f); });
    connect(m_btnMoinsVx, &QPushButton::clicked, [this]()
            { m_service.setAccelerationX(m_service.getAccelerationX() - 10.0f); });
    connect(m_btnPlusVy, &QPushButton::clicked, [this]()
            { m_service.setAccelerationY(m_service.getAccelerationY() + 2.0f); });
    connect(m_btnMoinsVy, &QPushButton::clicked, [this]()
            { m_service.setAccelerationY(m_service.getAccelerationY() - 2.0f); });

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::loopSimulation);
    m_timer->start(16);
}

void MainWindow::loopSimulation()
{
    m_service.update(m_deltaTime);
    mettreAJourLabels();
    this->update();

    if (m_service.estTermine())
        m_timer->stop();
}

void MainWindow::ecrireDansFichier()
{
    ofstream fichier("data/result.txt", std::ios::app);

    if (fichier.is_open())
    {
        const Avion &avion = m_service.getAvion();

        fichier
            << m_service.getTempsEcoule() << ":TempsEcoulé| "
            << avion.getVitesseX() << ":VX| "
            << avion.getVitesseY() << ":VY| "
            << avion.getAltitude() << ":Altitude| "
            << avion.getDistancePiste() << ":DistancePiste| ";

        switch (m_service.getEtat())
        {
        case SimulationState::EN_VOL:
            fichier << "EN_VOL";
            break;
        case SimulationState::SUR_PISTE:
            fichier << "SUR_PISTE";
            break;
        case SimulationState::DECROCHAGE:
            fichier << "DECROCHAGE";
            break;
        case SimulationState::CRASH:
            fichier << "CRASH";
            break;
        case SimulationState::ATTERI:
            fichier << "ATTERI";
            break;
        }

        fichier << "\n";
        fichier.close();
    }
}

void MainWindow::mettreAJourLabels()
{
    const Avion &avion = m_service.getAvion();

    float tempsTotalSec = m_service.getTempsEcoule();

    int minutes = static_cast<int>(tempsTotalSec) / 60;
    int secondes = static_cast<int>(tempsTotalSec) % 60;
    int millisecondes = static_cast<int>((tempsTotalSec - static_cast<int>(tempsTotalSec)) * 1000);

    QString chronoStr = QString("%1:%2.%3")
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(secondes, 2, 10, QChar('0'))
                            .arg(millisecondes, 3, 10, QChar('0'));

    QString stats =
        QString("[ CHRONO : %1 ]\n"
                "Vx: %2 km/h | Vy: %3 m/s\n"
                "Ax: %4 | Ay: %5\n"
                "Altitude: %6 m | Distance: %7 m")
            .arg(chronoStr)
            .arg(avion.getVitesseX(), 0, 'f', 1)
            .arg(avion.getVitesseY(), 0, 'f', 1)
            .arg(m_service.getAccelerationX(), 0, 'f', 1)
            .arg(m_service.getAccelerationY(), 0, 'f', 1)
            .arg(avion.getAltitude(), 0, 'f', 1)
            .arg(avion.getDistancePiste(), 0, 'f', 1);

    m_labelStats->setText(stats);

    QString etatStr = "État : ";
    switch (m_service.getEtat())
    {
    case SimulationState::EN_VOL:
        etatStr += "<font color='blue'><b>EN VOL</b></font>";
        break;
    case SimulationState::SUR_PISTE:
        etatStr += "<font color='orange'><b>SUR PISTE</b></font>";
        break;
    case SimulationState::DECROCHAGE:
        etatStr += "<font color='red'><b>CRASH (Décrochage)</b></font>";
        break;
    case SimulationState::CRASH:
        etatStr += "<font color='red'><b>CRASH !!</b></font>";
        break;
    case SimulationState::ATTERI:
        etatStr += QString("<font color='green'><b>ATTERRISSAGE RÉUSSI en %1 !</b></font>").arg(chronoStr);
        break;
    }
    m_labelEtat->setText(etatStr);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int hauteurSol = this->height() * 0.75;
    painter.fillRect(0, 0, this->width(), hauteurSol, QColor(135, 206, 235));                         // Ciel
    painter.fillRect(0, hauteurSol, this->width(), this->height() - hauteurSol, QColor(34, 139, 34)); // Sol

    const Avion &avion = m_service.getAvion();
    const Piste &piste = m_service.getPiste();

    QColor couleurAvion = Qt::darkBlue;
    if (m_service.getEtat() == SimulationState::CRASH || m_service.getEtat() == SimulationState::DECROCHAGE)
        couleurAvion = Qt::red;
    else if (m_service.getEtat() == SimulationState::ATTERI)
        couleurAvion = Qt::darkGreen;

    float echelleAltitude = 0.3f;
    int avionY = hauteurSol - 20 - (avion.getAltitude() * echelleAltitude);
    if (avionY > hauteurSol - 20)
        avionY = hauteurSol - 20;
    if (avionY < 20)
        avionY = 20;

    switch (m_vue)
    {
    case VueCamera::DROITE:
    {
        int avionX = this->width() * 0.20;
        float echelleX = 1.0f;
        float dist = avion.getDistancePiste();
        float longPiste = piste.getLongueur();

        int positionVisuellePisteX;
        if (dist < 0.0f)
        {
            positionVisuellePisteX = avionX + (abs(dist) * echelleX);
        }
        else
        {
            positionVisuellePisteX = avionX - (dist * echelleX);
        }

        int largeurVisuellePiste = longPiste * echelleX;

        if (positionVisuellePisteX + largeurVisuellePiste > 0 && positionVisuellePisteX < this->width())
        {
            painter.fillRect(positionVisuellePisteX, hauteurSol - 5, largeurVisuellePiste, 10, QColor(80, 80, 80));
            painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
            painter.drawLine(positionVisuellePisteX, hauteurSol, positionVisuellePisteX + largeurVisuellePiste, hauteurSol);
            painter.setPen(Qt::NoPen);
        }

        painter.setBrush(couleurAvion);
        painter.drawEllipse(avionX, avionY, 45, 12);
        painter.fillRect(avionX + 18, avionY - 8, 8, 16, couleurAvion);
        painter.fillRect(avionX + 2, avionY - 6, 4, 8, couleurAvion);
        break;
    }

    case VueCamera::GAUCHE:
    {
        int avionX = this->width() * 0.80 - 45;
        float echelleX = 1.0f;
        float dist = avion.getDistancePiste();
        float longPiste = piste.getLongueur();

        int positionVisuellePisteX;
        if (dist < 0.0f)
        {
            positionVisuellePisteX = avionX - (abs(dist) * echelleX) - longPiste;
        }
        else
        {
            positionVisuellePisteX = avionX + (dist * echelleX) - longPiste;
        }

        int largeurVisuellePiste = longPiste * echelleX;

        if (positionVisuellePisteX + largeurVisuellePiste > 0 && positionVisuellePisteX < this->width())
        {
            painter.fillRect(positionVisuellePisteX, hauteurSol - 5, largeurVisuellePiste, 10, QColor(80, 80, 80));
            painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
            painter.drawLine(positionVisuellePisteX, hauteurSol, positionVisuellePisteX + largeurVisuellePiste, hauteurSol);
            painter.setPen(Qt::NoPen);
        }

        painter.setBrush(couleurAvion);
        painter.drawEllipse(avionX, avionY, 45, 12);
        painter.fillRect(avionX + 19, avionY - 8, 8, 16, couleurAvion);
        painter.fillRect(avionX + 39, avionY - 6, 4, 8, couleurAvion);
        break;
    }

    case VueCamera::DERRIERE:
    {
        int centreX = this->width() / 2;
        float dist = avion.getDistancePiste();
        float longPiste = piste.getLongueur();

        int largeurPisteProche = 120;
        int largeurPisteLoin = 20;
        int yLoin = hauteurSol - 10;
        int yProche = hauteurSol;

        if (dist >= 0.0f && dist <= longPiste + 200.0f)
        {
            QPolygon trapeze;
            trapeze << QPoint(centreX - largeurPisteLoin / 2, yLoin)
                    << QPoint(centreX + largeurPisteLoin / 2, yLoin)
                    << QPoint(centreX + largeurPisteProche / 2, yProche)
                    << QPoint(centreX - largeurPisteProche / 2, yProche);

            painter.setBrush(QColor(80, 80, 80));
            painter.setPen(Qt::NoPen);
            painter.drawPolygon(trapeze);

            painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
            painter.drawLine(centreX, yLoin, centreX, yProche);
            painter.setPen(Qt::NoPen);
        }

        painter.setBrush(couleurAvion);
        int fuselageRayon = 14;
        int avionX = centreX - (fuselageRayon / 2);
        int avionYCentre = avionY + 2;

        int envergureAiles = 56;
        int epaisseurAiles = 4;
        painter.fillRect(centreX - (envergureAiles / 2), avionYCentre + 4, envergureAiles, epaisseurAiles, couleurAvion);
        painter.drawEllipse(avionX, avionYCentre, fuselageRayon, fuselageRayon);
        painter.fillRect(centreX - 2, avionYCentre - 10, 4, 10, couleurAvion);
        break;
    }
    }

    painter.setPen(QColor(0, 0, 0, 160));
    QFont vueFont("Arial", 9, QFont::Bold);
    painter.setFont(vueFont);
    QString labelVue;
    switch (m_vue)
    {
    case VueCamera::DROITE:
        labelVue = "◀ VUE DROITE ACTIVE";
        break;
    case VueCamera::GAUCHE:
        labelVue = "VUE GAUCHE ACTIVE ▶";
        break;
    case VueCamera::DERRIERE:
        labelVue = "▲ VUE DERRIÈRE ACTIVE";
        break;
    }
    painter.drawText(this->width() / 2 - 65, 22, labelVue);
}