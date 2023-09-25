#include <gx/renderer.hpp>

#include <gx/error.hpp>

#include <SDL_image.h>

#include <utility>

#include <iostream>

namespace gx {

Bitmap::Bitmap(SDL_Texture* ptr)
    : _ptr(ptr, SDL_DestroyTexture)
{ }

ScreenSize Bitmap::size() const
{
    auto size = ScreenSize{};
    sdlCheck(SDL_QueryTexture(_ptr.get(), nullptr, nullptr, &size.w, &size.h));
    return size;
}

Renderer::Renderer()
    : _window(
        sdlCheck(SDL_CreateWindow(
            "gx",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800,
            600,
            0)),
        SDL_DestroyWindow)
    , _renderer(
        sdlCheck(SDL_CreateRenderer(
            _window.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
        SDL_DestroyRenderer)
{ }

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
    const ScreenPosition& position,
    float zoom)
{
    auto src = SDL_Rect{.x = rect.x, .y = rect.y, .w = rect.w, .h = rect.h};
    auto dst = SDL_FRect{
        .x = position.x - zoom * (float)rect.w / 2.f,
        .y = position.y - zoom * (float)rect.h / 2.f,
        .w = zoom * (float)rect.w,
        .h = zoom * (float)rect.h
    };

    sdlCheck(SDL_RenderCopyF(_renderer.get(), bitmap._ptr.get(), &src, &dst));
}

void Renderer::clear()
{
    sdlCheck(SDL_RenderClear(_renderer.get()));
}

void Renderer::present()
{
    SDL_RenderPresent(_renderer.get());
}

ScreenSize Renderer::windowSize() const
{
    auto size = ScreenSize{};
    SDL_GetWindowSize(_window.get(), &size.w, &size.h);
    return size;
}

} // namespace gx
