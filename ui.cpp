#include <gx/ui.hpp>

namespace gx {

void Ui::update(double delta)
{
    for (auto& widget : _widgets) {
        widget->update(delta);
    }
}

void Ui::render(Renderer& window) const
{
    for (const auto& widget : _widgets) {
        widget->render(window);
    }
}

void Button::render(Renderer& window) const
{
}

void Button::changeState(State state)
{
}

} // namespace gx
