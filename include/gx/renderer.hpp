#pragma once

#include <gx/ui_position.hpp>

#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <ostream>
#include <span>
#include <vector>

namespace gx {

struct PixelPoint {
    float x = 0.f;
    float y = 0.f;
};

struct PixelSize {
    float w = 0.f;
    float h = 0.f;
};

struct Frame {
    int x = 0.f;
    int y = 0.f;
    int w = 0.f;
    int h = 0.f;
};

struct StrictSize {
    int w = 0;
    int h = 0;
};

class Bitmap {
public:
    Bitmap();

    StrictSize size() const;

private:
    explicit Bitmap(SDL_Texture* ptr);

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> _ptr;

    friend class Renderer;
};

class Cursor {
public:
    Cursor();

private:
    explicit Cursor(SDL_Surface* ptr, int x, int y);

    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> _surface;
    std::unique_ptr<SDL_Cursor, void(*)(SDL_Cursor*)> _cursor;

    friend class Renderer;
};

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

class Renderer {
public:
    Renderer();

    Bitmap loadBitmap(const std::filesystem::path& path) const;
    Bitmap loadBitmap(const std::span<const std::byte>& data) const;

    static Cursor loadCursor(const std::filesystem::path& path, int x, int y);
    static void setCursor(Cursor& cursor);

    void draw(
        const Bitmap& bitmap,
        const Frame& rect,
        const ScreenPoint& position,
        float zoom = 1.f);

    void drawRectangle(
        const ScreenPoint& position,
        const ScreenSize& size,
        const Color& color);

    bool processEvent(const SDL_Event& e);
    void clear();
    void present();

    StrictSize windowSize() const;

private:
    PixelPoint pixel(const ScreenPoint& screenPoint) const;
    PixelSize pixel(const ScreenSize& screenPoint) const;

    StrictSize _windowSize;
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> _window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _renderer;
};

} // namespace gx
