#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QDebug>
#include <FColorWheel/colorwheel.h>

class Test {
public:
    explicit Test() = default;
    virtual ~Test() = default;
    void do_something() const {qDebug() << "....";}
};

struct WheelHooker {
    QColor operator ()(const std::weak_ptr<QColor>& weak_pt) {
        auto shared_pt = weak_pt.lock();
        qDebug() << *shared_pt;
        this->test.do_something();
        return *shared_pt;
    }

    WheelHooker(){qDebug() << "constructed";}
    const Test test;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    FColorWheel* wheel;
    WheelHooker wheel_hooker;
    Tether<QColor> color_tether;

};

#endif // MAINWINDOW_H
