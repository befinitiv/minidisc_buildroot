#include <iostream>
#include <mpg123.h>
#include <filesystem>
#include <unistd.h>


#include "SSD1306.h"
#include "Cd.h"
#include "Mpg123.h"
#include "Buttons.h"

Mpg123 mpg123;
SSD1306 display;

bool poweroff = false;

void ready() {
	std::cout << "ready" << std::endl;
	
	namespace fs = std::filesystem;
	for (auto const& dir_entry : fs::recursive_directory_iterator("/tmp/cd")) {
				if(!dir_entry.is_regular_file() || dir_entry.path().extension() != ".mp3")
					continue;
        std::cout << dir_entry.path().string() << '\n';
				mpg123.play(dir_entry.path().string());
				if(poweroff)
					break;
	}
}

void notReady() {
	std::cout << "not ready" << std::endl;
}

void pressed(unsigned int num) {
	std::cout << "pressed " << num << std::endl;

	switch(num) {
		case Buttons::STOP:
			std::cout << "Stopping" << std::endl;
			mpg123.stop();
			break;
		case Buttons::VOLINC:
			std::cout << "Increasing volume" << std::endl;
			mpg123.volumeChange(0.05);
			break;
		case Buttons::VOLDEC:
			std::cout << "Decreasing volume" << std::endl;
			mpg123.volumeChange(-0.05);
			break;
		case Buttons::PLAY:
			poweroff = true;
			mpg123.stop();
			break;
		default:
			break;
		}
}

int main() {
	/*Cd cd(ready, notReady);
	Buttons buttons(pressed);

	while(!poweroff) {
		sleep(1);
	}
	buttons.powerOff();
*/
	return 0;
}
