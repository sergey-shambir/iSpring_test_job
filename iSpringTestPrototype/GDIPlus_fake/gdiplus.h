#pragma once
#include <QtGlobal>
#include <QRectF>
#include <QPen>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

namespace Gdiplus {

class Graphics;
class PointF;
class SizeF;
class RectF;
class Pen;
class Brush;

enum Status {
    Ok = 0,
    InvalidParameter = 2
};

class Graphics
{
public:
    Graphics(QPainter *painter);

    Status DrawEllipse(const Pen *pen, const RectF &rect);
    Status FillEllipse(const Brush *brush, const RectF &rect);
    Status DrawRectangle(const Pen *pen, const RectF &rect);
    Status DrawRectangle(const Pen *pen, float x, float y, float width, float height);
    Status FillRectangle(const Brush *brush, const RectF &rect);
    Status FillRectangle(const Brush *brush, float x, float y, float width, float height);
    Status DrawPolygon(const Pen *pen, const PointF *points, int count);
    Status FillPolygon(const Brush *brush, const PointF *points, int count);

private:
    QPainter *m_painter = nullptr;
};

class PointF
{
public:
    PointF();
    PointF(const QPointF &qPoint);
    PointF(float x, float y);
    QPointF qPoint() const;

private:
    QPointF m_qPoint;
};

class SizeF
{
public:
    QSizeF qSize() const;

private:
    QSizeF m_qSize;
};

class RectF
{
public:
    RectF();
    RectF(const PointF &pos, const SizeF &size);
    RectF(float x, float y, float width, float height);
    QRectF qRect() const;

private:
    QRectF m_qRect;
};

class Pen
{
public:
    Pen(const QPen &qPen);
    QPen qPen() const;
    void setQPen(const QPen &qPen);

private:
    QPen m_qPen;
};

class Brush
{
public:
    Brush(const QBrush &qBrush);
    QBrush qBrush() const;
    void setQBrush(const QBrush &qBrush);

private:
    QBrush m_qBrush;
};

} // namespace GDIPlus
