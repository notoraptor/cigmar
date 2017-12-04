// We must internally only use utf-8 strings.

#include <cstddef>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/HashMap.hpp>

namespace cigmar::gui {

	namespace backend {
		class Font;
	}

	/// Enumerations MouseButton and KeyCode are inspired from SFML 2.4.2
	enum class MouseButton { UNKNOWN = -1, LEFT = 0, RIGHT, MIDDLE, EXTRA1, EXTRA2, COUNT };
	enum class KeyCode {
		UNKNOWN = -1,
		A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
		ESCAPE,
		CONTROL_LEFT, SHIFT_LEFT, ALT_LEFT, SYSTEM_LEFT,
		CONTROL_RIGHT, SHIFT_RIGHT, ALT_RIGHT, SYSTEM_RIGHT,
		MENU,
		BRACKET_LEFT, BRACKET_RIGHT,
		SEMICOLON, COMMA, PERIOD, QUOTE, SLASH, BACKSLASH, TILDE, EQUAL, DASH, SPACE, RETURN, BACKSPACE, TAB,
		PAGEUP, PAGEDOWN, END, HOME, INSERT, DELETE, ADD, SUBTRACT, MULTIPLY, DIVIDE, LEFT, RIGHT, UP, DOWN,
		NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
		PAUSE,
		COUNT
	};
	enum class VerticalPosition { TOP, CENTER, BOTTOM };
	enum class HorizontalPosition { LEFT, CENTER, RIGHT, JUSTIFY };

	class Surface;

	template<typename Type, Type min, Type max>
	struct Angle {
		Type value;
	};

	template<typename T>
	struct Directions {
		T top;
		T left;
		T bottom;
		T right;
	};

	template<typename T, typename U>
	struct Occurrences<T, U> {
		HashSet <T> values;
		ArrayList <U> usages;
	};

	struct Coordinate {
		size_t x;
		size_t y;
	};

	namespace primitive {
		struct Background {
			enum class Type {CANVAS, COLOR, GRADIENT, MOTIF};
			Type type;
		};
	}

	struct Color : public primitive::Background {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha; // 0: transparent, 255: opaque
	};

	namespace primitive {

		struct Size {
			size_t width;
			size_t height;
		};

		struct Outline {
			size_t width;
			Color color;
		};

		struct Rectangle;

		struct Primitive {
			Outline outline;
			Background* background;
			virtual Size getSize() const = 0;
			virtual Rectangle getBox() const = 0;
			virtual void paint(Surface* surface, const Coordinate& topLeft) = 0;
		};

		struct Ellipse: public Primitive {
			size_t hradius;
			size_t vradius;
			Coordinate center;
		};

		struct Rectangle: public Primitive {
			size_t width;
			size_t height;
		};

		class Coordinates: public Primitive {
			/* List of coordinates to define a geometric form.
			 * When drawn, the coordinates are transposed to a space
			 * where the reference point if the top-left vertex of the smallest
			 * rectangle surrounding these coordinates. */
			ArrayList<Coordinate> coordinates;
		public:
			virtual void paint(Surface* surface, const ArrayList<Coordinate>& normalized) = 0;
			void normalize(ArrayList<Coordinate>& out, const Coordinate& topLeft = {0, 0});
			void paint(Surface* surface, const Coordinate& topLeft) override {
				ArrayList<Coordinates> normalized;
				normalize(normalized, topLeft);
				paint(surface,normalized);
			}
		};

		struct Polygon: public Coordinates {};

		struct RegularPolygon: public Coordinates {
			RegularPolygon(size_t radius, size_t n_vertices);
		};

		struct PolyLine: public Coordinates {
			/// Connected series of line segments.
		};

		struct Bezier: public Coordinates {};

		struct Segment: public Coordinates {
			// Only 2 coordinates allowed.
		};

		struct Line: public Segment {};

		struct WindowHandler {
			void drawPoints(const Coordinates& vertices);
			void drawSegments(const Coordinates& vertices);
			void drawPolygon(const Coordinates& vertices);
			void drawEllipse(const Ellipse& ellipse);
			/// NB: To draw a Bezier curve, we could calculate coordinates ourselves
			/// and use either drawPoints() or drawSegments().
		};

	}

