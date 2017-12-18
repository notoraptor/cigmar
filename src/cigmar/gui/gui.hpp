// We must internally use only utf-8 strings.

#include <cstddef>
#include <cigmar/tree.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/gui/utils.hpp>
#include <cigmar/gui/backend.hpp>
#include "utils.hpp"

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
		mouse_scroll_handler_t onMouseScrollHandler;
		mouse_button_handler_t onMouseDownHandler;
		mouse_button_handler_t onMouseUpHandler;
		mouse_handler_t onMouseMovedHandler;
		default_handler_t onMouseInHandler;
		default_handler_t onMouseOutHandler;
		unicode_handler_t onTextHandler;
		key_handler_t onKeyDownHandler;
		key_handler_t onKeyUpHandler;
	public:
		// Setters / cleaners (clean by passing nullptr).
		void setOnClosed(const default_handler_t& h) {onClosedHandler = h;};
		void setOnResized(const size_handler_t& h) {onResizedHandler = h;};
		void setOnFocusIn(const default_handler_t& h) {onFocusInHandler = h;};
		void setOnFocusOut(const default_handler_t& h) {onFocusOutHandler = h;};
		void setOnMouseScroll(const mouse_scroll_handler_t& h) {onMouseScrollHandler = h;};
		void setOnMouseDown(const mouse_button_handler_t& h) {onMouseDownHandler = h;};
		void setOnMouseUp(const mouse_button_handler_t& h) {onMouseUpHandler = h;};
		void setOnMouseMoved(const mouse_handler_t& h) {onMouseMovedHandler = h;};
		void setOnMouseIn(const default_handler_t& h) {onMouseInHandler = h;};
		void setOnMouseOut(const default_handler_t& h) {onMouseOutHandler = h;};
		void setOnText(const unicode_handler_t& h) {onTextHandler = h;};
		void setOnKeyDown(const key_handler_t& h) {onKeyDownHandler = h;};
		void setOnKeyUp(const key_handler_t& h) {onKeyDownHandler = h;};
		// Virtual executors before.
		virtual bool onClosedBefore() {return true;};
		virtual bool onResizedBefore(size_t width, size_t height) {return true;};
		virtual bool onFocusInBefore() {return true;};
		virtual bool onFocusOutBefore() {return true;};
		virtual bool onMouseScrollBefore(bool vertical, float delta, int x, int y) {return true;};
		virtual bool onMouseDownBefore(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseUpBefore(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseMovedBefore(int x, int y) {return true;};
		virtual bool onMouseInBefore() {return true;};
		virtual bool onMouseOutBefore() {return true;};
		virtual bool onTextBefore(uint32_t unicode) {return true;};
		virtual bool onKeyDownBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		virtual bool onKeyUpBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {return true;};
		// Virtual executors after.
		virtual bool onClosedAfter() {return true;};
		virtual bool onResizedAfter(size_t width, size_t height) {return true;};
		virtual bool onFocusInAfter() {return true;};
		virtual bool onFocusOutAfter() {return true;};
		virtual bool onMouseScrollAfter(bool vertical, float delta, int x, int y) {return true;};
		virtual bool onMouseDownAfter(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseUpAfter(MouseButton button, int x, int y) {return true;};
		virtual bool onMouseMovedAfter(int x, int y) {return true;};
		virtual bool onMouseInAfter() {return true;};
		virtual bool onMouseOutAfter() {return true;};
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
		virtual void draw(Coordinate origin, size_t width, size_t height) = 0;
		virtual size_t width() const;
		virtual size_t height() const;
		const Coordinate& position() const {
			return surface.position;
		}
		widget_t position(Coordinate newPosition) {
			surface.position = newPosition;
			return node();
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

	template <typename T>
	class valid_iterator_t {
		T it, it_end;
	public:
		valid_iterator_t(const T& iterator, const T& end): it(iterator), it_end(end) {
			this->operator++();
		}
		bool operator==(const valid_iterator_t& other) const {return it == other.it;}
		bool operator!=(const valid_iterator_t& other) const {return it != other.it;}
		valid_iterator_t& operator++() {
			while (it != it_end && !(*it))
				++it;
			return *this;
		}
		auto operator*() -> decltype(T::operator*()) {
			return it.operator*();
		}
		auto operator->() -> decltype(T::operator->()) {
			return it.operator->();
		}
	};

	template <typename T>
	class valid_t {
		T& o;
		typedef valid_iterator_t<decltype(T::begin())> iterator_t;
		valid_t(T& object): o(object) {}
		iterator_t begin() {return iterator_t(o.begin(), o.end());}
		iterator_t end() {return iterator_t(o.end(), o.end());}
	};

	template <typename T>
	class const_valid_t {
		typedef const T const_t;
		const_t& o;
		typedef valid_iterator_t<decltype(const_t::begin())> const_iterator_t;
		const_valid_t(const_t& object): o(object) {}
		const_iterator_t begin() const {return const_iterator_t(o.begin(), o.end());}
		const_iterator_t end() const {return const_iterator_t(o.end(), o.end());}
	};

	// full ?
	class Window: public Layout {
		// 2 children: the context menu and the windows content.
		enum {CONTEXT, CONTENT, COUNT};
		WindowProperties properties;
		backend::WindowHandler* handler;
		Widget* focus;
	protected:
		void display() {
			handler->clear(primitive::Color::white);
			draw({0, 0}, width(), height());
			handler->display();
		}
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
		void draw(Coordinate origin, size_t width, size_t height) override {
			surface.position = origin;
			surface.width = width;
			surface.height = height;
			handler->drawSurface(surface);
			widget_t child = content();
			if (child)
				child->draw(origin, width, height);
		};
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
					display();
				}
			}
		}
		bool onClosedBefore() override {
			bool close = true;
			if (child(CONTEXT)) close = child(CONTEXT)->onClosed();
			if (close && child(CONTENT)) close = child(CONTENT)->onClosed();
			return close;
		}
		bool onResizedBefore(size_t width, size_t height) override {
			properties.width = width;
			properties.height = height;
			// todo: pass to children.
			return true;
		}
		size_t width() const override {
			return properties.width;
		}
		size_t height() const override {
			return properties.height;
		}
		size_t minChildren() const override {return COUNT;}
		size_t maxChildren() const override {return COUNT;}
		bool forceRoot() const override {return true;}
		bool preallocate() const override {return true;}
	};

}
