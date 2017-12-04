//
// Created by notoraptor on 17-12-04.
//

#include <cigmar/gui/gui.hpp>

namespace cigmar::gui {

	Window& Widget::window() const {
		if (parentWidget) return parentWidget->window();
		return dynamic_cast<Window&>(this);
	}

	void Window::draw(Coordinate origin, size_t width, size_t height) override {
		// TODO: OPTIMIZE: For a windows, surface.position should always be (0;0).
		surface.position = origin;
		surface.width = width;
		surface.height = height;
		handler->drawSurface(surface);
		Widget* child = getChild();
		if (child)
			child->draw(origin, width, height);
	}

	void Window::display() {
		handler->clear(primitive::Color::black);
		draw({0, 0}, getWidth(), getHeight());
		handler->display();
	};

	int Window::show() {
		while (isOpen()) {
			Event event;
			while (handler->pollEvent(event)) {
				switch (event.type) {
					case Event::Type::CLOSED:
						if (onClosed())
							close();
						break;
					case Event::Type::RESIZED:
						onResized(event.size.width, event.size.height);
						break;
					case Event::Type::FOCUS_IN:
						// TODO:
						// Window should determine which widget has the focus,
						// set the focus pointer to this widget, and call widget->onFocusIn().
						onFocusIn();
						break;
					case Event::Type::FOCUS_OUT:
						// TODO:
						// Window should call focus->onFocusOut() if focus is valid.
						onFocusOut();
						break;
					case Event::Type::MOUSE_SCROLL:
						onMouseScroll(event.mouse.scroll.vertical, event.mouse.scroll.delta, event.mouse.x,
									  event.mouse.y);
						break;
					case Event::Type::MOUSE_DOWN:
						onMouseDown(event.mouse.button, event.mouse.x, event.mouse.y);
						break;
					case Event::Type::MOUSE_UP:
						onMouseUp(event.mouse.button, event.mouse.x, event.mouse.y);
						break;
					case Event::Type::MOUSE_MOVED:
						onMouseMoved(event.mouse.x, event.mouse.y);
						break;
					case Event::Type::MOUSE_IN:
						onMouseIn();
						break;
					case Event::Type::MOUSE_OUT:
						onMouseOut();
						break;
					case Event::Type::TEXT:
						onText(event.unicode);
						break;
					case Event::Type::KEY_DOWN:
						onKeyDown(event.code.code, event.code.alt, event.code.ctrl, event.code.shift,
								  event.code.system);
						break;
					case Event::Type::KEY_UP:
						onKeyUp(event.code.code, event.code.alt, event.code.ctrl, event.code.shift,
								event.code.system);
						break;
					case Event::Type::UNKNOWN:
						// Unknown event. What to do ?
						break;
				}
			}
			display();
		}
		return 0;
	}

}
