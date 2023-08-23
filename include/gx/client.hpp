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
    ObjectId spawn(SpriteId spriteId, Position position);

    void update(double delta);
    void present();

private:
    Renderer _window;

    Scene _scene;
    Ui _ui;
};

} // namespace gx
