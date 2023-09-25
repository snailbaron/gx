#include <gx/client.hpp>

#include <gx/error.hpp>

#include <string>

namespace gx {

Client::Client()
{
    _scene.camera().setWindowSize(_renderer.windowSize());
}

SpriteId Client::loadSprite(
    const std::filesystem::path& path,
    int fps,
    const SpriteLayout& layout)
{
    return _resources.loadSprite(_renderer, path, fps, layout);
}

ObjectId Client::spawn(SpriteId spriteId, ScreenPosition position)
{
    return _scene.spawn(_resources[spriteId], position);
}

void Client::processInput()
{
    for (SDL_Event e; SDL_PollEvent(&e); ) {
        processEvent(e);
    }
}

void Client::update(double delta)
{
    _scene.update(delta);
}

void Client::present()
{
    _renderer.clear();
    _scene.render(_renderer);
    _renderer.present();
}

Scene& Client::scene()
{
    return _scene;
}

void Client::processEvent(const SDL_Event& event)
{
    

}


} // namespace gx
