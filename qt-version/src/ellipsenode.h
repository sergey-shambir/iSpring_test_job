#pragma once
#include "abstractnode.h"

class EllipseNode : public AbstractNode
{
public:
    static const char *CLASS_NAME;

    EllipseNode() = default;
    EllipseNode(const rectangle &boundingRect);

    bool testHit(const vec2 &point) const override;
    json11::Json toJson() const override;
    void render(RenderContext &context) const override;
    bool fromJson(const json11::Json &json) override;
};
