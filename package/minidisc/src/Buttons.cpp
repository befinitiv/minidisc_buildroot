#include <iostream>

#include <gpiod.hpp>

#include "Buttons.h"

Buttons::Buttons(Callback callbackPressed)
	: callbackPressed(callbackPressed) {
	
	::gpiod::chip chip("gpiochip0");
	pwrLine = chip.get_line(PWR);
	pwrLine.request({"example", gpiod::line_request::DIRECTION_OUTPUT, 0},1);
	pwrLine.set_value(1);

	bulk = gpiod::chip("gpiochip0", gpiod::chip::OPEN_BY_NAME).get_lines({ PLAY, STOP, PREV, NEXT, VOLINC, VOLDEC });
	
	bulk.request({ "monitor",
        gpiod::line_request::EVENT_RISING_EDGE});

	monitorThread = std::thread(&Buttons::monitor, this);
}

Buttons::~Buttons() {
	stopped = true;
	monitorThread.join();
}

void Buttons::monitor() {
	using namespace std::chrono_literals;
	while (!stopped) {
					// wait for events and get bulk of lines on which events occured
					auto changed = bulk.event_wait(std::chrono::seconds(1s));
					// iterate lines in bulk
					for (const auto &line : changed) {
							callbackPressed(line.offset());
							// read events occured on a line
							auto events = line.event_read_multiple();
							/*// iterate events
							for (const auto &event : events) {
									std::cout << "\ttype: " << event.event_type << std::endl;
							}*/
					}
			}
}
