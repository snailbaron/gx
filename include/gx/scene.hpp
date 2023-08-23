#pragma once

#include <gx/window.hpp>

#include <chrono>
#include <vector>

namespace gx {

class Camera {
public:
    Position project(const Position& worldPosition) const;

    void setWindowSize(int width, int height);

private:
    Position _position;
    float _unitPixelSize = 1.f;
    int _screenWidth = 0;
    int _screenHeight = 0;
};

class Sprite {
public:
    Sprite(const Bitmap& texture, std::vector<Frame> frames, int fps);

private:
    using Clock = std::chrono::high_resolution_clock;

    const Bitmap* _texture = nullptr;
    std::vector<Frame> _frames;
    double _frameDuration;
};

class SpriteState {
public:
    const Bitmap& texture() const;
    const Frame& frame() const;
    const Position& position() const;

    void move(Position position);
    void update(double delta);

private:
    const Sprite* _sprite = nullptr;
    size_t _frameIndex = 0;
    double _timeSum = 0.0;
    Position _position;
};

class Scene {
public:
    void update(double delta);
    void render(Renderer& window) const;

private:
    Camera _camera;
    std::vector<Sprite> _sprites;
};

} // namespace gx
