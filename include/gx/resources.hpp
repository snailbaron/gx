#pragma once

#include <gx/id.hpp>
#include <gx/renderer.hpp>

#include <cstddef>
#include <filesystem>
#include <map>
#include <span>
#include <string>
#include <utility>

namespace gx {

class BitmapId : public Id {};
class SpriteId : public Id {};

struct SpriteLayout {
    enum class Type {
        Horizontal,
        Vertical,
    };

    Type type = Type::Horizontal;
    int frameCount = 0;
};

SpriteLayout horizontalLayout(int frameCount);
SpriteLayout verticalLayout(int frameCount);

struct Sprite {
    BitmapId bitmapId;
    Bitmap* bitmap = nullptr;
    std::vector<Frame> frames;
    double frameDuration = 0.0;
};

class Resources {
public:
    BitmapId loadBitmap(
        const Renderer& renderer, const std::filesystem::path& path);
    BitmapId loadBitmap(
        const Renderer& renderer, const std::span<const std::byte>& data);

    SpriteId loadSprite(
        const Renderer& renderer,
        const std::filesystem::path& path,
        int fps,
        const SpriteLayout& layout = horizontalLayout(1));

    const Bitmap& operator[](BitmapId bitmapId) const;
    const Sprite& operator[](SpriteId spriteId) const;

private:
    ObjectCache<BitmapId, Bitmap> _bitmaps;
    ObjectCache<SpriteId, Sprite> _sprites;

    std::map<BitmapId, std::string> _bitmapNames;
    std::map<SpriteId, std::string> _spriteNames;
};

} // namespace gx
