//
// Created by notoraptor on 17-12-04.
//

#ifndef SRC_CIGMAR_UTILS_HPP
#define SRC_CIGMAR_UTILS_HPP

#include <cstddef>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/HashSet.hpp>

namespace cigmar::gui {

	template<typename T, typename U>
	struct Occurrences {
		HashSet <T> values;
		ArrayList <U> usages;
	};

	template<typename T>
	struct TextPosition {
		T* element;
		size_t start;
		size_t length;
	};

	struct Size {
		size_t width;
		size_t height;
		static const Size max;
	};

	struct Coordinate {
		int x;
		int y;
	};

	struct Image {
		enum class Depth {GRAY = 1, RGB = 3, RGBA = 4};
		Depth depth; // Number of channels.
		size_t width;
		size_t height;
		char* data;

		struct View: public Size, public Coordinate {
			Image* image = nullptr;
		};
	};

	struct WindowProperties: public Streamable {
		size_t width;
		size_t height;
		size_t bitsPerPixels;
		bool resizable;
		bool closeable;
		bool fullscreen;
		bool decored;
		bool titled;
		bool required; // True if values defined for properties above are mandatory, for any reason.

		bool operator==(const WindowProperties& o) const {
			return (width == o.width
					&& height == o.height
					&& bitsPerPixels == o.bitsPerPixels
					&& resizable == o.resizable
					&& closeable == o.closeable
					&& fullscreen == o.fullscreen
					&& decored == o.decored
					&& titled == o.titled
			        && required == o.required);
		}
		bool operator!=(const WindowProperties& o) const {
			return !this->operator==(o);
		}
		void toStream(std::ostream& o) const override {
			// TODO
			o << "WindowProperties(" << width << ", " << height << ", " << bitsPerPixels;
			if (resizable) o << ", resizable";
			if (closeable) o << ", closeable";
			if (fullscreen) o << ", fullscreen";
			if (decored) o << ", decored";
			if (titled) o << ", titled";
			if (required) o << ", required";
			o << ")";
		}
	};

	/// Enumerations MouseButton and KeyCode are inspired from SFML 2.4.2
	enum class MouseButton { UNKNOWN = -1, LEFT = 0, RIGHT, MIDDLE, EXTRA1, EXTRA2, COUNT };
	enum class KeyCode {
		UNKNOWN = -1,
		A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
		NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
		ADD, SUBTRACT, MULTIPLY, DIVIDE, ESCAPE, MENU, PAUSE, BRACKET_LEFT, BRACKET_RIGHT,
		CONTROL_LEFT, SHIFT_LEFT, ALT_LEFT, SYSTEM_LEFT, CONTROL_RIGHT, SHIFT_RIGHT, ALT_RIGHT, SYSTEM_RIGHT,
		TAB, SEMICOLON, COMMA, PERIOD, QUOTE, SLASH, BACKSLASH, TILDE, EQUAL, DASH, SPACE,
		RETURN, BACKSPACE, INSERT, DELETE, PAGEUP, PAGEDOWN, END, HOME,  LEFT, RIGHT, UP, DOWN,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
		COUNT
	};

	struct Event {
		enum class Type {
			UNKNOWN = -1, CLOSED = 0, RESIZED, FOCUS_IN, FOCUS_OUT,
			MOUSE_SCROLL, MOUSE_DOWN, MOUSE_UP, MOUSE_MOVED, MOUSE_IN, MOUSE_OUT, TEXT, KEY_DOWN, KEY_UP,
			COUNT
		};
		union {
			Size size;
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
		Type type = Type::UNKNOWN;
	};

}

#endif //SRC_CIGMAR_UTILS_HPP
