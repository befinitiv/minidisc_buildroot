#pragma once

#include <out123.h>
#include <mpg123.h>

#include <string>

class Mpg123 {
	public:
		Mpg123();

		void play(std::string fileName);
		void stop() { stopped = true; }
		void volumeChange(float change);
private:
	mpg123_handle *mh;
	out123_handle *ao;

	bool stopped = false;
};
