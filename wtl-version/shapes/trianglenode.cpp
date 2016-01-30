
#include "stdafx.h"
#include "trianglenode.h"

const char *TriangleNode::CLASS_NAME = "Triangle";

TriangleNode::TriangleNode(const rectangle &boundingRect)
{
    setBoundingRect(boundingRect);
}

bool TriangleNode::testHit(const vec2 &point) const
{
    const rectangle &br = boundingRect();
    if (br.width <= 0 || br.height <= 0)
        return false;
    if (!br.hits(point))
        return false;
    // Transform to triangle inside square [0..1]
    // than flip right triangle side
    // also flip Y
    vec2 innerPoint = (point - br.origin()) / br.size();
    if (innerPoint.x > 0.5f)
        innerPoint.x = 1.0f - innerPoint.x;
    innerPoint.y = 1.f - innerPoint.y;
    return 2.f * innerPoint.x > innerPoint.y;
}

json11::Json TriangleNode::toJson() const
{
    json11::Json::object ret;
    ret["type"] = json11::Json(CLASS_NAME);
    ret["bounds"] = json11::Json(boundingRect().toString());
    return json11::Json(ret);
}

void TriangleNode::render(IVGRenderer &renderer) const
{
    const rectangle &br = boundingRect();
    std::vector<vec2> points = {
        vec2(br.x, br.y + br.height),
        vec2(br.x + 0.5f * br.width, br.y),
        vec2(br.x + br.width, br.y + br.height)
    };
    renderer.DrawPolygon(points);
}

bool TriangleNode::fromJson(const json11::Json &json)
{
    rectangle br;
    if (!br.parseString(json["bounds"].string_value()))
        return false;
    setBoundingRect(br);
    return true;
}
