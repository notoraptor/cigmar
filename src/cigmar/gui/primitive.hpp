//
// Created by notoraptor on 17-12-04.
//
#ifndef SRC_CIGMAR_PRIMITIVE_HPP
#define SRC_CIGMAR_PRIMITIVE_HPP

#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/gui/utils.hpp>

namespace cigmar::gui::primitive {

	struct Primitive;

	struct Background {
		enum class Type {CANVAS, COLOR, GRADIENT, MOTIF};
		const Type type;
		explicit Background(Type backgroundType) noexcept : type(backgroundType) {}
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

		struct Gradient: public Background {
			ArrayList<Color> points;
			Gradient(): Background(Type::GRADIENT), points() {}
			Gradient(std::initializer_list<Color> il): Background(Type::GRADIENT), points(il) {}
			Gradient(const ArrayList<Color>& gradPoints): Background(Type::GRADIENT), points(gradPoints) {}
			Gradient(ArrayList<Color>&& gradPoints) noexcept : Background(Type::GRADIENT), points(std::move(gradPoints)) {}
		};
	};

	struct Motif: public Background {
		Image::View view;
		bool hrepeat = false;
		bool vrepeat = false;
		Motif(): Background(Type::MOTIF) {}
		Motif(Image* image, const Coordinate& from, const Size& size, bool hRepeat, bool vRepeat): Motif() {
			view.image = image;
			view.x = from.x;
			view.y = from.y;
			view.width = size.width;
			view.height = size.height;
			hrepeat = hRepeat;
			vrepeat = vRepeat;
		}
	};

	struct Canvas : public Background {
		// A canvas is always defined wrt/ the bound box the primitive for which it's a background.
		enum class Scaling {NONE, SCALED, FIT};
		Scaling scaling;
		ArrayList<Primitive*> primitives;
		Canvas(): Background(Type::CANVAS), scaling(Scaling::NONE), primitives() {}
		explicit Canvas(Scaling canvasScaling): Background(Type::CANVAS), scaling(canvasScaling), primitives() {}
	};

	struct Outline {
		size_t width;
		Color color;
	};

	struct Border: public Outline {};

	struct Primitive {
		Outline outline;
		Background* background;	// should be nullptr for non-closed figures.
		Coordinate position;	// Position of the top-left point of the surrounding box.
		/** Must return width and height of the primitive box (smallest rectangle surrounding the primitive).
		 * NB; Should take outline into account. **/
		virtual Size size() const = 0;
	};

	struct Ellipse: public Primitive {
		size_t hradius;
		size_t vradius;
		Size size() const override {
			return {2 * (hradius + outline.width), 2 * (vradius + outline.width)};
		}
	};

	struct Surface: public Primitive, public Size {
		Size size() const override {
			return {width + 2 * outline.width, height + 2 * outline.width};
		}
	};

	struct Coordinates: public Primitive {
		/* List of coordinates to define a geometric form.
		 * NB: Coordinates are internally always stored in a same space (to be defined).
		 * e.g. where the reference point is the top-left vertex of the bound box. */
		ArrayList<Coordinate> coordinates;
		Size size() const override {
			if (!coordinates)
				return {0, 0};
			int min_x, min_y, max_x, max_y;
			auto it = coordinates.begin(), end = coordinates.end();
			min_x = max_x = it->x;
			min_y = max_y = it->y;
			++it;
			while (it != end) {
				if (min_x > it->x) min_x = it->x;
				if (min_y > it->y) min_y = it->y;
				if (max_x < it->x) max_x = it->x;
				if (max_y < it->y) max_y = it->y;
				++it;
			}
			return {size_t(max_x - min_x) + 2 * outline.width, size_t(max_y - min_y) + 2 * outline.width};
		}
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

namespace cigmar::gui {

	struct Drawer {
		virtual void drawPoints(const primitive::Coordinates& vertices) = 0;
		virtual void drawSegments(const primitive::Coordinates& vertices) = 0;
		virtual void drawPolygon(const primitive::Coordinates& vertices) = 0;
		virtual void drawEllipse(const primitive::Ellipse& ellipse) = 0;
		virtual void drawSurface(const primitive::Surface& surface) = 0;
		virtual void drawText(const UnicodeString& text, const primitive::TextFormat& format) = 0;
		/// NB: To draw a Bezier curve, we could calculate coordinates ourselves
		/// and use either drawPoints() or drawSegments().
	};

}

#endif //SRC_CIGMAR_PRIMITIVE_HPP
