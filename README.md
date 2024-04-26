# Minidisc Buildroot External Project
This repository contains everything needed to build an SD card image which can be flashed onto the minidisc player. This includes u-boot, kernel, rootfs & minidisc application.

# Installation
```
wget https://buildroot.org/downloads/buildroot-2024.02.1.tar.gz
tar xzvf buildroot-2024.02.1.tar.gz
git clone git@github.com:befinitiv/minidisc_buildroot.git
cd buildroot-2024.02.1
make BR2_EXTERNAL=../minidisc_buildroot minidisc_defconfig
make
```

After the compilation you can flash the SD card image like so:
```
sudo dd if=output/images/sdcard.img of=/dev/mmcblk0 status=progress
```

The main application resides under package/minidisc.

# Usage
Insert the written SD card into the minidisc player and connect it via USB to your PC. The device will present itself as a serial device with 921600 baud. 
By connecting with a terminal program to the serial port you can see the kernel boot messages and after ~one second you can log in using root/root.

## SSH & SCP
Serial terminals typically suck compared to SSH (single session only, limited resolution and terminal emulations).
Unfortunately, the minidisc hardware can only provide serial console via USB because the USB port of the Allwinner CPU is blocked by the CD drive.

To overcome this annoyance, the generated system contains a helper script which will spawn a PPP software on the serial port.
This allows you then to connect to this PPP software from you PC and creates a network link over serial with the minidisc hardware.
It can be initiated like so:

(On minidisc)
```
ppp #(helper script on minidisc filesystem)
```

(On PC)
```
sudo pppd debug nodetach noauth nocrtscts local 192.168.10.1:192.168.10.10 /dev/ttyUSB0 921600
```


Now you can do the usual things like "ssh root@192.168.10.10" or "scp something root@192.168.10.10:".
