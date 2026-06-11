#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QPaintEvent>
#include "../../includes/service/FlightSimulatorService.hpp" 

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(const FlightSimulatorService &service, QWidget *parent = nullptr);
    ~MainWindow() override = default;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void loopSimulation();

private:
    FlightSimulatorService m_service;

    QTimer *m_timer;
    const float m_deltaTime = 0.016f;

    QPushButton *m_btnPlusVx;
    QPushButton *m_btnMoinsVx;
    QPushButton *m_btnPlusVy;
    QPushButton *m_btnMoinsVy;
    QPushButton *m_btnPausePlay;

    QPushButton *m_btnScreenShot;

    QLabel *m_labelStats;
    QLabel *m_labelEtat;

    bool m_isPaused = false;

    void mettreAJourLabels();
    void ecrireDansFichier();
    enum class VueCamera
    {
        DROITE,
        GAUCHE,
        DERRIERE
    };

    VueCamera m_vue = VueCamera::DROITE;

    QPushButton *m_btnVueDroite = nullptr;
    QPushButton *m_btnVueGauche = nullptr;
    QPushButton *m_btnVueDerriere = nullptr;
};
