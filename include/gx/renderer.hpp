#pragma once

#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <ostream>
#include <span>
#include <vector>

namespace gx {

struct ScreenPosition {
    float x = 0.f;
    float y = 0.f;
};

inline std::ostream& operator<<(
    std::ostream& output, const ScreenPosition& position)
{
    return output << "(" << position.x << ", " << position.y << ")";
}

struct ScreenSize {
    int w = 0;
    int h = 0;
};

inline std::ostream& operator<<(std::ostream& output, const ScreenSize& size)
{
    return output << "(" << size.w << ", " << size.h << ")";
}

struct Frame {
    int x = 0.f;
    int y = 0.f;
    int w = 0.f;
    int h = 0.f;
};

class Bitmap {
public:
    ScreenSize size() const;

private:
    explicit Bitmap(SDL_Texture* ptr);

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> _ptr;

    friend class Renderer;
};

class Renderer {
public:
    Renderer();

    Bitmap loadBitmap(const std::filesystem::path& path) const;
    Bitmap loadBitmap(const std::span<const std::byte>& data) const;

    void draw(
        const Bitmap& bitmap,
        const Frame& rect,
        const ScreenPosition& position,
        float zoom = 1.f);

    void processInput();
    void clear();
    void present();

    ScreenSize windowSize() const;

private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> _window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _renderer;
};

} // namespace gx
