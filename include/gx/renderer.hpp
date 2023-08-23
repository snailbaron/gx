#pragma once

#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <span>
#include <vector>

namespace gx {

struct Position {
    float x = 0.f;
    float y = 0.f;
};

struct Size {
    int w = 0;
    int h = 0;
};

struct Frame {
    int x = 0.f;
    int y = 0.f;
    int w = 0.f;
    int h = 0.f;
};

class Bitmap {
public:
    Size size() const;

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
        const Bitmap& bitmap, const Frame& rect, const Position& position);

    void clear();
    void present();

private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> _window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _renderer;
};

} // namespace gx
