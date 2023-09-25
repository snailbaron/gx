#include <gx/resources.hpp>

#include <gx/error.hpp>

namespace gx {

SpriteLayout horizontalLayout(int frameCount)
{
    return SpriteLayout{
        .type = SpriteLayout::Type::Horizontal, .frameCount = frameCount};
}

SpriteLayout verticalLayout(int frameCount)
{
    return SpriteLayout{
        .type = SpriteLayout::Type::Vertical, .frameCount = frameCount};
}

BitmapId Resources::loadBitmap(
    const Renderer& renderer, const std::filesystem::path& path)
{
    return _bitmaps.push(renderer.loadBitmap(path));
}

BitmapId Resources::loadBitmap(
    const Renderer& renderer, const std::span<const std::byte>& data)
{
    return _bitmaps.push(renderer.loadBitmap(data));
}

SpriteId Resources::loadSprite(
    const Renderer& renderer,
    const std::filesystem::path& path,
    int fps,
    const SpriteLayout& layout)
{
    auto bitmapId = loadBitmap(renderer, path);
    const auto& size = _bitmaps[bitmapId].size();

    switch (layout.type) {
        case SpriteLayout::Type::Horizontal:
        {
            if (size.w % layout.frameCount != 0) {
                throw Error{
                    "cannot divide texture of width " + std::to_string(size.w) +
                    " into " + std::to_string(layout.frameCount) + " parts"};
            }
            auto spriteWidth = size.w / layout.frameCount;

            auto frames = std::vector<Frame>{};
            for (int i = 0; i < layout.frameCount; i++) {
                frames.push_back({
                    .x = i * spriteWidth,
                    .y = 0,
                    .w = spriteWidth,
                    .h = size.h
                });
            }

            auto sprite = Sprite{
                .bitmapId = bitmapId,
                .bitmap = &_bitmaps[bitmapId],
                .frames = std::move(frames),
                .frameDuration = 1.0 / fps
            };
            return _sprites.push(std::move(sprite));
        }
        case SpriteLayout::Type::Vertical:
        {
            if (size.h % layout.frameCount != 0) {
                throw Error{
                    "cannot divide texture of height " +
                    std::to_string(size.h) + " info " +
                    std::to_string(layout.frameCount) + " parts"};
            }
            auto spriteHeight = size.h / layout.frameCount;

            auto frames = std::vector<Frame>{};
            for (int i = 0; i < layout.frameCount; i++) {
                frames.push_back({
                    .x = 0,
                    .y = i * spriteHeight,
                    .w = size.w,
                    .h = spriteHeight
                });
            }

            return _sprites.push(Sprite{
                .bitmapId = bitmapId,
                .bitmap = &_bitmaps[bitmapId],
                .frames = std::move(frames),
                .frameDuration = 1.0 / fps
            });
        }
    }

    throw Error{
        "unknown SpriteLayout::Type value: " +
        std::to_string((int)layout.type)};
}

const Bitmap& Resources::operator[](BitmapId bitmapId) const
{
    return _bitmaps[bitmapId];
}

const Sprite& Resources::operator[](SpriteId spriteId) const
{
    return _sprites[spriteId];
}

} // namespace gx
