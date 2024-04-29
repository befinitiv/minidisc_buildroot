#pragma once

#include <functional>
#include <thread>

#include <gpiod.hpp>


class Buttons {
public:
	using Callback = std::function<void(unsigned int)>;
	Buttons(Callback callbackPressed);
	~Buttons();

	const unsigned int PWR = 96;
	const unsigned int PLAY = 97;
	const unsigned int STOP = 98;
	const unsigned int PREV = 99;
	const unsigned int NEXT = 100;
	const unsigned int VOLINC = 101;
	const unsigned int VOLDEC = 102;

private:
	gpiod::line_bulk bulk;
	Callback callbackPressed;
	
	std::thread monitorThread;
	bool stopped = false;
	void monitor();
};
