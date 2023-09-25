#include <gx/scene.hpp>

#include <cmath>

#include <iostream>

namespace gx {

ScreenPosition Camera::project(const ScreenPosition& worldPosition) const
{
    std::cerr << "worldPosition: " << worldPosition << "\n";
    std::cerr << "_position: " << _position << "\n";
    std::cerr << "_unitPixelSize: " << _unitPixelSize << "\n";
    std::cerr << "_zoom: " << _zoom << "\n";
    std::cerr << "_screenSize: " << _screenSize << "\n";

    return {
        .x = (worldPosition.x - _position.x) * _unitPixelSize * _zoom +
            (float)_screenSize.w / 2.f,
        .y = (_position.y - worldPosition.y) * _unitPixelSize * _zoom +
            (float)_screenSize.h / 2.f,
    };
}

float Camera::zoom() const
{
    return _zoom;
}

void Camera::setWindowSize(ScreenSize size)
{
    _screenSize = size;
}

void Camera::position(const ScreenPosition& center, float unitPixelSize, float zoom)
{
    _position = center;
    _unitPixelSize = unitPixelSize;
    _zoom = zoom;
}

Object::Object(const Sprite& sprite, const ScreenPosition& position)
    : _sprite(&sprite)
    , _position(position)
{ }

const Bitmap& Object::bitmap() const
{
    return *_sprite->bitmap;
}

const Frame& Object::frame() const
{
    return _sprite->frames.at(_frameIndex);
}

const ScreenPosition& Object::position() const
{
    return _position;
}

void Object::move(ScreenPosition position)
{
    _position = position;
}

void Object::update(double delta)
{
    _timeSum += delta;
    _frameIndex = static_cast<size_t>(_timeSum / _sprite->frameDuration) %
        _sprite->frames.size();
}

Camera& Scene::camera()
{
    return _camera;
}

void Scene::update(double delta)
{
    for (auto& object : _objects) {
        object.update(delta);
    }
}

void Scene::render(Renderer& renderer) const
{
    if (_texture) {
        auto zeroScreenCoords = _camera.project({0, 0});
        auto x0 = std::fmod(zeroScreenCoords.x, _texture->frames.front().w);
        auto y0 = std::fmod(zeroScreenCoords.y, _texture->frames.front().h);
    }

    for (const auto& object : _objects) {
        auto screenPosition = _camera.project(object.position());
        renderer.draw(
            object.bitmap(), object.frame(), screenPosition, _camera.zoom());
    }
}

ObjectId Scene::spawn(const Sprite& sprite, ScreenPosition position)
{
    return _objects.emplace(sprite, position);
}

void Scene::move(ObjectId objectId, ScreenPosition position)
{
    _objects[objectId].move(position);
}

void Scene::kill(ObjectId objectId)
{
    _objects.pop(objectId);
}

void Scene::layTexture(const Sprite& sprite)
{
    _texture = &sprite;
}

void Scene::removeTexture()
{
    _texture = nullptr;
}

} // namespace gx
