#include "stdafx.h"
#include "rectanglenode.h"

const char *RectangleNode::CLASS_NAME = "Rectangle";

RectangleNode::RectangleNode(const rectangle &boundingRect)
{
    setBoundingRect(boundingRect);
}

bool RectangleNode::testHit(const vec2 &point) const
{
    return boundingRect().hits(point);
}

json11::Json RectangleNode::toJson() const
{
    json11::Json::object ret;
    ret["type"] = json11::Json(CLASS_NAME);
    ret["bounds"] = json11::Json(boundingRect().toString());
    return json11::Json(ret);
}

void RectangleNode::render(IVGRenderer &renderer) const
{
    renderer.DrawRectangle(boundingRect());
}

bool RectangleNode::fromJson(const json11::Json &json)
{
    rectangle br;
    if (!br.parseString(json["bounds"].string_value()))
        return false;
    setBoundingRect(br);
    return true;
}
