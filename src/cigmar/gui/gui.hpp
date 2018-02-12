// We must internally use only utf-8 strings.
/** NB/TODO
 * Let's just work with basic rectangles and temporarly forget complex forms.
 * **/
#include <functional>
#include <cigmar/tree.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/Queue.hpp>
#include <cigmar/gui/backend.hpp>

namespace cigmar::gui {

	template <typename T> inline T clip(T value, T min, T max) {
		return value < min ? min : (value > max ? max : value);
	}

	template <typename T> inline T select(T value, T min, T max, T fixed, bool minimize) {
		return fixed ? fixed : (minimize ? min : clip(value, min, max));
	}

	namespace Orientation {
		enum class Horizontal {LEFT, CENTER, RIGHT};
		enum class Vertical {TOP, CENTER, BOTTOM};
		enum class Text {LEFT, CENTER, RIGHT, JUSTIFY};
	}

	template<typename T>
	struct Directions {
		T top, left, bottom, right;
		void operator=(const T& value) {
			top = left = bottom = right = value;
		}
	};

	class TextBlock {
		Orientation::Text orientation;
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

	struct EventHandler {
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

	class Widget: public tree::Node<Widget, Window>, public EventHandler {
		primitive::Surface surface; // current size.
		Directions<size_t> padding;
		Size fixed;
	public:
		bool visible;
		bool transparent;
		SizeState minimize;
		Widget(): Node(), surface(), padding(), fixed{0, 0},
				  visible(true), transparent(false), minimize{false, false} {
			Size minSize = min();
			surface.position.x = surface.position.y = 0;
			surface.width = minSize.width;
			surface.height = minSize.height;
			padding = 0;
		}
		virtual Size min() const {
			return {1, 1};
		}
		virtual Size max() const {
			return Size::max;
		}
		virtual Size request(size_t width, size_t height) const {
			Size minSize = min(), maxSize = max();
			return {
				select(width, minSize.width, maxSize.width, fixed.width, minimize.width),
				select(height, minSize.height, maxSize.height, fixed.height, minimize.height)
			};
		}
		virtual Size draw(Drawer& drawer) {
			Size drawn = {0, 0};
			if (visible) {
				if(!transparent)
					drawer.drawSurface(surface);
				drawn = surface.box();
			}
			return drawn;
		}
		virtual Size drawInterface(Drawer& drawer) {
			Size drawn = draw(drawer);
			for (Widget* node: *this) if (node) node->draw(drawer);
			return drawn;
		}
		const Coordinate& position() const {
			return surface.position;
		}
		size_t width() const {
			return fixed.width ? fixed.width : surface.width;
		};
		size_t height() const {
			return fixed.height ? fixed.height : surface.height;
		};
		Size box() const {
			return surface.box();
		}
		primitive::Border& border() {
			return surface.border;
		}
		primitive::Background& background() {
			return surface.background;
		}
		const primitive::Border& border() const {
			return surface.border;
		}
		const primitive::Background& background() const {
			return surface.background;
		}
		void setPosition(const Coordinate& newPosition) {
			/** Position of the top-left space occupied by object
			 * (including padding, border and margin). **/
			surface.position = newPosition;
		}
		void setWidth(size_t newWidth) {
			fixed.width = newWidth;
		}
		void setHeight(size_t newHeight) {
			fixed.height = newHeight;
		}
		void resetWidth() {
			fixed.width = 0;
		}
		void resetHeight() {
			fixed.height = 0;
		}
		void update(size_t width, size_t height) {
			Size requested = request(width, height);
			surface.width = requested.width;
			surface.height = requested.height;
		}
		bool contains(const Coordinate& point) const {
			return point.x >= surface.position.x && point.x < surface.position.x + width()
				   && point.y >= surface.position.y && point.y < surface.position.y + height();
		}
		//////////////////////////////////////////////////
		bool onClosedAfter() override {
			bool close = true;
			for (Widget* node: *this) if (node) close *= node->onClosed();
			return close;
		}
		bool onResizedBefore(size_t width, size_t height) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onResized(width, height);
			return ok;
		}
		bool onFocusInBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onFocusIn();
			return ok;
		}
		bool onFocusOutBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onFocusOut();
			return ok;
		}
		bool onWindowFocusInBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onWindowFocusIn();
			return ok;
		}
		bool onWindowFocusOutBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onWindowFocusOut();
			return ok;
		}
		bool onMouseScrollBefore(bool vertical, float delta, int x, int y) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseScroll(vertical, delta, x, y);
			return ok;
		}
		bool onMouseDownBefore(MouseButton button, int x, int y) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseDown(button, x, y);
			return ok;
		}
		bool onMouseUpBefore(MouseButton button, int x, int y) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseUp(button, x, y);
			return ok;
		}
		bool onMouseMovedBefore(int x, int y) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseMoved(x, y);
			return ok;
		}
		bool onMouseInBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseIn();
			return ok;
		}
		bool onMouseOutBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onMouseOut();
			return ok;
		}
		bool onWindowMouseInBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onWindowMouseIn();
			return ok;
		}
		bool onWindowMouseOutBefore() override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onWindowMouseOut();
			return ok;
		}
		bool onTextBefore(uint32_t unicode) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onText(unicode);
			return ok;
		}
		bool onKeyDownBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onKeyDown(code, alt, ctrl, shift, system);
			return ok;
		}
		bool onKeyUpBefore(KeyCode code, bool alt, bool ctrl, bool shift, bool system) override {
			bool ok = true;
			for (Widget* node: *this) if (node) ok *= node->onKeyUp(code, alt, ctrl, shift, system);
			return ok;
		}
	};

	class Division : public Widget {
		Text text;
		// leaf widget.
		size_t max_children() const override {return 0;}
	};

	class Layout : public Widget {
		// Free layout.
		// Exemple: set position of a widget and add this widget in one line:
		// layout->add(myWidget->setPosition(newPosition));
	};

	struct BorderLayout : public Layout {
		// children is a constant-size vector of 5 widgets.
		enum {TOP, LEFT, BOTTOM, RIGHT, CENTER, COUNT};
		bool topOnLeft = true, topOnRight = true, bottomOnLeft = true, bottomOnRight = true;
		Directions<size_t> internalPadding;
		size_t min_children() const override { return COUNT; }
		size_t max_children() const override { return COUNT; }
		size_t preallocate() const override { return COUNT; }
		BorderLayout& setTop(Widget* widget);
		BorderLayout& setBottom(Widget* widget);
		BorderLayout& setLeft(Widget* widget);
		BorderLayout& setRight(Widget* widget);
		BorderLayout& setCenter(Widget* widget);
		Widget* top() const;
		Widget* bottom() const;
		Widget* left() const;
		Widget* right() const;
		Widget* center() const;
	};

	template <typename OrientationType>
	class DirectedLayout: public Layout {
		OrientationType defaultAlignment;
		ArrayList<OrientationType> alignments;
		double percent;
	public:
		void with_node_added(Widget* node, size_t position) override {
			if (position == alignments.size())
				alignments.add(defaultAlignment);
			else
				alignments.insert(position, defaultAlignment);
		}
		void with_node_replaced(Widget* old_node, Widget* new_node, size_t position) override {
			alignments[position] = defaultAlignment;
		}
		void with_node_removed(Widget* node, size_t position) override {
			if (position < alignments.size())
				alignments.remove(position);
		}
		void align(size_t position, OrientationType alignment) {
			if (position < alignments.size()) {
				alignments[position] = alignment;
			}
		}
		void align(OrientationType alignment) {
			for (OrientationType disposition: alignments) disposition = alignment;
		}
		void setDefaultAlignment(OrientationType alignment) {
			defaultAlignment = alignment;
		}
		void setPercent(double newPercent) {
			if (newPercent <= 0 || newPercent > 100)
				throw Exception("A percent value must be > 0.0 and <= 100.0");
			percent = newPercent;
		}
		double getPercent() const {
			return percent;
		}
		OrientationType getDefaultAlignment() const {
			return defaultAlignment;
		}
	};

	struct HorizontalLayout: public DirectedLayout<Orientation::Horizontal > {};

	struct VerticalLayout: public DirectedLayout<Orientation::Vertical> {
		Size request(size_t width, size_t height) const override {
			Size realSize{0, 0};
			size_t allowedHeight = (size_t)(height * getPercent());
			for (Widget* node: *this) {
				if (node) {
					bool heightPacked = node->minimize.height;
					node->minimize.height = true;
					Size nodeSize = node->request(width, allowedHeight);
					if (realSize.width < nodeSize.width)
						realSize.width = nodeSize.width;
					realSize.height += nodeSize.height;
					node->minimize.height = heightPacked;
				}
			}
			return realSize;
		}
		void draw(Drawer& drawer, const Coordinate& origin, size_t width, size_t height) override {
			Coordinate childPosition = origin;
			// TODO ...
		}
	};

	class Window: public Layout {
		// 2 children: the context menu and the windows content.
		enum {CONTEXT, CONTENT, COUNT};
		WindowProperties properties;
		backend::WindowHandler* handler;
		Widget* focus;
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
			Widget::setWidth(properties.width);
			Widget::setHeight(properties.height);
			handler->setPosition(position());
			handler->resize({width(), height()});
			// TODO somewhere: handler->resize({width(), height()});
			// TODO somewhere: handler->setPosition(position());
		};
		Window(backend::WindowHandler& windowHandler, const WindowProperties& windowProperties):
				Window(&windowHandler, windowProperties) {};
		~Window() {
			if (isOpen()) close();
		}
		void setContent(Widget* widget) {
			setChild(CONTENT, widget);
		};
		const Widget* content() const {
			return child(CONTENT);
		};
		bool isOpen() const {
			return handler->isOpen();
		};
		void close() {
			if (handler->isOpen())
				handler->close();
		};
		void setFocus(Widget* widget) {
			if (widget) {
				if (widget->root() != this)
					throw Exception("Window: cannot focus on a non-descendant.");
				if (focus)
					focus->onFocusOut();
				focus = widget;
				focus->onFocusIn();
			} else {
				if (focus)
					focus->onFocusOut();
				focus = nullptr;
			}
		}
		void show() {
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
							onMouseScroll(event.mouse.scroll.vertical, event.mouse.scroll.delta,
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
					draw(*handler, position(), width(), height());
					handler->display();
					// End display.
				}
			}
		}
		bool onResizedBefore(size_t new_width, size_t new_height) override {
			Widget::setWidth(new_width);
			Widget::setHeight(new_height);
			handler->resize({width(), height()});
			return Widget::onResizedBefore(new_width, new_height);
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
		size_t min_children() const override {return COUNT;}
		size_t max_children() const override {return COUNT;}
		size_t preallocate() const override {return COUNT;}
		bool force_root() const override {return true;}
	};

}
