// We must internally only use utf-8 strings.
// Here, String is a ASCII-String, UnicodeString is a utf-8 string.

#include <cstddef>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/String.hpp>

namespace cigmar::gui {

	class Background;
	class TextBlock;
	typedef std::basic_string<uint32_t> UnicodeString;
	class MouseButton {
	public:
		bool left() const;
		bool right() const;
		bool middle() const;
		bool extra1() const;
		bool extra2() const;
	};
	class KeyCode {}; // TODO

	template<typename Type, Type min, Type max>
	struct Angle {
		Type value;
	};

	enum class VerticalPosition {
		TOP, CENTER, BOTTOM
	};

	enum class HorizontalPosition {
		LEFT, CENTER, RIGHT, JUSTIFY
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

	struct Interval {
		size_t start;
		size_t length;
	};

	struct Color : public Background {
		ubyte_t red;
		ubyte_t green;
		ubyte_t blue;
	};

	class Font { // TODO
	public:
		static Font *get(const String &font_name);
	};

	struct Format {
		enum class Type {
			FONT, SIZE, COLOR, BACKGROUND, ITALIC, BOLD, UNDERLINE, STRIKE
		};
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
	};

	struct FormatPosition {
		Format *format;
		size_t start;
		size_t length;
	};

	struct HrefPosition {
		String *href;
		size_t start;
		size_t length;
	};

	class Paragraph : public TextBlock {
		UnicodeString content;
		Occurrences<Format, Interval> formats;
		Occurrences<String, Interval> hrefs;
		HorizontalPosition orientation;
	};

	class List : public TextBlock {
		ArrayList<TextBlock*> entries;
	};

	class OrderedList : public List {};

	struct EventType {
		// window; other informed
		bool closed() const;

		// widget
		bool resized() const;
		bool focusIn() const;
		bool focusOut() const;
		bool mouseScroll() const;
		bool mouseDown() const;
		bool mouseUp() const;
		bool mouseMoved() const;
		bool mouseIn() const;
		bool mouseOut() const;

		// windows and object focused
		bool text() const;
		bool keyDown() const;
		bool keyUp() const;
		/// Maybe need other methods to access events specific properties ?
	};

	struct EventHandler {
		typedef std::function<bool()> default_handler_t;
		typedef std::function<bool(size_t, size_t)> size_handler_t;
		typedef std::function<bool(uint32_t)> unicode_handler_t;
		typedef std::function<bool(KeyCode, bool, bool, bool, bool)> key_handler_t;
		typedef std::function<bool(bool, float, int, int)> mouse_scroll_handler_t;
		typedef std::function<bool(MouseButton, int, int)> mouse_button_handler_t;
		typedef std::function<bool(int x, int y)> mouse_handler_t;

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

	class ColorGradient : public Background {
		Color first;
		ArrayList <GradientPoint> nextPoints;
		Angle<float, 0, 180> rotationFromLeft;
	};

	class Image {
		size_t n_channels;
		size_t width;
		size_t height;
		byte_t *data;
	};

	class ImageView {
		Image *image;
		size_t x;
		size_t y;
		size_t width;
		size_t height;
	};

	class Motif : public Background {
		ImageView imageView;
		bool repeat_horizontal;
		bool repeat_vertical;
	};

	class Canvas : public Background {
		virtual void paint(Surface *) = 0;
		// canvas.paint(surface) is called at every frame generation on the associated object.
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
