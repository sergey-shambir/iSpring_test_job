#pragma once
#include "abstractnode.h"

class TriangleNode : public AbstractNode
{
public:
    static const char *CLASS_NAME;

    TriangleNode() = default;
    explicit TriangleNode(const rectangle &boundingRect);

    bool testHit(const vec2 &point) const override;
    json11::Json toJson() const override;
    void render(IVGRenderer &renderer) const override;
    bool fromJson(const json11::Json &json) override;
};
