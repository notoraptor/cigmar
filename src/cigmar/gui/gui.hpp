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

namespace cigmar::gui {

	enum class VerticalPosition { TOP, CENTER, BOTTOM };
	enum class HorizontalPosition { LEFT, CENTER, RIGHT, JUSTIFY };

	template<typename T>
	struct Directions {
		T top, left, bottom, right;
	};

	template <typename Callback, typename... Args>
	class EventCallback {
		template <typename O>
		class ObjectMethodCaller {
			typedef typename Callback::result_type result_type;
			typedef result_type(O::*method_ptr_t)(Args);
			O* pointer;
			method_ptr_t methodPointer;
		public:
			ObjectMethodCaller(O* ptr, method_ptr_t m_ptr): pointer(ptr), methodPointer(m_ptr) {};
			result_type operator()(Args&&... args) {
				return (*pointer).*(methodPointer)(std::forward<Args>(args)...);
			}
		};
		Callback before;
		Callback callback;
		Callback after;
		void setBefore(Callback fn);
		void set(Callback fn);
		template<typename O, typename F> void set(O* o, F* f) {
			callback = ObjectMethodCaller(o, f);
		};
		void setAfter(Callback fn);
		void resetBefore();
		void reset();
		void resetAfter();
		bool operator()(Args&&... args);
	};

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
		// Setters.
		void setOnClosed(default_handler_t);
		void setOnResized(size_handler_t);
		void setOnFocusIn(default_handler_t);
		void setOnFocusOut(default_handler_t);
		void setOnMouseScroll(mouse_scroll_handler_t);
		void setOnMouseDown(mouse_button_handler_t);
		void setOnMouseUp(mouse_button_handler_t);
		void setOnMouseMoved(mouse_handler_t);
		void setOnMouseIn(default_handler_t);
		void setOnMouseOut(default_handler_t);
		void setOnText(unicode_handler_t);
		void setOnKeyDown(key_handler_t);
		void setOnKeyUp(key_handler_t);
		// Cleaners.
		void resetOnClosed();
		void resetOnResized();
		void resetOnFocusIn();
		void resetOnFocusOut();
		void resetOnMouseScroll();
		void resetOnMouseDown();
		void resetOnMouseUp();
		void resetOnMouseMoved();
		void resetOnMouseIn();
		void resetOnMouseOut();
		void resetOnText();
		void resetOnKeyDown();
		void resetOnKeyUp();
		// Executors.
		bool onClosed();
		bool onResized(size_t width, size_t height);
		bool onFocusIn();
		bool onFocusOut();
		bool onMouseScroll(bool vertical, float delta, int x, int y);
		bool onMouseDown(MouseButton button, int x, int y);
		bool onMouseUp(MouseButton button, int x, int y);
		bool onMouseMoved(int x, int y);
		bool onMouseIn();
		bool onMouseOut();
		bool onText(uint32_t unicode);
		bool onKeyDown(KeyCode code, bool alt, bool ctrl, bool shift, bool system);
		bool onKeyUp(KeyCode code, bool alt, bool ctrl, bool shift, bool system);
	};

	class TextBlock;

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

	class Widget : public EventHandler {
	protected:
		Widget* parentWidget;
		ArrayList<Widget*> children;
		primitive::Surface surface;
		virtual void draw(Coordinate origin, size_t width, size_t height) = 0;
	public:
		virtual bool accepts(size_t width, size_t height) = 0;
		virtual size_t getWidth() const = 0;
		virtual size_t getHeight() const = 0;
		Widget* parent() const { return parentWidget; };
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

	class Layout : public StyledWidget {
	public:
		size_t indexOf(Widget* child) const;
		void moveUp(size_t childPosition);
		void moveDown(size_t childPosition);
		void moveTop(size_t childPosition);
		void moveBottom(size_t childPosition);
		void switchPositions(size_t posA, size_t posB);
	}; // internal node widget

	class BorderLayout : public Layout {
		// children is a constant-size vector of 5 widgets.
		enum {TOP, LEFT, BOTTOM, RIGHT, CENTER, COUNT};
		bool topOnLeft, topOnRight, bottomOnLeft, bottomOnRight;
		Directions<size_t> internalPadding;
	};

	class FreeLayout : public Layout {
		ArrayList<Coordinate> coordinates;
	};

	template <typename PositionType>
	class DirectedLayout: public Layout {
		ArrayList<PositionType> internalAlignments;
		PositionType defaultInternalALignment;
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
		Window& setChild(Widget* widget);
		Widget* getChild();
		bool isOpen() const;
		void close(); // TODO: clean data, free widgets tree and close handler.
		int show(); // Display loop.
	};

}
