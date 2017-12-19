// We must internally use only utf-8 strings.

#include <cstddef>
#include <functional>
#include <cigmar/tree.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/gui/utils.hpp>
#include <cigmar/gui/backend.hpp>
#include <cigmar/gui/utils.hpp>
#include <cigmar/classes/Queue.hpp>

namespace cigmar::gui {

	enum class VerticalPosition { TOP, CENTER, BOTTOM };
	enum class HorizontalPosition { LEFT, CENTER, RIGHT, JUSTIFY };

	template<typename T>
	struct Directions {
		T top, left, bottom, right;
	};

	class TextBlock {
		HorizontalPosition orientation;
	};

	class Paragraph : public TextBlock {
		UnicodeString content;
		primitive::TextFormat format;
		primitive::TextUrlMapping urls;
	};

	class List : public TextBlock {
		ArrayList<TextBlock*> entries;
	};

	class OrderedList : public List {};

	struct Text {
		ArrayList<TextBlock*> content;
	};

	class EventHandler {
		// full
	public:
		typedef std::function<bool()> default_handler_t;
		typedef std::function<bool(size_t, size_t)> size_handler_t;
		typedef std::function<bool(uint32_t)> unicode_handler_t;
		typedef std::function<bool(KeyCode, bool, bool, bool, bool)> key_handler_t;
		typedef std::function<bool(bool, float, int, int)> mouse_scroll_handler_t;
		typedef std::function<bool(MouseButton, int, int)> mouse_button_handler_t;
		typedef std::function<bool(int x, int y)> mouse_handler_t;
	private:
		default_handler_t onClosedHandler;
		size_handler_t onResizedHandler;
		default_handler_t onFocusInHandler;
		default_handler_t onFocusOutHandler;
		default_handler_t onWindowFocusInHandler;
		default_handler_t onWindowFocusOutHandler;
		mouse_scroll_handler_t onMouseScrollHandler;
		mouse_button_handler_t onMouseDownHandler;
		mouse_button_handler_t onMouseUpHandler;
		mouse_handler_t onMouseMovedHandler;
		default_handler_t onMouseInHandler;
		default_handler_t onMouseOutHandler;
		default_handler_t onWindowMouseInHandler;
		default_handler_t onWindowMouseOutHandler;
		unicode_handler_t onTextHandler;
		key_handler_t onKeyDownHandler;
		key_handler_t onKeyUpHandler;
	public:
		// Setters / cleaners (clean by passing nullptr).
		void setOnClosed(const default_handler_t& h) {onClosedHandler = h;};
		void setOnResized(const size_handler_t& h) {onResizedHandler = h;};
		void setOnFocusIn(const default_handler_t& h) {onFocusInHandler = h;};
		void setOnFocusOut(const default_handler_t& h) {onFocusOutHandler = h;};
		void setOnWindowFocusIn(const default_handler_t& h) {onWindowFocusInHandler = h;};
		void setOnWindowFocusOut(const default_handler_t& h) {onWindowFocusOutHandler = h;};
		void setOnMouseScroll(const mouse_scroll_handler_t& h) {onMouseScrollHandler = h;};
		void setOnMouseDown(const mouse_button_handler_t& h) {onMouseDownHandler = h;};
		void setOnMouseUp(const mouse_button_handler_t& h) {onMouseUpHandler = h;};
		void setOnMouseMoved(const mouse_handler_t& h) {onMouseMovedHandler = h;};
		void setOnMouseIn(const default_handler_t& h) {onMouseInHandler = h;};
		void setOnMouseOut(const default_handler_t& h) {onMouseOutHandler = h;};
		void setOnWindowMouseIn(const default_handler_t& h) {onWindowMouseInHandler = h;};
		void setOnWindowMouseOut(const default_handler_t& h) {onWindowMouseOutHandler = h;};
		void setOnText(const unicode_handler_t& h) {onTextHandler = h;};
		void setOnKeyDown(const key_handler_t& h) {onKeyDownHandler = h;};
		void setOnKeyUp(const key_handler_t& h) {onKeyDownHandler = h;};
		// Virtual executors before.
		virtual bool onClosedBefore() {return true;};
		virtual bool onResizedBefore(size_t width, size_t height) {return true;};
		virtual bool onFocusInBefore() {return true;};
		virtual bool onFocusOutBefore() {return true;};
		virtual bool onWindowFocusInBefore() {return true;};
		virtual bool onWindowFocusOutBefore() {return true;};
		virtual bool onMouseScrollBefore(bool vertical, float delta, int x, int y) {return true;};
		virtual bool onMouseDownBefore(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseUpBefore(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseMovedBefore(int x, int y) {return true;};
		virtual bool onMouseInBefore() {return true;};
		virtual bool onMouseOutBefore() {return true;};
		virtual bool onWindowMouseInBefore() {return true;};
		virtual bool onWindowMouseOutBefore() {return true;};
		virtual bool onTextBefore(uint32_t unicode) {return true;};
		virtual bool onKeyDownBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		virtual bool onKeyUpBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		// Virtual executors after.
		virtual bool onClosedAfter() {return true;};
		virtual bool onResizedAfter(size_t width, size_t height) {return true;};
		virtual bool onFocusInAfter() {return true;};
		virtual bool onFocusOutAfter() {return true;};
		virtual bool onWindowFocusInAfter() {return true;};
		virtual bool onWindowFocusOutAfter() {return true;};
		virtual bool onMouseScrollAfter(bool vertical, float delta, int x, int y) {return true;};
		virtual bool onMouseDownAfter(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseUpAfter(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseMovedAfter(int x, int y) {return true;};
		virtual bool onMouseInAfter() {return true;};
		virtual bool onMouseOutAfter() {return true;};
		virtual bool onWindowMouseInAfter() {return true;};
		virtual bool onWindowMouseOutAfter() {return true;};
		virtual bool onTextAfter(uint32_t unicode) {return true;};
		virtual bool onKeyDownAfter(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		virtual bool onKeyUpAfter(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		// Executors.
		bool onClosed() {
			return onClosedBefore() && (onClosedHandler ? onClosedHandler() : true) && onClosedAfter();
		};
		bool onResized(size_t width, size_t height) {
			return onResizedBefore(width, height)
			       && (onResizedHandler? onResizedHandler(width, height) : true)
			       && onResizedAfter(width, height);
		};
		bool onFocusIn() {
			return onFocusInBefore() && (onFocusInHandler ? onFocusInHandler() : true) && onFocusInAfter();
		};
		bool onFocusOut() {
			return onFocusOutBefore() && (onFocusOutHandler ? onFocusOutHandler() : true) && onFocusOutAfter();
		};
		bool onWindowFocusIn() {
			return onWindowFocusInBefore() && (onWindowFocusInHandler ? onWindowFocusInHandler() : true) && onWindowFocusInAfter();
		};
		bool onWindowFocusOut() {
			return onWindowFocusOutBefore() && (onWindowFocusOutHandler ? onWindowFocusOutHandler() : true) && onWindowFocusOutAfter();
		};
		bool onMouseScroll(bool vertical, float delta, int x, int y) {
			return onMouseScrollBefore(vertical, delta, x, y)
			       && (onMouseScrollHandler ? onMouseScrollHandler(vertical, delta, x, y) : true)
			       && onMouseScrollAfter(vertical, delta, x, y);
		};
		bool onMouseDown(MouseButton button, int x, int y) {
			return onMouseDownBefore(button, x, y)
			       && (onMouseDownHandler ? onMouseDownHandler(button, x, y) : true)
			       && onMouseDownAfter(button, x, y);
		};
		bool onMouseUp(MouseButton button, int x, int y) {
			return onMouseUpBefore(button, x, y)
			       && (onMouseUpHandler ? onMouseUpHandler(button, x, y) : true)
			       && onMouseUpAfter(button, x, y);
		};
		bool onMouseMoved(int x, int y) {
			return onMouseMovedBefore(x, y)
			       && (onMouseMovedHandler ? onMouseMovedHandler(x, y) : true)
			       && onMouseMovedAfter(x, y);
		};
		bool onMouseIn() {
			return onMouseInBefore() && (onMouseInHandler ? onMouseInHandler() : true) && onMouseInAfter();
		};
		bool onMouseOut() {
			return onMouseOutBefore() && (onMouseOutHandler ? onMouseOutHandler() : true) && onMouseOutAfter();
		};
		bool onWindowMouseIn() {
			return onWindowMouseInBefore() && (onWindowMouseInHandler ? onWindowMouseInHandler() : true) && onWindowMouseInAfter();
		};
		bool onWindowMouseOut() {
			return onWindowMouseOutBefore() && (onWindowMouseOutHandler ? onWindowMouseOutHandler() : true) && onWindowMouseOutAfter();
		};
		bool onText(uint32_t unicode) {
			return onTextBefore(unicode) && (onTextHandler ? onTextHandler(unicode) : true) && onTextAfter(unicode);
		};
		bool onKeyDown(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {
			return onKeyDownBefore(code, alt, ctrl, shift, system)
			       && (onKeyDownHandler? onKeyDownHandler(code, alt, ctrl, shift, system) : true)
			       && onKeyDownAfter(code, alt, ctrl, shift, system);
		};
		bool onKeyUp(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {
			return onKeyUpBefore(code, alt, ctrl, shift, system)
			       && (onKeyUpHandler? onKeyUpHandler(code, alt, ctrl, shift, system) : true)
			       && onKeyUpAfter(code, alt, ctrl, shift, system);
		};
	};

	class Window;

	class Widget: public tree::Content<Widget, Window, Widget, Widget>, public EventHandler {
	protected:
		primitive::Surface surface;
	public:
		typedef root_t window_t;
		typedef node_t widget_t;
		bool visible;
		bool transparent;
		virtual void primarize(Coordinate origin, size_t width, size_t height, Queue<primitive::Primitive*>& queue) {
			surface.position = origin;
			surface.width = width;
			surface.height = height;
			queue << &surface;
		};
		virtual size_t width() const {
			return surface.width;
		};
		virtual size_t height() const {
			return surface.height;
		};
		virtual const Coordinate& position() const {
			return surface.position;
		}
		bool contains(const Coordinate& point) const {
			return point.x >= surface.position.x && point.x < surface.position.x + surface.width
				&& point.y >= surface.position.y && point.y < surface.position.y + surface.height;
		}
		widget_t position(Coordinate newPosition) {
			surface.position = newPosition;
			return node();
		}
		bool onClosedAfter() override {
			bool close = true;
			for (child_t& node: *this) if (node) close *= node->onClosed();
			return close;
		}
		bool onResizedBefore(size_t width, size_t height) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onResized(width, height);
			return ok;
		}
		bool onFocusInBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onFocusIn();
			return ok;
		}
		bool onFocusOutBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onFocusOut();
			return ok;
		}
		bool onWindowFocusInBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onWindowFocusIn();
			return ok;
		}
		bool onWindowFocusOutBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onWindowFocusOut();
			return ok;
		}
		bool onMouseScrollBefore(bool vertical, float delta, int x, int y) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseScroll(vertical, delta, x, y);
			return ok;
		}
		bool onMouseDownBefore(MouseButton button, int x, int y) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseDown(button, x, y);
			return ok;
		}
		bool onMouseUpBefore(MouseButton button, int x, int y) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseUp(button, x, y);
			return ok;
		}
		bool onMouseMovedBefore(int x, int y) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseMoved(x, y);
			return ok;
		}
		bool onMouseInBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseIn();
			return ok;
		}
		bool onMouseOutBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onMouseOut();
			return ok;
		}
		bool onWindowMouseInBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onWindowMouseIn();
			return ok;
		}
		bool onWindowMouseOutBefore() override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onWindowMouseOut();
			return ok;
		}
		bool onTextBefore(uint32_t unicode) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onText(unicode);
			return ok;
		}
		bool onKeyDownBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onKeyDown(code, alt, ctrl, shift, system);
			return ok;
		}
		bool onKeyUpBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) override {
			bool ok = true;
			for (child_t& node: *this) if (node) ok *= node->onKeyUp(code, alt, ctrl, shift, system);
			return ok;
		}
	};

	class StyledWidget : public Widget {
		Directions<primitive::Border> border;
		Directions<size_t> padding; // internal space between borders and the children
		Directions<size_t> margin; // external space between borders and the world
	};

	class Division : public StyledWidget { // leaf widget
		Text text;
	};

	class Layout : public StyledWidget {
		// Free layout.
		// Exemple: set position of a widget and add this widget in one line:
		// layout->add(myWidget->position(newPosition));
	};

	struct BorderLayout : public Layout {
		// children is a constant-size vector of 5 widgets.
		enum {TOP, LEFT, BOTTOM, RIGHT, CENTER, COUNT};
		bool topOnLeft = true, topOnRight = true, bottomOnLeft = true, bottomOnRight = true;
		Directions<size_t> internalPadding;
		size_t minChildren() const override { return COUNT; }
		size_t maxChildren() const override { return COUNT; }
		bool preallocate() const override { return true; }
		BorderLayout& setTop(widget_t widget);
		BorderLayout& setBottom(widget_t widget);
		BorderLayout& setLeft(widget_t widget);
		BorderLayout& setRight(widget_t widget);
		BorderLayout& setCenter(widget_t widget);
		widget_t top() const;
		widget_t bottom() const;
		widget_t left() const;
		widget_t right() const;
		widget_t center() const;
	};

	template <typename PositionType>
	class DirectedLayout: public Layout {
		ArrayList<PositionType> internalAlignments;
		PositionType defaultInternalAlignment;
	};

	class HorizontalLayout: public DirectedLayout<VerticalPosition> {};

	class VerticalLayout: public DirectedLayout<HorizontalPosition> {};

	// full ?
	class Window: public Layout {
		// 2 children: the context menu and the windows content.
		enum {CONTEXT, CONTENT, COUNT};
		WindowProperties properties;
		backend::WindowHandler* handler;
		widget_t focus;
	public:
		Window(backend::WindowHandler* windowHandler, const WindowProperties& windowProperties):
				Layout(), properties(), handler(windowHandler), focus(nullptr) {
			if (!handler)
				throw Exception("Window: a handler is required.");
			properties = handler->open(windowProperties);
			if (windowProperties.required && properties != windowProperties) {
				String message = "Window: the handler has not created the window with required properties.";
				message << ENDL;
				message << "Required properties: " << windowProperties << ENDL;
				message << "Returned properties: " << properties << ENDL;
				throw Exception(message);
			}
			surface.width = properties.width;
			surface.height = properties.height;
		};
		Window(backend::WindowHandler& windowHandler, const WindowProperties& windowProperties):
				Window(&windowHandler, windowProperties) {};
		~Window() {
			close();
		}
		void setContent(const widget_t& widget) {
			setChild(CONTENT, widget);
		};
		widget_t content() {
			return child(CONTENT);
		};
		bool isOpen() const {
			return handler->isOpen();
		};
		void close() {
			if (handler->isOpen())
				handler->close();
		};
		void collectPrimitives(Queue<primitive::Primitive*>& primitives, widget_t node) {
			// TODO ...
			if (node) {
				node->primarize(surface.position, surface.width, surface.height, primitives);
				for (widget_t& nodeChild: node)
					collectPrimitives(primitives, nodeChild);
			}
		}
		void draw() {
			Queue<primitive::Primitive*> primitives;
			primitives << &surface;
			for (widget_t node: *this)
				collectPrimitives(primitives, node);
			for (primitive::Primitive* primitivePointer: primitives) {
				// TODO: draw.
			}
		};
		void setFocus(widget_t& widget) {
			if (widget->root() != window_t(*this))
				throw Exception("Window: cannot focus on a non-descendant.");
			if (focus)
				focus->onFocusOut();
			focus = widget;
			focus->onFocusIn();
		}
		int show() {
			// The display loop is here.
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
							onFocusIn();
							break;
						case Event::Type::FOCUS_OUT:
							onFocusOut();
							break;
						case Event::Type::MOUSE_SCROLL:
							onMouseScroll(event.mouse.scroll.vertical,
							              event.mouse.scroll.delta,
							              event.mouse.x, event.mouse.y);
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
							onKeyDown(event.code.code, event.code.alt, event.code.ctrl,
							          event.code.shift, event.code.system);
							break;
						case Event::Type::KEY_UP:
							onKeyUp(event.code.code, event.code.alt, event.code.ctrl,
							        event.code.shift, event.code.system);
							break;
						case Event::Type::UNKNOWN:
						default:
							break;
					}
					// Start display.
					handler->clear(primitive::Color::white);
					draw();
					handler->display();
					// End display.
				}
			}
		}
		bool onResizedBefore(size_t width, size_t height) override {
			surface.width = width;
			surface.height = height;
			return Widget::onResizedBefore(width, height);
		}
		bool onFocusInBefore() override {
			return Widget::onWindowFocusInBefore();
		}
		bool onFocusOutBefore() override {
			return Widget::onWindowFocusOutBefore();
		}
		bool onMouseInBefore() override {
			return Widget::onWindowMouseInBefore();
		}
		bool onMouseOutBefore() override {
			return Widget::onWindowMouseOutBefore();
		}
		// TODO: When/where are emitted MouseIn/MouseOut event for widgets ?

		size_t minChildren() const override {return COUNT;}
		size_t maxChildren() const override {return COUNT;}
		bool forceRoot() const override {return true;}
		bool preallocate() const override {return true;}
	};

}
