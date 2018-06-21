#include "colorwheel.h"
#include <QDebug>


FColorWheel::FColorWheel(QWidget *parent, const int center_x, const int center_y, const int base_radius, const int wheel_width) : QWidget(parent),
    center_x(center_x), center_y(center_y),
    base_radius(base_radius), wheel_width(wheel_width),
    sq_length(1.414 * (base_radius - wheel_width / 2) - 0.63),
    top_left_x(center_x - sq_length / 2), top_left_y(center_y - sq_length / 2),
    is_in_wheel(false), is_in_sq(false),
    wheel_cursor_x(base_radius + center_x), wheel_cursor_y(center_y),
    sq_cursor_x(top_left_x), sq_cursor_y(top_left_y)
{
    this->resize(center_x * 2, center_y * 2);
    current_base_color = QColor::fromRgb(grab(QRect(QPoint(wheel_cursor_x, wheel_cursor_y), QSize(1, 1))).toImage().pixel(0, 0));
    current_color = QColor::fromRgb(grab(QRect(QPoint(sq_cursor_x, sq_cursor_y), QSize(1, 1))).toImage().pixel(0, 0));
}


const QColor FColorWheel::getBaseColor()
{
    return this->current_base_color;
}


const QColor FColorWheel::getColor()
{
    return this->current_color;
}


QConicalGradient FColorWheel::makeWheelGradColor(const int center_x, const int center_y)
{
    QConicalGradient ring_gradient(center_x, center_y, 60);
    ring_gradient.setColorAt(0.0, Qt::yellow);
    ring_gradient.setColorAt(1.0 / 6.0, Qt::red);
    ring_gradient.setColorAt(1.0 / 6.0 * 2.0, Qt::magenta);
    ring_gradient.setColorAt(1.0 / 6.0 * 3.0, Qt::blue);
    ring_gradient.setColorAt(1.0 / 6.0 * 4.0, Qt::cyan);
    ring_gradient.setColorAt(1.0 / 6.0 * 5.0, Qt::green);
    ring_gradient.setColorAt(1.0, Qt::yellow);
    return ring_gradient;
}


QPen FColorWheel::makeWheelPen(const int wheel_width, const int center_x, const int center_y)
{
    QBrush wheel_brush(makeWheelGradColor(center_x, center_y));
    QPen wheel_pen(wheel_brush, wheel_width, Qt::SolidLine, Qt::FlatCap);
    return wheel_pen;
}


void FColorWheel::drawGradFColorWheel(const int wheel_width, const int center_x, const int center_y, const int base_radius)
{
    QPainter wheel_painter(this);
    wheel_painter.setPen(makeWheelPen(wheel_width, center_x, center_x));
    wheel_painter.setRenderHint(QPainter::Antialiasing);
    wheel_painter.drawEllipse(QPoint(center_x, center_y), base_radius, base_radius);
}


void FColorWheel::drawGradFColorWheelCursor(const int wheel_width, const qreal wheel_cursor_x, const qreal wheel_cursor_y)
{
    QPainter wheel_cursor(this);
    QPen wheel_cursor_pen(QBrush(Qt::black), wheel_width/16.);
    wheel_cursor.setPen(wheel_cursor_pen);
    wheel_cursor.setRenderHint(QPainter::Antialiasing);
    wheel_cursor.drawEllipse(QPointF(wheel_cursor_x, wheel_cursor_y), wheel_width/2., wheel_width/2);
}


void FColorWheel::drawGradColorSquare(const QColor& current_base_color, const int center_x, const int center_y, const qreal top_left_x, const qreal top_left_y, const qreal sq_length)
{
    QLinearGradient grad_to_white(top_left_x, center_y, top_left_x + sq_length, center_y);
    grad_to_white.setColorAt(0.0, current_base_color);
    grad_to_white.setColorAt(0.9, QColor(255, 255, 255, 255));
    grad_to_white.setColorAt(1.0, QColor(255, 255, 255, 255));

    QLinearGradient grad_to_black(center_x, top_left_y, center_x, top_left_y + sq_length);
    grad_to_black.setColorAt(0.0, QColor(0, 0, 0, 0));
    grad_to_black.setColorAt(0.1, QColor(0, 0, 0, 0));
    grad_to_black.setColorAt(0.5, QColor(0, 0, 0, 100));
    grad_to_black.setColorAt(0.9, QColor(0, 0, 0, 255));
    grad_to_black.setColorAt(1.0, QColor(0, 0, 0, 255));

    QBrush grad_white_brush(grad_to_white);
    QBrush grad_black_brush(grad_to_black);

    QPainter sq_painter(this);
    sq_painter.setPen(Qt::NoPen);

    sq_painter.setCompositionMode(QPainter::CompositionMode_Source);
    sq_painter.setBrush(grad_white_brush);
    sq_painter.drawRect(top_left_x, top_left_y, sq_length, sq_length);

    sq_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    sq_painter.setBrush(grad_black_brush);
    sq_painter.drawRect(top_left_x, top_left_y, sq_length, sq_length);

}


