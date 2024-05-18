#pragma once

#include <gpiod.hpp>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


class SSD1306 {
public:
	SSD1306() {
		std::cout << "Initializing display" << std::endl;
		::gpiod::chip chip("gpiochip0");
		dc = chip.get_line(103);  // GPIO17
   	dc.request({"dc", gpiod::line_request::DIRECTION_OUTPUT, 0},1);
	 	dc.set_value(0);

		spiFd = open("/dev/spidev0.0", O_RDWR);
		if(spiFd < 0)
			throw std::runtime_error("Error opening SPI device");

		//stolen from https://elixir.bootlin.com/linux/latest/source/drivers/staging/fbtft/fb_ssd1306.c
		spiTransferByte(0xAE); //Set Display OFF
		spiTransferByte(0xD5); //Set Display Clock Divide Ratio/ Oscillator Frequency
		spiTransferByte(0x80);

		//Set Multiplex Ratio
		spiTransferByte(0xA8);
		spiTransferByte(0xF);

		//offset
		spiTransferByte(0xD3);
		spiTransferByte(0x0);

		//start line
		spiTransferByte(0x40);

		spiTransferByte(0x8D);
		spiTransferByte(0x14);
		
		spiTransferByte(0xDA);
		spiTransferByte(0x02);
		
		spiTransferByte(0x20);
		spiTransferByte(0x00);
		
		spiTransferByte(0xA1);
		
		spiTransferByte(0xC8);
		
		spiTransferByte(0x02);

		spiTransferByte(0xD9);
		spiTransferByte(0xF1);
		
		spiTransferByte(0xDB);
		spiTransferByte(0x40);
		
		spiTransferByte(0xA4);
		spiTransferByte(0xA6);
		spiTransferByte(0xAF);
	 	
	 	memset(img, 0, sizeof(img));
		update();
	 }
	
	void update() {
		uint8_t vmem[96*16/8];

		int line = 0;
		int idx = 95;
		for(int i=0; i<sizeof(vmem); ++i) {
			vmem[i] = (img[15-line-0][idx] << 0) | (img[15-line-1][idx] << 1) | (img[15-line-2][idx] << 2) | (img[15-line-3][idx] << 3) | (img[15-line-4][idx] << 4) | (img[15-line-5][idx] << 5) | (img[15-line-6][idx] << 6) | (img[15-line-7][idx] << 7);
			if(idx-- == 0) {
				idx = 95;
				line = 8;
			}
		}
		
		dc.set_value(0);
		spiTransferByte(0x22);
		spiTransferByte(0x0);
		spiTransferByte(0x1);

		spiTransferByte(0x21);
		spiTransferByte(0x00);
		spiTransferByte(96-1);
		
		dc.set_value(1);
		write(spiFd, vmem, sizeof(vmem));
	}

	uint8_t img[16][96];
private:
	gpiod::line dc;
	int spiFd;



	void spiTransferByte(uint8_t d) {
		if(write(spiFd, &d, 1) != 1) {
			throw std::runtime_error("Error writing SPI byte");
		}
	}

};
