#include <iostream>
#include <mpg123.h>

#include "SSD1306.h"
#include "Cd.h"
#include "Mpg123.h"
#include "Buttons.h"

Mpg123 mpg123;

void ready() {
	std::cout << "ready" << std::endl;
	mpg123.play("/tmp/cd/16___jim.mp3");
}

void notReady() {
	std::cout << "not ready" << std::endl;
}

void pressed(unsigned int num) {
	std::cout << "pressed " << num << std::endl;
}

int main() {
	Cd cd(ready, notReady);
	Buttons buttons(pressed);

	for(;;);

	return 0;
}
