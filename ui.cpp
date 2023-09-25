#include <gx/ui.hpp>

namespace gx {

Ui::Ui(const ScreenSize& areaSize)
    : _areaSize(areaSize)
{ }

bool Ui::processEvent(const SDL_Event& e)
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

Widget* Ui::widgetAtPosition(int x, int y) const
{
    for (const auto& widget : _widgets) {
        if (widget->enabled() && widget->contains(x, y)) {
            return widget.get();
        }
    }
    return nullptr;
}

void Button::render(Renderer& window) const
{
}

} // namespace gx
