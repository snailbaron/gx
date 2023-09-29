#pragma once

#include <gx/geometry.hpp>
#include <gx/id.hpp>
#include <gx/renderer.hpp>
#include <gx/sprite.hpp>
#include <gx/ui.hpp>

#include <chrono>
#include <cmath>
#include <map>
#include <vector>

namespace gx {

struct WorldTag;
using WorldVector = Vector<float, WorldTag>;
using WorldPoint = Point<float, WorldTag>;

struct Object {
    Animation animation;
    WorldPoint position;
    bool kill = false;
};

struct Camera {
    ScreenVector project(const WorldPoint& worldPosition) const;
    void update(float delta);

    WorldPoint position;
    float unitPixelSize = 1.f;
    int zoom = 1;
    Object* follow = nullptr;
};

class Scene : public Widget {
public:
    Camera& camera();

    void update(float delta) override;
    void render(Renderer& renderer, const ScreenRectangle& area) const override;

    void setupCamera(const WorldPoint& center, float unitPixelSize, int zoom);
    void cameraFollow(Object* object);

    Object* spawn(const Sprite& sprite, const WorldPoint& position);

private:
    Camera _camera;
    std::vector<std::unique_ptr<Object>> _objects;
};

} // namespace gx
