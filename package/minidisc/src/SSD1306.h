#pragma once

#include <gpiod.hpp>

class SSD1306 {
	SSD1306() {
		::gpiod::chip chip("gpiochip0");
	auto line = chip.get_line(17);  // GPIO17
   line.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0},1);
	 line.set_value(0);
	 }
};
