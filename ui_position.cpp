#include <gx/ui_position.hpp>

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace gx {

namespace {

constexpr bool isGoodFloat(long double x)
{
    return x >= std::numeric_limits<float>::lowest() &&
        x <= std::numeric_limits<float>::max();
}

} // namespace

constexpr UiLength& UiLength::operator+=(
    const UiLength& other)
{
    pixels += other.pixels;
    centimeters += other.centimeters;
    fraction += other.fraction;
    return *this;
}

constexpr UiLength& UiLength::operator-=(
    const UiLength& other)
{
    pixels -= other.pixels;
    centimeters -= other.centimeters;
    fraction -= other.fraction;
    return *this;
}

constexpr UiLength operator ""_px(unsigned long long pixels)
{
    if (pixels > std::numeric_limits<int>::max()) {
        throw std::invalid_argument{"pixels"};
    }
    return UiLength{.pixels = (int)pixels};
}

constexpr UiLength operator ""_cm(long double centimeters)
{
    if (!isGoodFloat(centimeters)) {
        throw std::invalid_argument{"centimeters"};
    }
    return UiLength{.centimeters = (float)centimeters};
}

constexpr UiLength operator ""_fr(long double fraction)
{
    if (!isGoodFloat(fraction)) {
        throw std::invalid_argument{"fraction"};
    }
    return UiLength{.fraction = (float)fraction};
}

std::ostream& operator<<(
    std::ostream& output, const UiLength& windowPosition)
{
    return output << windowPosition.pixels << "px, " <<
        windowPosition.centimeters << "cm, " <<
        windowPosition.fraction << "fr";
}

constexpr UiLength operator+(
    UiLength lhs, const UiLength& rhs)
{
    lhs += rhs;
    return lhs;
}

constexpr UiLength operator-(
    UiLength lhs, const UiLength& rhs)
{
    lhs -= rhs;
    return lhs;
}

} // namespace gx