void FColorWheel::drawGradColorSquareCursor(const int wheel_width, const qreal sq_cursor_x, const qreal sq_cursor_y)
{
    QPainter sq_cursor(this);
    QPen sq_cursor_pen(QBrush(Qt::black), wheel_width/16.);
    sq_cursor.setPen(sq_cursor_pen);
    sq_cursor.setRenderHint(QPainter::Antialiasing);
    sq_cursor.drawEllipse(QPointF(sq_cursor_x, sq_cursor_y), wheel_width/2., wheel_width/2);
}


int FColorWheel::clampCursorPosX(const int x, const qreal top_left_x, const qreal sq_length)
{
   if(x < top_left_x){
       return top_left_x;
   }else if(x > top_left_x + sq_length){
       return top_left_x + sq_length - 1.7; //padding
   }else{
       return x;
   }
}


int FColorWheel::clampCursorPosY(const int y, const qreal top_left_y, const qreal sq_length)
{
   if(y < top_left_x){
       return top_left_y;
   }else if(y > top_left_y + sq_length){
       return top_left_y + sq_length - 1.7; //padding
   }else{
       return y;
   }
}


void FColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    drawGradFColorWheel(wheel_width, center_x, center_y, base_radius);
    drawGradFColorWheelCursor(wheel_width, wheel_cursor_x, wheel_cursor_y);
    drawGradColorSquare(current_base_color, center_x, center_y, top_left_x, top_left_y, sq_length);
    drawGradColorSquareCursor(wheel_width, sq_cursor_x, sq_cursor_y);

}


void FColorWheel::mousePressEvent(QMouseEvent *event)
{
    const int outer_r = base_radius + wheel_width / 2;
    const int inner_r = base_radius - wheel_width / 2;
    const int x = event->pos().x() - center_x;
    const int y = event->pos().y() - center_y;

    if((event->button() == Qt::LeftButton) && (top_left_x < event->pos().x() && event->pos().x() < top_left_x + sq_length) && (top_left_y < event->pos().y() && event->pos().y() < top_left_y + sq_length)){
        is_in_sq = true;
        sq_cursor_x = clampCursorPosX(event->pos().x(), top_left_x, sq_length);
        sq_cursor_y = clampCursorPosX(event->pos().y(), top_left_y, sq_length);
        current_color = QColor::fromRgb(grab(QRect(QPoint(sq_cursor_x, sq_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
    }

    if((event->button() == Qt::LeftButton) && (x * x + y * y < outer_r*outer_r) && (inner_r*inner_r < x * x + y * y)){
        is_in_wheel = true;
        const qreal tmp_r = qSqrt(x*x + y*y);
        const qreal radian = qAsin(y/tmp_r);
        if(x < 0){
            wheel_cursor_x = -base_radius * qCos(radian) + center_x;
            wheel_cursor_y = base_radius * qSin(radian) + center_y;
        }else{
            wheel_cursor_x = base_radius * qCos(radian) + center_x;
            wheel_cursor_y = base_radius * qSin(radian) + center_y;
        }
        current_base_color = QColor::fromRgb(grab(QRect(QPoint(wheel_cursor_x, wheel_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
        current_color = QColor::fromRgb(grab(QRect(QPoint(sq_cursor_x, sq_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
    }
    repaint();
}


void FColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if(is_in_sq){
        sq_cursor_x = clampCursorPosX(event->pos().x(), top_left_x, sq_length);
        sq_cursor_y = clampCursorPosX(event->pos().y(), top_left_y, sq_length);
        current_color = QColor::fromRgb(grab(QRect(QPoint(sq_cursor_x, sq_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
    }

    if(is_in_wheel){
       const int x = event->pos().x() - center_x;
       const int y = event->pos().y() - center_y;
       const qreal tmp_r = qSqrt(x * x + y * y);
       const qreal radian = qAsin(y / tmp_r);

       if(x < 0){
           wheel_cursor_x = -base_radius * qCos(radian) + center_x;
           wheel_cursor_y = base_radius * qSin(radian) + center_y;
       }else{
           wheel_cursor_x = base_radius * qCos(radian) + center_x;
           wheel_cursor_y = base_radius * qSin(radian) + center_y;
       }
        current_base_color = QColor::fromRgb(grab(QRect(QPoint(wheel_cursor_x, wheel_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
        current_color = QColor::fromRgb(grab(QRect(QPoint(sq_cursor_x, sq_cursor_y), QSize(1,1))).toImage().pixel(0, 0));
   }
   repaint();
}


void FColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    is_in_wheel = false;
    is_in_sq= false;
    colorChangedSig(this->current_color);

}

