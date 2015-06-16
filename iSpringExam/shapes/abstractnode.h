#pragma once
#include "../math/math"
#include "json11.hpp"
#include "../render/IVGRenderer.h"
#include <memory>

class AbstractNode
{
public:
    virtual ~AbstractNode() = default;
    virtual bool testHit(const vec2 &point) const = 0;
    virtual json11::Json toJson() const = 0;
    virtual void render(IVGRenderer &renderer) const = 0;
    virtual bool fromJson(const json11::Json &json) = 0;

    const rectangle &boundingRect() const;
    vec2 minimalSize() const;
    virtual void resizeTo(const vec2 &boundingSize);
    virtual void moveTo(const vec2 &toPoint);
    virtual void moveBy(const vec2 &delta);
    virtual void setBoundingRect(const rectangle &rect);

private:
    rectangle m_boundingRect;
};

typedef std::shared_ptr<AbstractNode> NodePtr;
