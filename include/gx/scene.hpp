#pragma once

#include <gx/id.hpp>
#include <gx/renderer.hpp>
#include <gx/resources.hpp>

#include <chrono>
#include <map>

namespace gx {

class Camera {
public:
    ScreenPosition project(const ScreenPosition& worldPosition) const;
    float zoom() const;

    void setWindowSize(ScreenSize size);
    void position(const ScreenPosition& center, float unitPixelSize, float zoom);

private:
    ScreenPosition _position;
    ScreenSize _screenSize;
    float _unitPixelSize = 1.f;
    float _zoom = 1.f;
};

class Object {
public:
    Object(const Sprite& sprite, const ScreenPosition& position);

    const Bitmap& bitmap() const;
    const Frame& frame() const;
    const ScreenPosition& position() const;

    void move(ScreenPosition position);
    void update(double delta);

private:
    const Sprite* _sprite = nullptr;
    ScreenPosition _position;
    size_t _frameIndex = 0;
    double _timeSum = 0.0;
};

class ObjectId : public Id {};

class Scene {
public:
    Camera& camera();

    void update(double delta);
    void render(Renderer& renderer) const;

    ObjectId spawn(const Sprite& sprite, ScreenPosition position);
    void move(ObjectId objectId, ScreenPosition position);
    void kill(ObjectId objectId);

    void layTexture(const Sprite& sprite);
    void removeTexture();

private:
    Camera _camera;
    ObjectCache<ObjectId, Object> _objects;
    const Sprite* _texture = nullptr;
};

} // namespace gx
