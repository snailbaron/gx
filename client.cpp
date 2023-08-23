#include <gx/client.hpp>

#include <gx/error.hpp>

#include <string>

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

TextureId Client::loadTexture(const std::filesystem::path& path)
{
    return _textures.push(_window.loadBitmap(path));
}

TextureId Client::loadTexture(const std::span<const std::byte>& data)
{
    return _textures.push(_window.loadTexture(data));
}

SpriteId Client::loadSprite(
    const std::filesystem::path& path, int fps, const SpriteLayout& layout)
{
    auto textureId = loadTexture(path);
    const auto& size = _textures[textureId].size();

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

            return _sprites.emplace(
                _textures[textureId], std::move(frames), fps);
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

            return _sprites.emplace(
                _textures[textureId], std::move(frames), fps);
        }
    }

    throw Error{
        "unknown SpriteLayout::Type value: " +
        std::to_string((int)layout.type)};
}

ObjectId Client::spawn(SpriteId spriteId, Position position)
{
}

void Client::update(double delta)
{
    _scene.update(delta);
    _ui.update(delta);
}

void Client::present()
{
    _window.clear();
    _scene.render(_window);
    _ui.render(_window);
    _window.present();
}

} // namespace gx
