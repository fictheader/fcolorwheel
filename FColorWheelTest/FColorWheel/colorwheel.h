#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>
#include <QConicalGradient>
#include <QPen>
#include <QtMath>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QScopedPointer>
#include <../tether/tether.h>


class FColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit FColorWheel(QWidget *parent = nullptr, const int center_x = 0, const int center_y = 0, const int base_radius = 0, const int wheel_width = 0);
    virtual ~FColorWheel() = default;
    FColorWheel(FColorWheel&) = delete;

    virtual const QColor getBaseColor();
    virtual const QColor getColor();

private:
    virtual QConicalGradient makeWheelGradColor(const int center_x, const int center_y);
    virtual QPen makeWheelPen(const int wheel_width, const int center_x, const int center_y);
    virtual void drawGradFColorWheel(const int wheel_width, const int center_x, const int center_y, const int base_radius);
    virtual void drawGradFColorWheelCursor(const int wheel_width, const qreal wheel_cursor_x, const qreal wheel_cursor_y);
    virtual void drawGradColorSquare(const QColor& current_base_color, const int center_x, const int center_y, const qreal top_left_x, const qreal top_left_y, const qreal sq_length);
    virtual void drawGradColorSquareCursor(const int wheel_width, const qreal sq_cursor_x, const qreal sq_cursor_y);

    virtual int clampCursorPosX(const int x, const qreal top_left_x, const qreal sq_length);
    virtual int clampCursorPosY(const int y, const qreal top_left_y, const qreal sq_length);

    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

public:
    TetherSig<QColor> colorChangedSig;

private:
    const int center_x;
    const int center_y;
    const int base_radius;
    const int wheel_width;
    const qreal sq_length;
    const qreal top_left_x;
    const qreal top_left_y;
    bool is_in_wheel;
    bool is_in_sq;
    qreal wheel_cursor_x;
    qreal wheel_cursor_y;
    qreal sq_cursor_x;
    qreal sq_cursor_y;
    QColor current_base_color;
    QColor current_color;
};


#endif // COLORWHEEL_H
