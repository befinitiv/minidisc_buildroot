#pragma once

#include <string>
#include <thread>
#include <functional>

class Cd {
public:
	using Callback = std::function<void()>;
	Cd(Callback ready, Callback notReady);
	~Cd();

private:
	const std::string MOUNT_POINT = "/tmp/cd";
	const std::string DRIVE = "/dev/sr0";

	bool mounted = false;
	bool stopped = false;

	int driveFd;
	std::thread monitorThread;

	Callback ready;
	Callback notReady;

	void monitorDrive();
	void mountDrive();
	void umountDrive();
};
