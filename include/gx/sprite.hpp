#pragma once

#include <gx/renderer.hpp>

#include <vector>

namespace gx {

struct SpriteFrame {
    const Bitmap* bitmap = nullptr;
    Frame frame;
    float duration = 0.f;
};

struct Sprite {
    std::vector<SpriteFrame> frames;
};

Sprite createSimpleSprite(
    const Bitmap& bitmap, int frameCount = 1, float fps = 1.f);

class Animation {
public:
    explicit Animation(const Sprite& sprite);

    const Bitmap& bitmap() const;
    const Frame& frame() const;

    void update(float delta);
    void draw(Renderer& renderer, const ScreenPoint& position) const;

private:
    const Sprite* _sprite = nullptr;
    std::vector<float> _durationSum;
    float _time = 0.0;
    size_t _frameIndex = 0;
};

} // namespace gx
