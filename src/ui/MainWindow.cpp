#include "../includes/ui/MainWindow.hpp"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->resize(800,600);
    this->setWindowTitle("Flight Simulator");
}