	struct Font {
	public:
		backend::Font* handler;
		static HashMap<String, backend::Font*> loadedFonts;
		static Font* get(const String& font_name);

		struct Format {
			enum class Type { FONT, SIZE, COLOR, BACKGROUND, ITALIC, BOLD, UNDERLINE, STRIKE, COUNT };
			union {
				Font* font;
				size_t size;
				Color color;
				Color background;
				bool italic;
				bool bold;
				bool underline;
				bool strike;
			} value;
			Type type;

			struct Position {
				Font::Format* format;
				size_t start;
				size_t length;
			};
		};
	};

	struct HrefPosition {
		String* href;
		size_t start;
		size_t length;
	};

	class TextBlock;

	class Paragraph : public TextBlock {
		UnicodeString content;
		Occurrences<Font::Format, Font::Format::Position> formats;
		Occurrences<String, HrefPosition> hrefs;
		HorizontalPosition orientation;
	};

	class List : public TextBlock {
		ArrayList<TextBlock*> entries;
	};

	class OrderedList : public List {};

	struct Event {
		enum class Type {
			CLOSED, RESIZED, FOCUS_IN, FOCUS_OUT,
			MOUSE_SCROLL, MOUSE_DOWN, MOUSE_UP, MOUSE_MOVED, MOUSE_IN, MOUSE_OUT,
			TEXT, KEY_DOWN, KEY_UP,
			COUNT
		};
		union {
			struct {
				size_t width;
				size_t height;
			} size;
			struct {
				KeyCode code;
				bool alt;
				bool ctrl;
				bool shift;
				bool system;
			} code;
			struct {
				int x;
				int y;
				union {
					struct {
						bool vertical;
						float delta;
					} scroll;
					MouseButton button;
				};
			} mouse;
			uint32_t unicode;
		};
		Type type;
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

	struct GradientPoint {
		size_t distance;
		Color color;
	};

	class ColorGradient : public primitive::Background {
		Color first;
		ArrayList <GradientPoint> nextPoints;
		Angle<float, 0, 180> rotationFromLeft;
	};

	struct Image {
		size_t n_channels;
		size_t width;
		size_t height;
		byte_t *data;

		struct View {
			Image *image;
			size_t x;
			size_t y;
			size_t width;
			size_t height;
		};
	};

	class Motif : public primitive::Background {
		Image::View imageView;
		bool hrepeat;
		bool vrepeat;
	};

	class Canvas : public primitive::Background {
		// A canvas is always defined wrt/ the surrounding rectangle
		// of the primitive for which it's a background.
		enum class Scaling {FIXED, FORCED, SCALED};
		ArrayList<primitive::Primitive*> primitives;
		ArrayList<Coordinate> coordinates;
		Scaling scaling;
	public:
		size_t size() const ;
	};

	struct Border {
		size_t width;
		Background *background;
	};

	struct Surface {
		size_t width;
		size_t height;
		Background *background;
	};

	struct Text {
		ArrayList <TextBlock> content;
	};

	class Widget : public EventHandler {
		Widget *parent;
		ArrayList<Widget *> children;
		Surface surface;
	};

	class StyledWidget : public Widget {
		Directions<Border> border;
		Directions<size_t> padding;
		Directions<size_t> margin;
	};

	class Window : public Widget {
		// root element: parent is always nullptr
		Widget *focus;
	};

	class Division : public StyledWidget { // leaf widget
		// children is always empty
		Text text;
	};

	class Layout : public StyledWidget {
	}; // internal node widget

	class HorizontalLayout : public Layout {
		ArrayList <VerticalPosition> internal_alignments;
		VerticalPosition global_internal_alignment;
	};

	class VerticalLayout : public Layout {
		ArrayList <HorizontalPosition> internal_alignments;
		HorizontalPosition global_internal_alignment;
	};

	class BorderLayout : public Layout {
		// children is a constant-size vector of 5 widgets.
		// children[0]: top
		// children[0]: left
		// children[0]: bottom
		// children[0]: right
		// children[0]: center
		bool top_on_left;
		bool top_on_right;
		bool bottom_on_left;
		bool bottom_on_right;
		Directions<size_t> internal_padding;
	};

	class FreeLayout : public Layout {
		ArrayList <Coordinate> coordinates;
	};

}
