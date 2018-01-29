// We must internally use only utf-8 strings.

#include <functional>
#include <cigmar/tree.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/Queue.hpp>
#include <cigmar/gui/backend.hpp>

namespace cigmar::gui {

	class Disposition {
		enum class DispositionType {LEFT, RIGHT, JUSTIFY, CENTER, TOP, BOTTOM};
		explicit Disposition(DispositionType type): disposition(type) {}
	protected:
		DispositionType disposition;
	public:
		bool horizontal() const {return disposition <= DispositionType::CENTER;}
		bool vertical() const {return disposition >= DispositionType::CENTER;}
		bool operator==(const Disposition& other) const {return disposition == other.disposition;}
		static Disposition top;
		static Disposition left;
		static Disposition bottom;
		static Disposition right;
		static Disposition center;
		static Disposition justify;
	};

	template<typename T>
	struct Directions {
		T top, left, bottom, right;
	};

	class TextBlock {
		Disposition orientation; // Should be horizontal.
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
		Size fixed;
		primitive::Surface surface; // current size.
		bool is_visible;
		bool is_transparent;
	public:
		SizeState minimize = {false, false};
		Widget(): Node(), fixed{0, 0}, surface(), is_visible(true), is_transparent(false) {
			Size minSize = min();
			surface.position.x = surface.position.y = 0;
			surface.width = minSize.width;
			surface.height = minSize.height;
		}
		bool visible() const {return is_visible;}
		bool transparent() const {return is_transparent;}
		void setVisible(bool value) {
			is_visible = value;
		}
		virtual void setTransparent(bool value) {
			is_transparent = value;
		}
		virtual Size min() const {
			return {1, 1};
		}
		virtual Size max() const {
			return Size::max;
		}
		/** Current (last-drawed) width. **/
		virtual size_t width() const {
			return fixed.width ? fixed.width : surface.width;
		};
		/** Current (last-drawed) height. **/
		virtual size_t height() const {
			return fixed.height ? fixed.height : surface.height;
		};
		/** Current (last-drawed) position. **/
		virtual const Coordinate& position() const {
			return surface.position;
		}
		virtual Size request(size_t width, size_t height) const {
			Size current, minSize = min(), maxSize = max();
			// Setting width.
			if (fixed.width)
				current.width = fixed.width;
			else if (minimize.width)
				current.width = minSize.width;
			else {
				current.width = width;
				if (current.width < minSize.width)
					current.width = minSize.width;
				else if (current.width > maxSize.width)
					current.width = maxSize.width;
			}
			// Setting height.
			if (fixed.height)
				current.height = fixed.height;
			else if (minimize.height)
				current.height = minSize.height;
			else {
				current.height = height;
				if (current.height < minSize.height)
					current.height = minSize.height;
				else if (current.height > maxSize.height)
					current.height = maxSize.height;
			}
			return current;
		}
		/** To reset width, call setWidth(0); **/
		virtual void setWidth(size_t newWidth) {
			fixed.width = newWidth;
		}
		/** To reset height, call setHeight(0); **/
		virtual void setHeight(size_t newHeight) {
			fixed.height = newHeight;
		}
		virtual void setPosition(const Coordinate& newPosition) {
			surface.position = newPosition;
		}
		virtual void update(size_t width, size_t height) {
			Size requested = request(width, height);
			surface.width = requested.width;
			surface.height = requested.height;
		}
		virtual void draw(Drawer& drawer) {
			if (!transparent()) {
				drawer.drawSurface(surface);
				for (Widget *node: *this) if (node) node->draw(drawer);
			}
		}
		bool contains(const Coordinate& point) const {
			return point.x >= surface.position.x && point.x < surface.position.x + width()
				   && point.y >= surface.position.y && point.y < surface.position.y + height();
		}
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
		// layout->add(myWidget->setPosition(newPosition));
	};

	struct Floater: public Layout {
		Disposition disposition;
		Floater(Disposition placement, Widget* widget): Layout(), disposition(placement) {
			add(widget);
		}
		size_t min_children() const override {return 1;}
		size_t max_children() const override {return 1;}
		Widget* child() {return Layout::child(0);}
		const Widget* child() const {return Layout::child(0);}
		static Floater* top(Widget* widget) {return new Floater(Disposition::top, widget);}
		static Floater* left(Widget* widget) {return new Floater(Disposition::left, widget);}
		static Floater* bottom(Widget* widget) {return new Floater(Disposition::bottom, widget);}
		static Floater* right(Widget* widget) {return new Floater(Disposition::right, widget);}
		static Floater* center(Widget* widget) {return new Floater(Disposition::center, widget);}
		static Floater* justify(Widget* widget) {return new Floater(Disposition::justify, widget);}
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

	class DirectedLayout: public Layout {
		Disposition defaultAlignment;
		ArrayList<Disposition> alignments;
		double percent;
	public:
		virtual void check(const Disposition& alignment) const {}
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
		void align(size_t position, const Disposition& alignment) {
			if (position < alignments.size()) {
				check(alignment);
				alignments[position] = alignment;
			}
		}
		void align(const Disposition& alignment) {
			check(alignment);
			for (Disposition& disposition: alignments) disposition = alignment;
		}
		void setDefaultAlignment(const Disposition& alignment) {
			check(alignment);
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
		const Disposition& getDefaultAlignment() const {
			return defaultAlignment;
		}
	};

	struct HorizontalLayout: public DirectedLayout {
		void check(const Disposition& alignment) const override {
			if (!alignment.vertical())
				throw Exception("Horizontal layout: expected vertical alignment.");
		}
	};

	struct VerticalLayout: public DirectedLayout {
		void check(const Disposition& alignment) const override {
			if (!alignment.horizontal())
				throw Exception("Vertical layout: expected horizontal alignment.");
		}
		Size request(size_t width, size_t height) const override {
			Size realSize{0, 0};
			size_t allowedHeight = (size_t)(height * getPercent());
			for (Widget* node: *this) {
				if (node) {
					bool heightPacked = node->packHeight;
					node->packHeight = true;
					Size nodeSize = node->request(width, allowedHeight);
					if (realSize.width < nodeSize.width)
						realSize.width = nodeSize.width;
					realSize.height += nodeSize.height;
					node->packHeight = heightPacked;
				}
			}
			return realSize;
		}
		void draw(Drawer& drawer, const Coordinate& origin, size_t width, size_t height) override {
			Coordinate childPosition = origin;
			// TODO ...
		}
	};

	// full ?
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
		void setWidth(size_t newWidth) override {
			Widget::setWidth(newWidth);
			handler->resize({width(), height()});
		}
		void setHeight(size_t newHeight) override {
			Widget::setHeight(newHeight);
			handler->resize({width(), height()});
		}
		void setPosition(const Coordinate& newPosition) override {
			// Widget::setPosition(newPosition);
			handler->setPosition(position());
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
