#include <gx/scene.hpp>

namespace gx {

Position Camera::project(const Position& worldPosition) const
{
    return {
        .x = (worldPosition.x - _position.x) * _unitPixelSize +
            (float)_screenWidth / 2.f,
        .y = (_position.y - worldPosition.y) * _unitPixelSize +
            (float)_screenHeight / 2.f,
    };
}

void Camera::setWindowSize(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
}

Sprite::Sprite(const Bitmap& texture, std::vector<Frame> frames, int fps)
    : _texture(texture)
    , _frames(std::move(frames))
    , _frameDuration(1.0 / fps)
{ }

const Bitmap& Sprite::texture() const
{
    return _texture;
}

const Frame& Sprite::frame() const
{
    return _frames.at(_frameIndex);
}

const Position& Sprite::position() const
{
    return _position;
}

void Sprite::move(Position position)
{
    _position = position;
}

void Sprite::update(double delta)
{
    _timeSum += delta;
    _frameIndex =
        static_cast<size_t>(_timeSum / _frameDuration) % _frames.size();
}

void Scene::update(double delta)
{
    for (auto& sprite : _sprites) {
        sprite.update(delta);
    }
}

void Scene::render(Renderer& window) const
{
    for (const auto& sprite : _sprites) {
        auto screenPosition = _camera.project(sprite.position());
        window.draw(sprite.texture(), sprite.frame(), screenPosition);
    }
}

} // namespace gx
