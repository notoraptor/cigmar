// We must internally only use utf-8 strings.

#include <cstddef>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/gui/utils.hpp>
#include <cigmar/gui/backend.hpp>
#include <cigmar/gui/utils.hpp>

/*
Notes.
Rectangle {
 	x, y, width, height
}
widget {
	Rectangle draw(x, y, width, height) {
 		// LEAF:
 		// Compute / set primitives
 		// Send primitives to window handler
 		// NODE:
 		// ::LEAF
 		// for each child: .. .draw(child, _x, _y, _width, _height) ...
 	};
}
parent {
 	Rectangle regionDrawed = draw(child, x, y, width, height)
}
*/

namespace cigmar::gui {

	enum class VerticalPosition { TOP, CENTER, BOTTOM };
	enum class HorizontalPosition { LEFT, CENTER, RIGHT, JUSTIFY };

	template<typename T>
	struct Directions {
		T top, left, bottom, right;
	};

	class TextBlock {};

	class Paragraph : public TextBlock {
		UnicodeString content;
		primitive::TextFormat format;
		primitive::TextUrlMapping hrefs;
		HorizontalPosition orientation;
	};

	class List : public TextBlock {
		ArrayList<TextBlock*> entries;
	};

	class OrderedList : public List {};

	struct Text {
		ArrayList<TextBlock*> content;
	};

