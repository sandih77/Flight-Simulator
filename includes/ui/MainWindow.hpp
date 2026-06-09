#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QPaintEvent>
#include "../../includes/service/FlightSimulatorService.hpp" // Ajuste le chemin selon ton projet

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
    // Service du simulateur
    FlightSimulatorService m_service;

    // Timer pour la boucle graphique et physique (60 FPS -> ~16ms)
    QTimer *m_timer;
    const float m_deltaTime = 0.016f;

    // Éléments de l'interface
    QPushButton *m_btnPlusVx;
    QPushButton *m_btnMoinsVx;
    QPushButton *m_btnPlusVy;
    QPushButton *m_btnMoinsVy;
    QPushButton *m_btnPausePlay;

    QLabel *m_labelStats;
    QLabel *m_labelEtat;

    bool m_isPaused = false;

    void mettreAJourLabels();
};
