#include <gx/sprite.hpp>

#include <gx/error.hpp>

#include <algorithm>
#include <cmath>

namespace gx {

Sprite createSimpleSprite(const Bitmap& bitmap, int frameCount, float fps)
{
    auto size = bitmap.size();

    if (size.w % frameCount != 0) {
        throw Error{
            "cannot divide texture of width " + std::to_string(size.w) +
            " into " + std::to_string(frameCount) + " parts"};
    }
    auto spriteWidth = size.w / frameCount;

    auto sprite = Sprite{};
    for (int i = 0; i < frameCount; i++) {
        sprite.frames.push_back(SpriteFrame{
            .bitmap = &bitmap,
            .frame = Frame{
                .x = i * spriteWidth,
                .y = 0,
                .w = spriteWidth,
                .h = size.h
            },
            .duration = 1.f / fps,
        });
    }

    return sprite;
}

Animation::Animation(const Sprite& sprite)
    : _sprite(&sprite)
{
    _durationSum.reserve(sprite.frames.size());
    float sum = 0.0;
    for (const auto& frame : sprite.frames) {
        sum += frame.duration;
        _durationSum.push_back(sum);
    }
}

const Bitmap& Animation::bitmap() const
{
    return *_sprite->frames.at(_frameIndex).bitmap;
}

const Frame& Animation::frame() const
{
    return _sprite->frames.at(_frameIndex).frame;
}

void Animation::update(float delta)
{
    float totalDuration = _durationSum.back();
    _time = std::fmod(_time + delta, totalDuration);
    _frameIndex =
        std::ranges::upper_bound(_durationSum, _time) - _durationSum.begin();
}

void Animation::draw(Renderer& renderer, const ScreenPoint& position) const
{
    renderer.draw(bitmap(), frame(), position);
}

} // namespace gx
