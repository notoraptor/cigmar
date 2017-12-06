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
		virtual bool isTransparent() const = 0;
		explicit Background(Type backgroundType) noexcept : type(backgroundType) {}
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
	};

	class ColorGradient : public Background {
		struct Step {
			size_t distance;
			Color color;
		};
		ArrayList<Step> steps;
		double rotation;
	};

	class Motif : public Background {
		Image::View imageView;
		bool hrepeat;
		bool vrepeat;
	};

	class Canvas : public Background {
		// A canvas is always defined wrt/ the bound box the primitive for which it's a background.
		enum class Scaling {FIXED, FORCED, SCALED};
		ArrayList<Primitive*> primitives;
		ArrayList<Coordinate> coordinates;
		Scaling scaling;
	public:
		size_t size() const ;
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
		virtual Size getBoxSize() const = 0;
	};

	struct Ellipse: public Primitive {
		size_t hradius;
		size_t vradius;
	};

	struct Surface: public Primitive, public Size {
		Size getBoxSize() const override {
			return {width, height};
		}
	};

	class Coordinates: public Primitive {
		/* List of coordinates to define a geometric form.
		 * NB: Coordinates are internally always stored in a same space (to be defined).
		 * e.g. where the reference point is the top-left vertex of the bound box. */
		ArrayList<Coordinate> coordinates;
	protected:
		void set(const ArrayList<Coordinate>& points);
		const ArrayList<Coordinate>& get() const;
	};

	struct Polygon: public Coordinates {};

	struct RegularPolygon: public Coordinates {
		RegularPolygon(size_t radius, size_t n_vertices);
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

	struct Bezier: public Coordinates {};

	struct Border: public Polygon {
		// A border is a "regular" trapeze.
		enum class Type {TOP, LEFT, BOTTOM, RIGHT};
		Type type;
		size_t length; // should be no public
		size_t width; // thickness
		size_t padding1;
		size_t padding2;
		Size getBoxSize() const override {
			size_t boxWidth = 0, boxHeight = 0;
			switch (type) {
				case Type::TOP:
				case Type::BOTTOM:
					boxWidth = length;
					boxHeight = width;
					break;
				case Type::LEFT:
				case Type::RIGHT:
					boxWidth = width;
					boxHeight = length;
					break;
			}
			return {boxWidth, boxHeight};
		}
	};

	struct Font {
		const size_t id; // Internal (back-end) font ID, understandable by WindowHandler.
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
