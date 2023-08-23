#pragma once

#include <istream>
#include <ostream>

namespace gx {

struct UiLength {
    constexpr UiLength& operator+=(const UiLength& other);
    constexpr UiLength& operator-=(const UiLength& other);

    int pixels = 0;
    float centimeters = 0.f;
    float fraction = 0.f;
};

constexpr UiLength operator ""_px(unsigned long long pixels);
constexpr UiLength operator ""_cm(long double centimeters);
constexpr UiLength operator ""_fr(long double fraction);

std::ostream& operator<<(
    std::ostream& output, const UiLength& windowPosition);

constexpr UiLength operator+(
    UiLength lhs, const UiLength& rhs);
constexpr UiLength operator-(
    UiLength lhs, const UiLength& rhs);

struct UiPoint {
    UiLength x;
    UiLength y;
};

struct UiSize {
    UiLength w;
    UiLength h;
};

} // namespace gx
