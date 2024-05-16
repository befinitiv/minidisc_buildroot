#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <unistd.h>
#include <sys/mount.h>

#include <stdexcept>
#include <thread>
#include <iostream>

#include "Cd.h"

Cd::Cd(Callback ready, Callback notReady)
	: ready(ready)
	, notReady(notReady) {
		

	//umount in case there is a previous mount still active (ok to fail)
	umount(MOUNT_POINT.c_str());

	DIR* dir = opendir(MOUNT_POINT.c_str());

	if(!dir) {
		if(mkdir(MOUNT_POINT.c_str(), 0700)) {
			throw std::runtime_error("Failed to create mount point");
		}
		std::cout << "Created mount point" << std::endl;
	}

	driveFd = open(DRIVE.c_str(), O_NONBLOCK | O_RDONLY);
	if(driveFd < 0) {
		throw std::runtime_error("Error opening CD drive");
	}
	


	monitorThread = std::thread(&Cd::monitorDrive, this);
}

Cd::~Cd() {
	stopped = true;
	monitorThread.join();
}

void Cd::monitorDrive() {
	std::cout << "Started monitoring thread" << std::endl;
	while(!stopped) {
		int r = ioctl(driveFd, CDROM_DRIVE_STATUS);
		std::cout << r << std::endl;	
		if(!mounted && r == CDS_DISC_OK) {
		int speed = 1;
		 r = ioctl(driveFd, CDROM_SELECT_SPEED, speed);
		if(r)
			perror("set speed");

			mountDrive();
			}
	
		if(mounted && r != CDS_DISC_OK)
			umountDrive();

		sleep(1);
	}
}

void Cd::mountDrive() {
	std::cout << "Trying to mount drive..." << std::endl;

	int r = mount(DRIVE.c_str(), MOUNT_POINT.c_str(), "iso9660", MS_RDONLY, (const void*)"");
	if(r) {
		std::cout << "Failed to mount drive" << std::endl;
		perror("mount");
		return;
	}
	std::cout << "Successfully mounted drive" << std::endl;
	mounted = true;
	ready();
}

void Cd::umountDrive() {
	std::cout << "Trying to umount drive..." << std::endl;
	int r = umount(MOUNT_POINT.c_str());
	if(r) {
		std::cout << "Failed to umount drive" << std::endl;
		perror("umount");
		return;
	}
	std::cout << "Successfully umounted drive" << std::endl;
	mounted = false;
	notReady();
}

