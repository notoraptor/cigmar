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
	};

	struct Color : public Background {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha; // 0: transparent, 255: opaque
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

	struct Surface: public Primitive, public Size {};

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
		size_t width;
		size_t padding1;
		size_t padding2;
	};

	struct Font {
		const size_t id; // Internal (back-end) font ID, understandable by WindowHandler.

		struct Format {
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
	};

	typedef Occurrences<Font::Format, TextPosition<Font::Format>> TextFormat;
	typedef Occurrences<String, TextPosition<String>> TextUrlMapping;

}

#endif //SRC_CIGMAR_PRIMITIVE_HPP
