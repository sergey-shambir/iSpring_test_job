#pragma once
#include "vec4.h"
#include <cstdio>

namespace base {

template<class treal>
class rectangle
{
public:
    //! attributes
    treal x;
    treal y;
    treal width;
    treal height;

    //! constructors
    explicit rectangle(treal vx = 0.0, treal vy = 0.0, treal vwidth = 0.0, treal vheight = 0.0)
        : x(vx), y(vy), width(vwidth), height(vheight)
    {
    }

    explicit rectangle(vec4<float> const& values)
        : x(values.x), y(values.y), width(values.z), height(values.w)
    {
    }

    explicit rectangle(vec4<double> const& values)
        : x(values.x), y(values.y), width(values.z), height(values.w)
    {
    }

    explicit rectangle(vec2<treal> const& origin, vec2<treal> const& size)
        : x(origin.x), y(origin.y), width(size.x), height(size.y)
    {
    }

    explicit rectangle(const float *values)
        : x(values[0]), y(values[1]), width(values[2]), height(values[3])
    {
    }

    explicit rectangle(const double *values)
        : x(values[0]), y(values[1]), width(values[2]), height(values[3])
    {
    }

    //! methods
    inline vec2<treal> origin() const
    {
        return vec2<treal>(x, y);
    }

    inline vec2<treal> size() const
    {
        return vec2<treal>(width, height);
    }

    inline bool hits(treal px, treal py) const
    {
        return (px >= x) && (py >= y) && (px <= x + width) && (py <= y + height);
    }

    inline bool hits(const vec2<treal> &p) const
    {
        return hits(p.x, p.y);
    }

    inline bool hitsBound(treal px, treal py) const
    {
        return (x == px) || (y == py) || (x + width == px) || (y + height == py);
    }

    inline bool hitsBound(const vec2<treal> &p) const
    {
        return hitsBound(p.x, p.y);
    }

    inline treal diagonalLength() const
    {
        return std::sqrt(width * width + height * height);
    }

    inline void moveBy(treal dx, treal dy)
    {
        x += dx;
        y += dy;
    }

    inline void moveBy(const vec2<treal> &dp)
    {
        x += dp.x;
        y += dp.y;
    }

    inline void moveTo(treal x, treal y)
    {
        x = x;
        y = y;
    }

    inline void moveTo(const vec2<treal> &p)
    {
        x = p.x;
        y = p.y;
    }

    inline void resizeTo(const vec2<treal> &size)
    {
        width = size.x;
        height = size.y;
    }

    std::string toString() const
    {
        char data[256];
        std::sprintf(data, "%f;%f;%f;%f", x, y, width, height);
        return data;
    }

    bool parseString(const std::string &data)
    {
        float parsed[4];
        const int amount = std::sscanf(data.c_str(), "%f;%f;%f;%f", parsed, parsed + 1, parsed + 2, parsed + 3);
        if (amount == 4) {
            x = parsed[0];
            y = parsed[1];
            width = parsed[2];
            height = parsed[3];
            return true;
        }
        return false;
    }

    static rectangle fromString(const std::string &data)
    {
        rectangle ret;
        if (ret.parseString(data))
            return ret;
        return rectangle();
    }
};

} // namespace base

typedef base::rectangle<float> rectangle;
typedef base::rectangle<double> rectangled;
