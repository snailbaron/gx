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

    virtual void update(double delta) {}
    virtual void render(Renderer& renderer) const = 0;
    virtual bool contains(int /*x*/, int /*y*/) const { return false; }

    State state() const { return _state; }
    void state(State s) { onStateChange(_state, s); _state = s; }

    bool enabled() const { return _state != State::Disabled; }
    bool disabled() const { return _state == State::Disabled; }

    virtual void onStateChange(State source, State target) {}
    virtual void onActivate() {}

private:
    State _state = State::Normal;
};

class Ui {
public:
    Ui(const ScreenSize& areaSize);

    template <class T, class... Args>
    requires std::derived_from<T, Widget> && std::constructible_from<T, Args...>
    T& createWidget(Args&&... args)
    {
        return _widgets.emplace_back(std::forward<Args>(args)...);
    }

    template <std::derived_from<Widget> T>
    T& addWidget(T&& widget)
    {
        _widgets.push_back(std::forward<T>(widget));
        return _widgets.back();
    }

    bool processEvent(const SDL_Event& e);

private:
    Widget* widgetAtPosition(int x, int y) const;

    ScreenSize _areaSize;
    std::vector<std::unique_ptr<Widget>> _widgets;
    Widget* _focusedWidget = nullptr;
    Widget* _pressedWidget = nullptr;
};

class Button : public Widget {
public:
    Button(const UiPoint& position, const UiSize& size);

    void render(Renderer& window) const override;

private:
    UiPoint _position;
    UiSize _size;
    State _state = State::Normal;
};

} // namespace gx
