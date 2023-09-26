#pragma once

#include <gx/renderer.hpp>
#include <gx/ui_position.hpp>

#include <SDL.h>

#include <concepts>
#include <memory>
#include <utility>
#include <vector>

namespace gx {

class Widget {
public:
    enum class State {
        Disabled,    // widget does not react to input
        Normal,      // widget is ready for use
        Focused,     // widget is focused (e.g. mouse is hovering over it)
        Pressed,     // widget is pressed down
        SlipPressed, // widget was pressed, then unfocused (e.g mouse cursor
                     // left its area) while still being pressed down
    };

    virtual ~Widget() = default;

    virtual void update(float delta) {}
    virtual void render(Renderer& renderer) const = 0;
    virtual bool contains(int /*x*/, int /*y*/) const { return false; }

    State state() const { return _state; }
    void state(State s) { onStateChange(_state, s); _state = s; }

    bool enabled() const { return _state != State::Disabled; }
    bool disabled() const { return _state == State::Disabled; }

    virtual void onStateChange(State source, State target) {}
    virtual void onActivate() {}

private:
    State _state = State::Disabled;
};

class Button : public Widget {
public:
    Button(const ScreenPoint& position, const ScreenSize& size);

    void render(Renderer& window) const override;

private:
    ScreenPoint _position;
    ScreenSize _size;
    State _state = State::Normal;
};

} // namespace gx
