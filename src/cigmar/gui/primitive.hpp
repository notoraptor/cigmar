//
// Created by notoraptor on 17-12-04.
//
#ifndef SRC_CIGMAR_PRIMITIVE_HPP
#define SRC_CIGMAR_PRIMITIVE_HPP

#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/gui/utils.hpp>

namespace cigmar::gui {

	namespace primitive {

		struct Primitive;

		struct Color {
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char alpha; // 0: transparent, 255: opaque
			explicit Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) noexcept :
					red(r), green(g), blue(b), alpha(a) {}
			bool isTransparent() const {
				return alpha == 0;
			}
			static Color white;
			static Color black;
			static Color transparent;
			// TODO: Define all standard HTML colors.
		};

		struct Gradient {
			ArrayList<Color> points;
			Gradient(): points() {}
			explicit Gradient(std::initializer_list<Color> il): points(il) {}
			explicit Gradient(const ArrayList<Color>& gradPoints): points(gradPoints) {}
			explicit Gradient(ArrayList<Color>&& gradPoints) noexcept : points(std::move(gradPoints)) {}
			bool isTransparent() const {
				if (points) {
					for (Color& color: points) if (color.alpha != 0) return false;
					return true;
				}
				return false;
			}
		};

		struct Motif {
			Image::View view;
			bool hrepeat = false;
			bool vrepeat = false;
			Motif(Image* image, const Coordinate& from, const Size& size, bool hRepeat, bool vRepeat) {
				view.image = image;
				view.x = from.x;
				view.y = from.y;
				view.width = size.width;
				view.height = size.height;
				hrepeat = hRepeat;
				vrepeat = vRepeat;
			}
			bool isTransparent() const;
		};

		struct Canvas {
			// A canvas is always defined wrt/ the bound box the primitive for which it's a background.
			enum class Scaling {NONE, SCALED, FIT};
			Scaling scaling;
			ArrayList<Primitive*> primitives;
			Canvas(): scaling(Scaling::NONE), primitives() {}
			explicit Canvas(Scaling canvasScaling): scaling(canvasScaling), primitives() {}
			bool isTransparent() const;
		};

		struct Background {
			enum class Type {COLOR, GRADIENT, MOTIF, CANVAS, UNKNOWN};
			Type type() const {return m_type;}
			Background(): m_type(Type::UNKNOWN), m_background() {};
			explicit Background(const Color& color): m_type(Type::COLOR), m_background() {
				m_background.color = &color;
			};
			explicit Background(const Gradient& gradient): m_type(Type::GRADIENT), m_background() {
				m_background.gradient = &gradient;
			};
			explicit Background(const Motif& motif): m_type(Type::MOTIF), m_background() {
				m_background.motif = &motif;
			};
			explicit Background(const Canvas& canvas): m_type(Type::CANVAS), m_background() {
				m_background.canvas = &canvas;
			};
			void set(const Color& color) {
				m_type = Type::COLOR;
				m_background.color = &color;
			}
			void set(const Gradient& gradient) {
				m_type = Type::GRADIENT;
				m_background.gradient = &gradient;
			}
			void set(const Motif& motif) {
				m_type = Type::MOTIF;
				m_background.motif = &motif;
			}
			void set(const Canvas& canvas) {
				m_type = Type::CANVAS;
				m_background.canvas = &canvas;
			}
			const Color& color() const {
				if (m_type != Type::COLOR)
					throw Exception("Background: bad type (expected Color).");
				return *m_background.color;
			}
			const Gradient& gradient() const {
				if (m_type != Type::GRADIENT)
					throw Exception("Background: bad type (expected Gradient).");
				return *m_background.gradient;
			}
			const Motif& motif() const {
				if (m_type != Type::MOTIF)
					throw Exception("Background: bad type (expected Motif).");
				return *m_background.motif;
			}
			const Canvas& canvas() const {
				if (m_type != Type::CANVAS)
					throw Exception("Background: bad type (expected Canvas).");
				return *m_background.canvas;
			}
			bool isColor() const {
				return m_type == Type::COLOR;
			}
			bool isGradient() const {
				return m_type == Type::GRADIENT;
			}
			bool isMotif() const {
				return m_type == Type::MOTIF;
			}
			bool isCanvas() const {
				return m_type == Type::CANVAS;
			}
			bool transparent() const {
				switch (m_type) {
					case Type::COLOR:
						return m_background.color->isTransparent();
					case Type::GRADIENT:
						return m_background.gradient->isTransparent();
					case Type::MOTIF:
						return m_background.motif->isTransparent();
					case Type::CANVAS:
						return m_background.canvas->isTransparent();
					case Type::UNKNOWN:
						return true;
				}
			}
		private:
			union {
				const Color* color;
				const Gradient* gradient;
				const Motif* motif;
				const Canvas* canvas;
			} m_background;
			Type m_type;
		};

		struct Border {
			size_t width;
			Color color;
		};

		struct Primitive {
			Border border;
			Background background;
			Coordinate position;	// Position of the top-left point of the surrounding box.
			/** Must return width and height of the primitive box (smallest rectangle surrounding the primitive).
			 * NB; Should take border into account. **/
			virtual Size box() const = 0;
		};

		struct Ellipse: public Primitive {
			size_t hradius;
			size_t vradius;
			Ellipse(size_t horizontalRadius, size_t verticalRadius): hradius(horizontalRadius), vradius(verticalRadius) {}
			Size box() const override {
				return {2 * (hradius + border.width), 2 * (vradius + border.width)};
			}
		};

		struct Surface: public Primitive, public Size {
			Size box() const override {
				return {width + 2 * border.width, height + 2 * border.width};
			}
		};

		struct Coordinates: public Primitive {
			/* List of coordinates to define a geometric form.
			 * NB: Coordinates are internally always stored in a same space (to be defined).
			 * e.g. where the reference point is the top-left vertex of the bound box. */
			ArrayList<Coordinate> coordinates;
			Size box() const override {
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
				return {size_t(max_x - min_x) + 2 * border.width, size_t(max_y - min_y) + 2 * border.width};
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
			/**< If true:
			 *  - First and last segment are extended as semi-lines.
			 *  - If connection contains only two points, then the segment is extended as a line.
			 *  **/
		};

		struct Bezier: public Connection {};

		struct Font {
			const size_t id; // Internal (back-end) font ID, understandable by WindowHandler only.

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
