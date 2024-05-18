#include <stdexcept>
#include <iostream>

#include "Mpg123.h"

Mpg123::Mpg123()
	: mh(nullptr)
	, ao(nullptr)
	{
	int err;
	
	if(mpg123_init() != MPG123_OK || (mh = mpg123_new(NULL, &err)) == NULL)
		throw std::runtime_error("Error initializing mpg123");

	ao = out123_new();
	if(!ao)
		throw std::runtime_error("Error initializing out123");
}


void Mpg123::play(std::string fileName) {
	stopped = false;
	int channels = 0;
	int encoding = 0;
	long rate = 0;

	if(mpg123_open(mh, fileName.c_str()) != MPG123_OK || mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK)
		throw std::runtime_error("Error opening MP3 file");

	if(out123_open(ao, nullptr, nullptr) != OUT123_OK)
		throw std::runtime_error("Error opening out123 device");

	mpg123_format_none(mh);
	mpg123_format(mh, rate, channels, encoding);

	int framesize;
	if(  out123_start(ao, rate, channels, encoding)
	  || out123_getformat(ao, NULL, NULL, NULL, &framesize) )
			throw std::runtime_error("Error starting out123");

	size_t buffer_size = 0;
	unsigned char* buffer = NULL;
	buffer_size = mpg123_outblock(mh);
	buffer = (unsigned char*)malloc( buffer_size );

	size_t done = 0;
	off_t samples = 0;

	int err;
	do
	{
		size_t played;
		err = mpg123_read( mh, buffer, buffer_size, &done );
		played = out123_play(ao, buffer, done);
		if(played != done)
		{
			fprintf(stderr
			,	"Warning: written less than gotten from libmpg123: %li != %li\n"
			,	(long)played, (long)done);
		}
		samples += played/framesize;
		//std::cout << "Played " << samples << std::endl;
	} while (!stopped && done && err==MPG123_OK);

	mpg123_close(mh);

	free(buffer);
}

void Mpg123::volumeChange(float change) {
	mpg123_volume_change(mh, change);
}
