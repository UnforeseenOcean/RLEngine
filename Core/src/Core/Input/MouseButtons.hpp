#pragma once

namespace _internal {

struct MouseButton {
	enum Enum {
		LEFT,
		RIGHT,
		MIDDLE,
		FOUR,
		FIVE,
	};

	static const int TOTAL = FIVE + 1;
};

} //namespace _internal