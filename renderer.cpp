#include <gx/renderer.hpp>

#include <gx/error.hpp>

#include <SDL_image.h>

#include <utility>

#include <iostream>

namespace gx {

namespace {

} // namespace

Bitmap::Bitmap()
    : _ptr(nullptr, SDL_DestroyTexture)
{ }

Bitmap::Bitmap(SDL_Texture* ptr)
    : _ptr(ptr, SDL_DestroyTexture)
{ }

StrictSize Bitmap::size() const
{
    auto size = StrictSize{};
    sdlCheck(SDL_QueryTexture(_ptr.get(), nullptr, nullptr, &size.w, &size.h));
    return size;
}

Renderer::Renderer()
    : _window(
        sdlCheck(SDL_CreateWindow(
            "gx",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1024,
            768,
            SDL_WINDOW_RESIZABLE)),
        SDL_DestroyWindow)
    , _renderer(
        sdlCheck(SDL_CreateRenderer(
            _window.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
        SDL_DestroyRenderer)
{
    SDL_GetWindowSize(_window.get(), &_windowSize.w, &_windowSize.h);
}

Bitmap Renderer::loadBitmap(const std::filesystem::path& path) const
{
    return Bitmap{
        sdlCheck(IMG_LoadTexture(_renderer.get(), path.string().c_str()))
    };
}

Bitmap Renderer::loadBitmap(const std::span<const std::byte>& data) const
{
    return Bitmap{
        sdlCheck(IMG_LoadTexture_RW(
            _renderer.get(),
            sdlCheck(SDL_RWFromMem(
                (void*)data.data(),
                static_cast<int>(data.size()))),
            1 /* freesrc */))
    };
}

void Renderer::draw(
    const Bitmap& bitmap,
    const Frame& rect,
    const ScreenPoint& position,
    float zoom)
{
    auto pixelPosition = pixel(position);
    auto src = SDL_Rect{.x = rect.x, .y = rect.y, .w = rect.w, .h = rect.h};
    auto dst = SDL_FRect{
        .x = pixelPosition.x - zoom * (float)rect.w / 2.f,
        .y = pixelPosition.y - zoom * (float)rect.h / 2.f,
        .w = zoom * (float)rect.w,
        .h = zoom * (float)rect.h
    };

    sdlCheck(SDL_RenderCopyF(_renderer.get(), bitmap._ptr.get(), &src, &dst));
}

void Renderer::drawRectangle(
    const ScreenPoint& position,
    const ScreenSize& size,
    const Color& color)
{
    auto pixelPosition = pixel(position);
    auto pixelSize = pixel(size);

    auto rect = SDL_FRect{
        .x = pixelPosition.x - pixelSize.w / 2,
        .y = pixelPosition.y - pixelSize.h / 2,
        .w = pixelSize.w,
        .h = pixelSize.h
    };

    sdlCheck(SDL_SetRenderDrawColor(
        _renderer.get(), color.r, color.g, color.b, color.a));
    sdlCheck(SDL_RenderFillRectF(_renderer.get(), &rect));
}

bool Renderer::processEvent(const SDL_Event& e)
{
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
        std::cerr << "renderer processing resize\n";
        _windowSize = {e.window.data1, e.window.data2};
        return true;
    }

    return false;
}

void Renderer::clear()
{
    sdlCheck(SDL_RenderClear(_renderer.get()));
}

void Renderer::present()
{
    SDL_RenderPresent(_renderer.get());
}

StrictSize Renderer::windowSize() const
{
    return _windowSize;
}

PixelPoint Renderer::pixel(const ScreenPoint& screenPoint) const
{
    return {
        .x = screenPoint.x.pixels +
            screenPoint.x.fraction * (float)_windowSize.w,
        .y = screenPoint.y.pixels +
            screenPoint.y.fraction * (float)_windowSize.h
    };
}
PixelSize Renderer::pixel(const ScreenSize& screenPoint) const
{
    return {
        .w = screenPoint.w.pixels +
            screenPoint.w.fraction * (float)_windowSize.w,
        .h = screenPoint.h.pixels +
            screenPoint.h.fraction * (float)_windowSize.h
    };
}

} // namespace gx
