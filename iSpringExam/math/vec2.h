#pragma once
#include <cmath>

namespace base {
template <class treal>
class vec2
{
public:
    treal x;
    treal y;

	inline explicit vec2()
		: x(0.f), y(0.f) { }
    inline explicit vec2(treal vx, treal vy)
        : x(vx), y(vy) { }
    inline explicit vec2(treal *array)
        : x(array[0]), y(array[1]) { }

    inline vec2 &operator +=(treal addenum)
    {
        x += addenum;
        y += addenum;
        return *this;
    }

    inline vec2 &operator +=(const vec2 &addenum)
    {
        x += addenum.x;
        y += addenum.y;
        return *this;
    }

    inline vec2 operator +(treal addenum) const
    {
        return vec2(x + addenum, y + addenum);
    }

    inline vec2 operator +(const vec2 &addenum) const
    {
        return vec2(x + addenum.x, y + addenum.y);
    }

    inline vec2 operator -(treal value) const
    {
        return vec2(x - value, y - value);
    }

    inline vec2 operator -(const vec2 &value) const
    {
        return vec2(x - value.x, y - value.y);
    }

    inline vec2 operator *(treal value) const
    {
        return vec2(x * value, y * value);
    }

    inline vec2 operator *(const vec2 &value) const
    {
        return vec2(x * value.x, y * value.y);
    }

    inline vec2 operator /(treal value) const
    {
        return vec2(x / value, y / value);
    }

    inline vec2 operator /(const vec2 &value) const
    {
        return vec2(x / value.x, y / value.y);
    }

    inline treal length() const
    {
        return std::sqrt(x * x + y * y);
    }

    inline vec2 normalized() const
    {
        treal len = length();
        return vec2(x / len, y / len);
    }

    inline vec2 &normalize()
    {
        treal len = length();
        x /= len;
        y /= len;
        return *this;
    }

    inline treal distance(vec2 const& value) const
    {
        treal dx = x - value.x;
        treal dy = y - value.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

} // namespace base

template <class treal>
float dot(const base::vec2<treal> &left, const base::vec2<treal> &right)
{
    return left.x * right.x + left.y * right.y;
}

template <class treal>
base::vec2<treal> operator *(const treal &left, const base::vec2<treal> &right)
{
	return base::vec2<treal>(left * right.x, left * right.y);
}

typedef base::vec2<float> vec2;
typedef base::vec2<double> vec2d;
