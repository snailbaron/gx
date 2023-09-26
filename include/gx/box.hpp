#pragma once

#include <gx/renderer.hpp>
#include <gx/scene.hpp>
#include <gx/ui.hpp>

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <utility>
#include <vector>

namespace gx {

class Box {
public:
    Bitmap loadBitmap(const std::filesystem::path& path) const;
    Bitmap loadBitmap(const std::span<const std::byte>& data) const;

    bool processEvent(const SDL_Event& e);
    void update(double delta);
    void present();

    bool alive() const;
    bool dead() const;

    template <class T, class... Args>
    requires std::derived_from<T, Widget> && std::constructible_from<T, Args...>
    T* createWidget(Args&&... args)
    {
        _widgets.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return reinterpret_cast<T*>(_widgets.back().get());
    }

private:
    Widget* widgetAtPosition(int x, int y) const;
    bool processUiEvent(const SDL_Event& e);

    bool _alive = true;
    Renderer _renderer;

    std::vector<std::unique_ptr<Widget>> _widgets;
    Widget* _focusedWidget = nullptr;
    Widget* _pressedWidget = nullptr;
};

} // namespace gx
