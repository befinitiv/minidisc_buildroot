#pragma once

#include <functional>
#include <thread>

#include <gpiod.hpp>


class Buttons {
public:
	using Callback = std::function<void(unsigned int)>;
	Buttons(Callback callbackPressed);
	~Buttons();

	void powerOff() { pwrLine.set_value(0); }

	static const unsigned int PWR = 96;
	static const unsigned int PLAY = 97;
	static const unsigned int STOP = 98;
	static const unsigned int PREV = 99;
	static const unsigned int NEXT = 100;
	static const unsigned int VOLINC = 101;
	static const unsigned int VOLDEC = 102;

private:
	gpiod::line_bulk bulk;
	Callback callbackPressed;
	gpiod::line pwrLine;
	
	std::thread monitorThread;
	bool stopped = false;
	void monitor();
};
