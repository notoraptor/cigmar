//
// Created by notoraptor on 17-12-04.
//

#ifndef SRC_CIGMAR_BACKEND_HPP
#define SRC_CIGMAR_BACKEND_HPP
#include <cigmar/gui/primitive.hpp>

namespace cigmar::gui::backend {
	struct WindowHandler: public Drawer {
		/** Initialize the window handler. Must raise an Exception on any failure.
		 * If ``windowProperties.required``, should initialize the handler with
		 * exactly the properties provided, else should raise an exception.
		 * Return the real properties of the initialized window.
		 * If ``windowProperties.required``, the returned value
		 * should be equal to the requested value. **/
		virtual WindowProperties open(const WindowProperties& requestedProperties) = 0;
		virtual bool close() = 0;
		virtual bool isOpen() = 0;
		virtual bool waitEvent(Event& event) = 0;
		virtual bool pollEvent(Event& event) = 0;
		virtual void clear(const primitive::Color& color) = 0;
		virtual void display() = 0;
		virtual void resize(const Size& size) = 0;
		virtual void setPosition(const Coordinate& position) = 0;
		virtual void getDesktopMode(WindowProperties& windowProperties) = 0;
		virtual void getFullscreenModes(ArrayList<WindowProperties>& out) = 0;
		virtual primitive::Font getFont(const String& fontPath) = 0;
		// virtual Coordinate currentMousePosition() = 0;
	};
}

#endif //SRC_CIGMAR_BACKEND_HPP
