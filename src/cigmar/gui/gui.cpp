//
// Created by notoraptor on 17-12-04.
//

#include <cigmar/gui/gui.hpp>

namespace cigmar::gui {
	Disposition Disposition::top(DispositionType::TOP);
	Disposition Disposition::left(DispositionType::LEFT);
	Disposition Disposition::bottom(DispositionType::BOTTOM);
	Disposition Disposition::right(DispositionType::RIGHT);
	Disposition Disposition::center(DispositionType::CENTER);
	Disposition Disposition::justify(DispositionType::JUSTIFY);

	namespace primitive {
		Color Color::white = {255, 255, 255};
		Color Color::black = {0, 0, 0};
		Color Color::transparent = {0, 0, 0, 0};
	}

}
