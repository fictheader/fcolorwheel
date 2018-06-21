#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), wheel(new FColorWheel(this, 60, 60, 50, 10))
{
    wheel->move(20, 20);
    this->resize(160, 160);
    this->setStyleSheet("background : gray");

    color_tether.make_tether(wheel->colorChangedSig);
    color_tether.make_hooker(wheel_hooker);
}

MainWindow::~MainWindow()
{
    delete wheel;
}
