#include "../includes/ui/MainWindow.hpp"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <cmath>

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
            { m_service.setAccelerationX(m_service.getAccelerationX() + 5.0f); });

    connect(m_btnMoinsVx, &QPushButton::clicked, [this]()
            { m_service.setAccelerationX(m_service.getAccelerationX() - 5.0f); });

    connect(m_btnPlusVy, &QPushButton::clicked, [this]()
            { m_service.setAccelerationY(m_service.getAccelerationY() + 1.0f); });

    connect(m_btnMoinsVy, &QPushButton::clicked, [this]()
            { m_service.setAccelerationY(m_service.getAccelerationY() - 1.0f); });

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
    {
        m_timer->stop();
    }
}

void MainWindow::mettreAJourLabels()
{
    const Avion &avion = m_service.getAvion();

    QString stats =
        QString("Vx: %1 km/h | Vy: %2 m/s\n"
                "Ax: %3 | Ay: %4\n"
                "Altitude: %5 m | Distance: %6 m")
            .arg(avion.getVitesseX())
            .arg(avion.getVitesseY())
            .arg(m_service.getAccelerationX())
            .arg(m_service.getAccelerationY())
            .arg(avion.getAltitude())
            .arg(avion.getDistancePiste());
    m_labelStats->setText(stats);

    QString etatStr = "État : ";
    switch (m_service.getEtat())
    {
    case SimulationState::EN_VOL:
        etatStr += "<font color='blue'>EN VOL</font>";
        break;
    case SimulationState::SUR_PISTE:
        etatStr += "<font color='black'>SUR PISTE</font>";
        break;
    case SimulationState::DECROCHAGE:
        etatStr += "<font color='red'>CRASH (vitesse inférieur à la vitesse de décrochage)</font>";
        break;
    case SimulationState::CRASH:
        etatStr += "<font color='red'>CRASH !!</font>";
        break;
    case SimulationState::ATTERI:
        etatStr += "<font color='black'>ATTERRISSAGE RÉUSSI !</font>";
        break;
    }
    m_labelEtat->setText(etatStr);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Active l'anti-crénelage pour des dessins fluides
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. DÉCOR (Ciel et Sol)
    int hauteurSol = this->height() * 0.75;

    // Dessin du Ciel (Bleu ciel)
    painter.fillRect(0, 0, this->width(), hauteurSol, QColor(135, 206, 235));

    // Dessin du Sol (Vert gazon)
    painter.fillRect(0, hauteurSol, this->width(), this->height() - hauteurSol, QColor(34, 139, 34));

    // Récupération des données réelles de la simulation
    const Avion &avion = m_service.getAvion();
    const Piste &piste = m_service.getPiste();

    // 2. COORDONNÉES DE L'AVION (Fixe sur l'axe X, dynamique sur l'axe Y)
    int avionX = this->width() * 0.20; // Ancré horizontalement à 20% de l'écran

    // Échelle Altitude : 1 mètre physique = 0.3 pixel à l'écran
    float echelleAltitude = 0.3f;
    int avionY = hauteurSol - 20 - (avion.getAltitude() * echelleAltitude);

    // Sécurités visuelles pour empêcher l'avion de sortir des limites de l'écran
    if (avionY > hauteurSol - 20)
    {
        avionY = hauteurSol - 20; // Bloqué au sol
    }
    if (avionY < 20)
    {
        avionY = 20; // Bloqué au plafond
    }

    // 3. DESSIN SYNCHRONISÉ ET COHÉRENT DE LA PISTE
    // Échelle Horizontale : 1 mètre physique = 1.5 pixel à l'écran
    float echelleX = 1.5f;

    // CALCUL CRUCIAL : Alignement visuel sur les calculs du Service
    // - Quand distancePiste == longueurPiste (ex: 600), positionVisuellePisteX vaut exactement avionX (le début touche l'avion).
    // - Plus distancePiste diminue, plus la piste glisse vers la gauche.
    // - Quand distancePiste == 0, le bout de la piste arrive sous l'avion.
    int positionVisuellePisteX = avionX - ((piste.getLongueur() - avion.getDistancePiste()) * echelleX);
    int largeurVisuellePiste = piste.getLongueur() * echelleX;

    // On ne dessine la piste que si elle est visible dans la fenêtre
    if (positionVisuellePisteX + largeurVisuellePiste > 0 && positionVisuellePisteX < this->width())
    {
        // Dessin de la bande de bitume (Gris foncé)
        painter.fillRect(positionVisuellePisteX, hauteurSol - 4, largeurVisuellePiste, 8, QColor(80, 80, 80));

        // Dessin des pointillés centraux (Blanc)
        painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
        painter.drawLine(positionVisuellePisteX, hauteurSol, positionVisuellePisteX + largeurVisuellePiste, hauteurSol);
    }

    // 4. DESSIN DE LA SILHOUETTE DE L'AVION
    // Sélection de la couleur en fonction de l'état de la simulation
    QColor couleurAvion = Qt::darkBlue;
    if (m_service.getEtat() == SimulationState::CRASH)
    {
        couleurAvion = Qt::red; // Rouge si crash
    }
    else if (m_service.getEtat() == SimulationState::DECROCHAGE)
    {
        couleurAvion = Qt::red; // Rouge si décrochage
    }
    else if (m_service.getEtat() == SimulationState::ATTERI)
    {
        couleurAvion = Qt::darkGreen; // Vert si posé avec succès !
    }

    // Outils de dessin pour l'avion
    painter.setBrush(couleurAvion);
    painter.setPen(Qt::NoPen);

    // Forme de l'appareil
    painter.drawEllipse(avionX, avionY, 45, 12);                    // Fuselage (Corps)
    painter.fillRect(avionX + 18, avionY - 8, 8, 16, couleurAvion); // Aile principale
    painter.fillRect(avionX + 2, avionY - 6, 4, 8, couleurAvion);   // Gouvernail de queue
}