#pragma once

#include <gx/ui_position.hpp>
#include <gx/window.hpp>

#include <memory>
#include <vector>

namespace gx {

class Widget {
public:
    enum class State {
        Normal,
        Disabled,
        Focused,
        Pressed,

    };

    virtual ~Widget() = 0;

    virtual void update(double delta) {}
    virtual void render(Renderer& window) const = 0;

    virtual void changeState(State state) {}
};

class Ui {
public:
    void update(double delta);
    void render(Renderer& window) const;

private:
    std::vector<std::unique_ptr<Widget>> _widgets;
};

class Button : public Widget {
public:
    Button(const UiPoint& position, const UiSize& size);

    void render(Renderer& window) const override;
    void changeState(State state) override;

private:
    UiPoint _position;
    UiSize _size;
    State _state = State::Normal;
};

} // namespace gx
