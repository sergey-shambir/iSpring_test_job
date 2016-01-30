#include "stdafx.h"
#include "ellipsenode.h"

const char *EllipseNode::CLASS_NAME = "Ellipse";

EllipseNode::EllipseNode(const rectangle &boundingRect)
{
    setBoundingRect(boundingRect);
}

bool EllipseNode::testHit(const vec2 &point) const
{
    const rectangle &br = boundingRect();
    if (br.width <= 0 || br.height <= 0)
        return false;
    // Transform to circle inside square [-1..1].
    vec2 innerPoint = (point - br.origin()) / br.size();
    innerPoint.x = 2.f * innerPoint.x - 1.f;
    innerPoint.y = 2.f * innerPoint.y - 1.f;
    return 1.f >= innerPoint.x * innerPoint.x + innerPoint.y * innerPoint.y;
}

json11::Json EllipseNode::toJson() const
{
    json11::Json::object ret;
    ret["type"] = json11::Json(CLASS_NAME);
    ret["bounds"] = json11::Json(boundingRect().toString());
    return json11::Json(ret);
}

void EllipseNode::render(IVGRenderer &renderer) const
{
    renderer.DrawEllipse(boundingRect());
}

bool EllipseNode::fromJson(const json11::Json &json)
{
    rectangle rect;
    if (!rect.parseString(json["bounds"].string_value()))
        return false;
    setBoundingRect(rect);
    return true;
}
