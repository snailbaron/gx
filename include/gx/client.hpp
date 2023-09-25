#pragma once

#include <gx/renderer.hpp>
#include <gx/resources.hpp>
#include <gx/scene.hpp>
#include <gx/ui.hpp>

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <utility>

namespace gx {

class Client {
public:
    Client();

    SpriteId loadSprite(
        const std::filesystem::path& path,
        int fps = 1,
        const SpriteLayout& layout = horizontalLayout(1));

    ObjectId spawn(SpriteId spriteId, ScreenPosition position);

    void processInput();
    void update(double delta);
    void present();

    Scene& scene();

private:
    void processEvent(const SDL_Event& event);

    Renderer _renderer;
    Resources _resources;
    Scene _scene;
};

} // namespace gx
