//
// Created by notoraptor on 17-12-04.
//

#include <cigmar/gui/gui.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::gui {

	namespace primitive {
		Color Color::white = {255, 255, 255};
		Color Color::black = {0, 0, 0};
		Color Color::transparent = {0, 0, 0, 0};
	}

	Window& Widget::window() const {
		return ancestor ? ancestor->window() : dynamic_cast<Window&>(this);
	}

	Widget::Widget(): ancestor(nullptr), descendants(), surface() {}

	void Widget::setParent(Widget *newAncestor) {
		if (ancestor)
			ancestor->descendants.remove(this);
		if (newAncestor)
			newAncestor->descendants.add(this);
		ancestor = newAncestor;
	}

	size_t Widget::width() const {
		return surface.width;
	}

	size_t Widget::height() const {
		return surface.height;
	}

	const Widget *Widget::parent() const {
		return ancestor;
	}

	Widget::~Widget() {
		setParent(nullptr);
		ArrayList<Widget*> copyOfDescendants(descendants);
		for (Widget* widget: copyOfDescendants)
			delete widget;
	}

	void Widget::addChild(Widget *child) {
		if (child) child->setParent(this);
	}

	size_t Widget::countChildren() const {
		return descendants.size();
	}

	Widget* Widget::getChild(size_t pos) {
		return descendants[pos];
	}

	const Widget* Widget::getChild(size_t pos) const {
		return descendants[pos];
	}

	void Widget::reserveChildren(size_t size) {
		if (size < descendants.size())
			throw Exception("Forbidden to reserve less space for children than current space allowed. Please remov extra-children before.");
		if (size > descendants.size())
			descendants.reserve(size);
		for (size_t i = size; i < descendants.size(); ++i)
			descendants[i] = nullptr;
	}

	void Widget::removeChild(size_t pos) {
		if (pos < descendants.size()) {
			Widget* ptr = descendants[pos];
			ptr->setParent(nullptr);
		}
	}

	void Widget::setChild(size_t pos, Widget *child) {
		if (pos < descendants.size()) {
			Widget* ptr = descendants[pos];
			if (ptr)
				ptr->setParent(nullptr);
			if (child) {
				child->setParent(nullptr);
				descendants[pos] = child;
				child->ancestor = this;
			}
		}
	}

	void Window::draw(Coordinate origin, size_t width, size_t height) {
		// TODO: OPTIMIZE: For a windows, surface.position should always be (0;0).
		surface.position = origin;
		surface.width = width;
		surface.height = height;
		handler->drawSurface(surface);
		Widget* wchild = child();
		if (wchild)
			wchild->draw(origin, width, height);
	}

	void Window::display() {
		handler->clear(primitive::Color::black);
		draw({0, 0}, width(), height());
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
						onMouseScroll(event.mouse.scroll.vertical, event.mouse.scroll.delta, event.mouse.x, event.mouse.y);
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
						onKeyDown(event.code.code, event.code.alt, event.code.ctrl, event.code.shift, event.code.system);
						break;
					case Event::Type::KEY_UP:
						onKeyUp(event.code.code, event.code.alt, event.code.ctrl, event.code.shift, event.code.system);
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

	Window::Window(backend::WindowHandler* windowHandler, const WindowProperties& windowProperties):
		Widget(), properties(), handler(windowHandler), focus(nullptr) {
		if (!handler)
			throw Exception("a Window needs an handler to be used.");
		properties = handler->init(windowProperties);
		if (windowProperties.required && windowProperties != properties)
			throw Exception("Cannot initialize the window handler with the required properties.", ENDL, windowProperties);
		surface.width = properties.resolution.width;
		surface.height = properties.resolution.height;
		setParent(nullptr);
	}

	void Window::close() {
		if (!handler->close())
			throw Exception("Unable to close application. Internal handler error.");
		focus = nullptr;
	}

	Window::~Window() {
		focus = nullptr;
		delete handler;
	}

	Window& Window::setChild(Widget *widget) {
		if (countChildren()) removeChild(0);
		addChild(widget);
		return *this;
	}

	Widget* Window::child() {
		return countChildren() ? getChild(0) : nullptr;
	}

	const Widget *Window::child() const {
		return countChildren() ? getChild(0) : nullptr;
	}

	bool Window::isOpen() const {
		return handler->isOpen();
	}

	BorderLayout::BorderLayout(): Layout(), internalPadding() {
		reserveChildren(COUNT);
	}

	BorderLayout &BorderLayout::setTop(Widget *widget) {
		setChild(TOP, widget);
		return *this;
	}

	BorderLayout &BorderLayout::setBottom(Widget *widget) {
		setChild(BOTTOM, widget);
		return *this;
	}

	BorderLayout &BorderLayout::setLeft(Widget *widget) {
		setChild(LEFT, widget);
		return *this;
	}

	BorderLayout &BorderLayout::setRight(Widget *widget) {
		setChild(RIGHT, widget);
		return *this;
	}
}
