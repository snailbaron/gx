#pragma once

#include <gx/id.hpp>
#include <gx/renderer.hpp>
#include <gx/sprite.hpp>
#include <gx/ui.hpp>

#include <chrono>
#include <cmath>
#include <map>
#include <vector>

namespace gx {

struct WorldVector {
    constexpr WorldVector& operator+=(const WorldVector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr WorldVector& operator-=(const WorldVector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr WorldVector& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr WorldVector& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float x = 0.f;
    float y = 0.f;
};

constexpr WorldVector operator+(WorldVector lhs, const WorldVector& rhs)
{
    lhs += rhs;
    return lhs;
}

constexpr WorldVector operator-(WorldVector lhs, const WorldVector& rhs)
{
    lhs -= rhs;
    return lhs;
}

constexpr WorldVector operator*(WorldVector vector, float scalar)
{
    vector *= scalar;
    return vector;
}

constexpr WorldVector operator*(float scalar, WorldVector vector)
{
    vector *= scalar;
    return vector;
}

constexpr WorldVector operator/(WorldVector vector, float scalar)
{
    vector /= scalar;
    return vector;
}

constexpr float length(const WorldVector& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

constexpr WorldVector normalize(const WorldVector& vector)
{
    if (float l = length(vector); l != 0) {
        return vector / l;
    }
    return WorldVector{0, 0};
}

constexpr WorldVector resize(const WorldVector& vector, float newLength)
{
    if (float l = length(vector); l != 0) {
        return vector * (newLength / l);
    }
    return WorldVector{0, 0};
}

struct WorldPoint {
    constexpr WorldPoint& operator+=(const WorldVector& vector)
    {
        x += vector.x;
        y += vector.y;
        return *this;
    }

    constexpr WorldPoint& operator-=(const WorldVector& vector)
    {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }

    float x = 0.f;
    float y = 0.f;
};

constexpr WorldVector operator-(const WorldPoint& lhs, const WorldPoint& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

struct Object {
    Animation animation;
    WorldPoint position;
    bool kill = false;
};

struct Camera {
    ScreenPoint project(const WorldPoint& worldPosition) const;
    void update(float delta);

    WorldPoint position;
    float unitPixelSize = 1.f;
    int zoom = 1;
    Object* follow = nullptr;
};

class Scene : public Widget {
public:
    Camera& camera();

    void update(float delta) override;
    void render(Renderer& renderer) const override;

    void setupCamera(const WorldPoint& center, float unitPixelSize, int zoom);
    void cameraFollow(Object* object);

    Object* spawn(const Sprite& sprite, const WorldPoint& position);

private:
    Camera _camera;
    std::vector<std::unique_ptr<Object>> _objects;
};

} // namespace gx
