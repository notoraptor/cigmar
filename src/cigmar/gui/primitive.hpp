//
// Created by notoraptor on 17-12-04.
//

#ifndef SRC_CIGMAR_PRIMITIVE_HPP
#define SRC_CIGMAR_PRIMITIVE_HPP

#include <cstddef>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/gui/utils.hpp>

namespace cigmar::gui::primitive {

	struct Primitive;

	struct Background {
		enum class Type {CANVAS, COLOR, GRADIENT, MOTIF};

		Type type;
		explicit Background(Type backgroundType = Type::CANVAS) noexcept : type(backgroundType) {}
		virtual bool isTransparent() const = 0;
	};

	struct Color : public Background {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha; // 0: transparent, 255: opaque
		Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) noexcept :
			Background(Type::COLOR), red(r), green(g), blue(b), alpha(a) {}
		bool isTransparent() const override {return alpha == 0;}
		static Color white;
		static Color black;
		static Color transparent;
		// TODO: Define all standard HTML colors.

		class Gradient: public Background {
			ArrayList<Color> points;
			// double rotation;
		};
	};

	struct Motif: public Background {
		Image::View imageView;
		bool hrepeat;
		bool vrepeat;
	};

	struct Canvas : public Background {
		// A canvas is always defined wrt/ the bound box the primitive for which it's a background.
		enum class Scaling {NONE, SCALED, FIT};
		ArrayList<Primitive*> primitives;
		Scaling scaling;
	};

	struct Size {
		size_t width;
		size_t height;
	};

	struct Primitive {
		struct {
			size_t width;
			Color color;
		} outline;
		Background* background;	// should be nullptr for non-closed figures.
		Coordinate position;	// Position of the top-left point of the surrounding box.
		double rotation;
		virtual Size size() const {
			// TODO: Override in derived classes.
			return {0, 0};
		};
		/** Return size of bound box when drawed (with rotation applied). **/
		Size space() const;
	};

	struct Ellipse: public Primitive {
		size_t hradius;
		size_t vradius;
	};

	struct Surface: public Primitive, public Size {};

	struct Coordinates: public Primitive {
		/* List of coordinates to define a geometric form.
		 * NB: Coordinates are internally always stored in a same space (to be defined).
		 * e.g. where the reference point is the top-left vertex of the bound box. */
		ArrayList<Coordinate> coordinates;
	};

	struct Polygon: public Coordinates {
		Polygon();
		// Constructor for regular polygon.
		Polygon(size_t radius, size_t nVertices);
	};

	struct Connection: public Coordinates {
		/*
		 * Connected series of line segments.
		 * e.g. (A, B, C, D) => connected segments [AB], [BC], [CD]
		 * (A, B) => only one segment [AB]
		 * */
		bool extended = false;
		/**< If true, first and last segment are extended as semi-lines.
		 * If connection contains only two points, then the segment is extended as a line. **/
	};

	struct Bezier: public Connection {};

	struct Border: public Polygon {
		// A border is a trapeze.
		enum class Type {TOP, LEFT, BOTTOM, RIGHT};
		Type type;
		size_t length;      // should be no public
		size_t width;       // thickness
		size_t padding1;    // top or left
		size_t padding2;    // bottom or right
	};

	struct Font {
		const size_t id; // Internal (back-end) font ID, understandable by WindowHandler only.
	};

	struct FontFormat {
		enum class Type { FONT, SIZE, COLOR, BACKGROUND, ITALIC, BOLD, UNDERLINE, STRIKE, COUNT };
		union {
			Font font;
			size_t size;
			primitive::Color color;
			primitive::Color background;
			bool italic;
			bool bold;
			bool underline;
			bool strike;
		} value;
		Type type;
	};

	typedef Occurrences<FontFormat, TextPosition<FontFormat>> TextFormat;
	typedef Occurrences<String, TextPosition<String>> TextUrlMapping;

}

#endif //SRC_CIGMAR_PRIMITIVE_HPP
