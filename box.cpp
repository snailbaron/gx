#include <gx/box.hpp>

#include <gx/error.hpp>

#include <string>

namespace gx {

Bitmap Box::loadBitmap(const std::filesystem::path& path) const
{
    return _renderer.loadBitmap(path);
}

Bitmap Box::loadBitmap(const std::span<const std::byte>& data) const
{
    return _renderer.loadBitmap(data);
}

void Box::update(double delta)
{
    for (const auto& widget : _widgets) {
        widget->update(delta);
    }
}

void Box::present()
{
    _renderer.clear();
    for (const auto& widget : _widgets) {
        widget->render(_renderer);
    }
    _renderer.present();
}

bool Box::alive() const
{
    return _alive;
}

bool Box::dead() const
{
    return !_alive;
}

bool Box::processEvent(const SDL_Event& e)
{
    if (e.type == SDL_QUIT) {
        _alive = false;
        return true;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        _alive = false;
        return true;
    }

    processUiEvent(e) || _renderer.processEvent(e);

    return false;
}

Widget* Box::widgetAtPosition(int x, int y) const
{
    for (const auto& widget : _widgets) {
        if (widget->enabled() && widget->contains(x, y)) {
            return widget.get();
        }
    }
    return nullptr;
}

bool Box::processUiEvent(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEMOTION) {
        auto* newFocusedWidget = widgetAtPosition(e.motion.x, e.motion.y);
        if (newFocusedWidget != _focusedWidget) {
            if (_focusedWidget) {
                if (_focusedWidget->state() == Widget::State::Focused) {
                    _focusedWidget->state(Widget::State::Normal);
                } else if (_focusedWidget->state() == Widget::State::Pressed) {
                    _focusedWidget->state(Widget::State::SlipPressed);
                }
            }
            _focusedWidget = newFocusedWidget;
            if (_focusedWidget) {
                if (_pressedWidget == nullptr) {
                    _focusedWidget->state(Widget::State::Focused);
                } else if (_pressedWidget == _focusedWidget) {
                    _focusedWidget->state(Widget::State::Pressed);
                }
            }
        }
        return (_pressedWidget != nullptr);
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (_focusedWidget) {
            _focusedWidget->state(Widget::State::Pressed);
            _pressedWidget = _focusedWidget;
            return true;
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (_pressedWidget) {
            if (_pressedWidget == _focusedWidget) {
                _pressedWidget->state(Widget::State::Focused);
                _pressedWidget->onActivate();
            } else if (_focusedWidget) {
                _focusedWidget->state(Widget::State::Focused);
            }
            _pressedWidget = nullptr;
            return true;
        }
    }

    return false;
}

} // namespace gx
