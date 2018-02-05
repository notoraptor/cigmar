//
// Created by notoraptor on 17-12-04.
//

#include <cigmar/gui/gui.hpp>

namespace cigmar::gui {

	Size Size::max{std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};

	namespace primitive {
		Color Color::white = {255, 255, 255};
		Color Color::black = {0, 0, 0};
		Color Color::transparent = {0, 0, 0, 0};
	}

}
