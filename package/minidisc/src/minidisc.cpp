#include <iostream>
#include <mpg123.h>

#include "SSD1306.h"
#include "Cd.h"
#include "Mpg123.h"

Mpg123 mpg123;

void ready() {
	std::cout << "ready" << std::endl;
	mpg123.play("/tmp/cd/16___jim.mp3");
}

void notReady() {
	std::cout << "not ready" << std::endl;
}

int main() {
	Cd cd(ready, notReady);
	for(;;);
	return 0;
}
