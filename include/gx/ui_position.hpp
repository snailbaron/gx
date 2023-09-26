#pragma once

#include <limits>
#include <ostream>
#include <stdexcept>

namespace gx {

namespace internal {

constexpr bool isGoodFloat(long double x)
{
    return x >= std::numeric_limits<float>::lowest() &&
        x <= std::numeric_limits<float>::max();
}

} // namespace internal

struct ScreenLength {
    constexpr ScreenLength& operator+=(const ScreenLength& other)
    {
        pixels += other.pixels;
        centimeters += other.centimeters;
        fraction += other.fraction;
        return *this;
    }

    constexpr ScreenLength& operator-=(const ScreenLength& other)
    {
        pixels -= other.pixels;
        centimeters -= other.centimeters;
        fraction -= other.fraction;
        return *this;
    }

    constexpr ScreenLength& operator*=(float scalar)
    {
        pixels *= scalar;
        centimeters *= scalar;
        fraction *= scalar;
        return *this;
    }

    constexpr ScreenLength& operator/=(float scalar)
    {
        pixels /= scalar;
        centimeters /= scalar;
        fraction /= scalar;
        return *this;
    }

    float pixels = 0.f;
    float centimeters = 0.f;
    float fraction = 0.f;
};

constexpr ScreenLength operator ""_px(long double pixels)
{
    if (!internal::isGoodFloat(pixels)) {
        throw std::invalid_argument{"pixels"};
    }
    return ScreenLength{.pixels = (float)pixels};
}

constexpr ScreenLength operator ""_cm(long double centimeters)
{
    if (!internal::isGoodFloat(centimeters)) {
        throw std::invalid_argument{"centimeters"};
    }
    return ScreenLength{.centimeters = (float)centimeters};
}

constexpr ScreenLength operator ""_fr(long double fraction)
{
    if (!internal::isGoodFloat(fraction)) {
        throw std::invalid_argument{"fraction"};
    }
    return ScreenLength{.fraction = (float)fraction};
}

inline std::ostream& operator<<(
    std::ostream& output, const ScreenLength& windowPosition)
{
    return output << windowPosition.pixels << "px, " <<
        windowPosition.centimeters << "cm, " <<
        windowPosition.fraction << "fr";
}

constexpr ScreenLength operator+(ScreenLength lhs, const ScreenLength& rhs)
{
    lhs += rhs;
    return lhs;
}

constexpr ScreenLength operator-(ScreenLength lhs, const ScreenLength& rhs)
{
    lhs -= rhs;
    return lhs;
}

constexpr ScreenLength operator*(ScreenLength length, float scalar)
{
    length *= scalar;
    return length;
}

constexpr ScreenLength operator*(float scalar, ScreenLength length)
{
    length *= scalar;
    return length;
}

constexpr ScreenLength operator/(ScreenLength length, float scalar)
{
    length /= scalar;
    return length;
}


struct ScreenPoint {
    ScreenLength x;
    ScreenLength y;
};

inline std::ostream& operator<<(std::ostream& output, const ScreenPoint& screenPoint)
{
    return output << "(" << screenPoint.x << ", " << screenPoint.y << ")";
}

struct ScreenSize {
    ScreenLength w;
    ScreenLength h;
};

inline std::ostream& operator<<(std::ostream& output, const ScreenSize& screenSize)
{
    return output << "(" << screenSize.w << ", " << screenSize.h << ")";
}

} // namespace gx