	class Window;
	// full
	class EventHandler {
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
		void setOnClosed(default_handler_t h) {onClosedHandler = h;};
		void setOnResized(size_handler_t h) {onResizedHandler = h;};
		void setOnFocusIn(default_handler_t h) {onFocusInHandler = h;};
		void setOnFocusOut(default_handler_t h) {onFocusOutHandler = h;};
		void setOnMouseScroll(mouse_scroll_handler_t h) {onMouseScrollHandler = h;};
		void setOnMouseDown(mouse_button_handler_t h) {onMouseDownHandler = h;};
		void setOnMouseUp(mouse_button_handler_t h) {onMouseUpHandler = h;};
		void setOnMouseMoved(mouse_handler_t h) {onMouseMovedHandler = h;};
		void setOnMouseIn(default_handler_t h) {onMouseInHandler = h;};
		void setOnMouseOut(default_handler_t h) {onMouseOutHandler = h;};
		void setOnText(unicode_handler_t h) {onTextHandler = h;};
		void setOnKeyDown(key_handler_t h) {onKeyDownHandler = h;};
		void setOnKeyUp(key_handler_t h) {onKeyDownHandler = h;};
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
			return onResizedBefore(width, height) && (onResizedHandler? onResizedHandler(width, height) : true) && onResizedAfter(width, height);
		};
		bool onFocusIn() {
			return onFocusInBefore() && (onFocusInHandler ? onFocusInHandler() : true) && onFocusInAfter();
		};
		bool onFocusOut() {
			return onFocusOutBefore() && (onFocusOutHandler ? onFocusOutHandler() : true) && onFocusOutAfter();
		};
		bool onMouseScroll(bool vertical, float delta, int x, int y) {
			return onMouseScrollBefore(vertical, delta, x, y) && (onMouseScrollHandler ? onMouseScrollHandler(vertical, delta, x, y) : true) && onMouseScrollAfter(vertical, delta, x, y);
		};
		bool onMouseDown(MouseButton button, int x, int y) {
			return onMouseDownBefore(button, x, y) && (onMouseDownHandler ? onMouseDownHandler(button, x, y) : true) && onMouseDownAfter(button, x, y);
		};
		bool onMouseUp(MouseButton button, int x, int y) {
			return onMouseUpBefore(button, x, y) && (onMouseUpHandler ? onMouseUpHandler(button, x, y) : true) && onMouseUpAfter(button, x, y);
		};
		bool onMouseMoved(int x, int y) {
			return onMouseMovedBefore(x, y) && (onMouseMovedHandler ? onMouseMovedHandler(x, y) : true) && onMouseMovedAfter(x, y);
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
			return onKeyDownBefore(code, alt, ctrl, shift, system) && (onKeyDownHandler? onKeyDownHandler(code, alt, ctrl, shift, system) : true) && onKeyDownAfter(code, alt, ctrl, shift, system);
		};
		bool onKeyUp(KeyCode code, bool alt, bool ctrl, bool shift, bool system) {
			return onKeyUpBefore(code, alt, ctrl, shift, system) && (onKeyUpHandler? onKeyUpHandler(code, alt, ctrl, shift, system) : true) && onKeyUpAfter(code, alt, ctrl, shift, system);
		};
	};

	class Widget : public EventHandler {
	private:
		Widget* ancestor;
		ArrayList<Widget*> descendants;
	protected:
		primitive::Surface surface;
		void setParent(Widget* newAncestor);
		void reserveChildren(size_t size);
		void addChild(Widget* child);
		void setChild(size_t pos, Widget* child);
		void removeChild(size_t pos);
		size_t countChildren() const;
		Widget* getChild(size_t pos);
		const Widget* getChild(size_t pos) const;
		virtual void draw(Coordinate origin, size_t width, size_t height) = 0;
	public:
		Widget();
		virtual ~Widget();
		virtual bool accepts(size_t width, size_t height) const = 0; // TODO: useful ?
		size_t width() const;
		size_t height() const;
		const Widget* parent() const;
		Window& window() const;
	};

	class StyledWidget : public Widget {
		Directions<primitive::Border> border;
		Directions<size_t> padding; // internal space between borders and the children
		Directions<size_t> margin; // external space between borders and the world
	};

	class Division : public StyledWidget { // leaf widget
		// children is always empty, or cannot be extended after instanciation.
		Text text;
	};

	class Layout : public StyledWidget {};

	struct BorderLayout : public Layout {
		// children is a constant-size vector of 5 widgets.
		enum {TOP, LEFT, BOTTOM, RIGHT, CENTER, COUNT};
		bool topOnLeft = true, topOnRight = true, bottomOnLeft = true, bottomOnRight = true;
		Directions<size_t> internalPadding;
		BorderLayout();
		BorderLayout& setTop(Widget* widget);
		BorderLayout& setBottom(Widget* widget);
		BorderLayout& setLeft(Widget* widget);
		BorderLayout& setRight(Widget* widget);
	};

	struct StandardLayout: public Layout {
		void add(Widget* widget);
		void remove(Widget* widget);
		void remove(size_t pos);
		size_t indexOf(Widget* child) const;
		void moveUp(size_t childPosition);
		void moveDown(size_t childPosition);
		void moveTop(size_t childPosition);
		void moveBottom(size_t childPosition);
		void switchPositions(size_t posA, size_t posB);
	};

	class FreeLayout : public StandardLayout {
		ArrayList<Coordinate> coordinates;
	};

	template <typename PositionType>
	class DirectedLayout: public StandardLayout {
		ArrayList<PositionType> internalAlignments;
		PositionType defaultInternalALignment;
		void add(Widget* widget);
		void remove(Widget* widget);
		void remove(size_t pos);
		size_t indexOf(Widget* child) const;
		void moveUp(size_t childPosition);
		void moveDown(size_t childPosition);
		void moveTop(size_t childPosition);
		void moveBottom(size_t childPosition);
		void switchPositions(size_t posA, size_t posB);
	};

	class HorizontalLayout: public DirectedLayout<VerticalPosition> {};

	class VerticalLayout: public DirectedLayout<HorizontalPosition> {};

	class Window : public Widget {
		// root element: parent is always nullptr
		// window element: only 1 child allowed.
		WindowProperties properties;
		backend::WindowHandler* handler;
		Widget* focus;
	protected:
		void draw(Coordinate origin, size_t width, size_t height) override;
		void display();
	public:
		Window(backend::WindowHandler* windowHandler, const WindowProperties& windowProperties);
		~Window() override;
		Window& setChild(Widget* widget);
		Widget* child();
		const Widget* child() const;
		bool isOpen() const;
		void close();
		int show(); // Display loop.
	};

}
