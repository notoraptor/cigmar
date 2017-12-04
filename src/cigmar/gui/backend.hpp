//
// Created by notoraptor on 17-12-04.
//

#ifndef SRC_CIGMAR_BACKEND_HPP
#define SRC_CIGMAR_BACKEND_HPP

#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/gui/primitive.hpp>

namespace cigmar::gui::backend {
	struct WindowHandler {
		// Init, and return real properties of initialized handler (to check if properties queried are indeed retrieved).
		virtual WindowProperties init(const WindowProperties& windowProperties) = 0;
		virtual bool isOpen() = 0;
		virtual bool close() = 0;
		virtual bool waitEvent(Event& event) = 0;
		virtual bool pollEvent(Event& event) = 0;
		virtual void clear(const primitive::Color& color) = 0;
		virtual void display() = 0;
		virtual void setDesktopMode(WindowResolution& windowResolution) = 0;
		virtual void getFullscreenModes(ArrayList<WindowResolution>& out) = 0;
		virtual primitive::Font getFont(const String& fontPath) = 0;
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

#endif //SRC_CIGMAR_BACKEND_HPP
