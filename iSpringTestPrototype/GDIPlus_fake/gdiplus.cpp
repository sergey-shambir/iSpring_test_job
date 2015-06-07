#include "gdiplus.h"
#include <QPainter>

namespace GDIPlus {

Graphics::Graphics(QPainter *painter)
    : m_painter(painter)
{
}

Status Graphics::DrawEllipse(const Pen *pen, const RectF &rect)
{
    m_painter->setPen(pen->qPen());
    m_painter->setBrush(Qt::NoBrush);
    m_painter->drawEllipse(rect.qRect());
    return Ok;
}

Status Graphics::FillEllipse(const Brush *brush, const RectF &rect)
{
    m_painter->setPen(Qt::NoPen);
    m_painter->setBrush(brush->qBrush());
    m_painter->drawEllipse(rect.qRect());
    return Ok;
}

Status Graphics::DrawRectangle(const Pen *pen, const RectF &rect)
{
    m_painter->setPen(pen->qPen());
    m_painter->setBrush(Qt::NoBrush);
    m_painter->drawRect(rect.qRect());
    return Ok;
}

Status Graphics::DrawRectangle(const Pen *pen, float x, float y, float width, float height)
{
    m_painter->setPen(pen->qPen());
    m_painter->setBrush(Qt::NoBrush);
    m_painter->drawRect(x, y, width, height);
    return Ok;
}

Status Graphics::FillRectangle(const Brush *brush, const RectF &rect)
{
    m_painter->setPen(Qt::NoPen);
    m_painter->setBrush(brush->qBrush());
    m_painter->drawRect(rect.qRect());
    return Ok;
}

Status Graphics::FillRectangle(const Brush *brush, float x, float y, float width, float height)
{
    m_painter->setPen(Qt::NoPen);
    m_painter->setBrush(brush->qBrush());
    m_painter->drawRect(x, y, width, height);
    return Ok;
}

Status Graphics::DrawPolygon(const Pen *pen, const PointF *points, int count)
{
    if (count <= 0)
        return InvalidParameter;

    QPolygonF polygon;
    for (int i = 0; i < count; ++i)
        polygon << points[i].qPoint();
    m_painter->setPen(pen->qPen());
    m_painter->setBrush(Qt::NoBrush);
    m_painter->drawPolygon(polygon);
    return Ok;
}

Status Graphics::FillPolygon(const Brush *brush, const PointF *points, int count)
{
    if (count <= 0)
        return InvalidParameter;

    QPolygonF polygon;
    for (int i = 0; i < count; ++i)
        polygon << points[i].qPoint();
    m_painter->setPen(Qt::NoPen);
    m_painter->setBrush(brush->qBrush());
    m_painter->drawPolygon(polygon);
    return Ok;
}

RectF::RectF()
    : m_qRect()
{
}

RectF::RectF(const GDIPlus::PointF &pos, const GDIPlus::SizeF &size)
    : m_qRect(pos.qPoint(), size.qSize())
{
}

RectF::RectF(float x, float y, float width, float height)
    : m_qRect(x, y, width, height)
{
}

QRectF RectF::qRect() const
{
    return m_qRect;
}

PointF::PointF()
    : m_qPoint()
{
}

PointF::PointF(const QPointF &qPoint)
    : m_qPoint(qPoint)
{
}

PointF::PointF(float x, float y)
    : m_qPoint(x, y)
{
}

QPointF PointF::qPoint() const
{
    return m_qPoint;
}

QSizeF SizeF::qSize() const
{
    return m_qSize;
}

Pen::Pen(const QPen &qPen)
    : m_qPen(qPen)
{
}

QPen Pen::qPen() const
{
    return m_qPen;
}

void Pen::setQPen(const QPen &qPen)
{
    m_qPen = qPen;
}

Brush::Brush(const QBrush &qBrush)
    : m_qBrush(qBrush)
{
}

QBrush Brush::qBrush() const
{
    return m_qBrush;
}

void Brush::setQBrush(const QBrush &qBrush)
{
    m_qBrush = qBrush;
}

} // namespace GDIPlus
