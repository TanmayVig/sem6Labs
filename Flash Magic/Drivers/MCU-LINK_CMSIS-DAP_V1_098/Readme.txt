Release Notes for MCU-Link Firmware
===================================

Version: 1.098
Date: 13 October 2020

Overview
========

MCU-Link is a new debug probe solution/architecture being developed for NXP LPC, Kinetis
and i.MX RT MCU targets, intended as a replacement for the standalone LPC-Link2 (OM13054)
and various on-board debug probe solutions.

Scope
=====

Currently the firmware supports MCU-Link probe (LQFP64 package).

Release History
===============

 - V1.098 - First MCU-Link production firmware release
               Includes:
               - drivers for Windows operating systems
               - Udev rules for Linux operating systems


Feature Set
===========

The current release of the firmware provides the following features:
    - CMSIS-DAP 1.1 support
    - Support for SWD debugging from MCUXpresso IDE (and other CMSIS-DAP compatible IDEs)
    - Support for SWO Trace capture from MCUXpresso IDE
    - VCOM - UART bridge connected to the target processor

The name (product string) is of the form "MCU-LINK r<RRR> CMSIS_DAP V<V>.<BBB>" where
<RRR> is the hardware revision, <V> is the firmware version (1) and <BBB> is a non-zero
build number.


Firmware Installation Guide
===========================

This firmware installation guide is suitable for Windows, Linux and Mac OSX environments

A script is supplied to enable the programming of the firmware image into MCU-Link.

To make use of this functionality, place the board in ISP USB mode using the appropriate
jumper on your MCU-Link board, then connect it to the host computer via USB.
Refer to the board documentation for more information.

To install CMSIS-DAP debug firmware, open a command prompt and call the program script:
    <Install Dir>\scripts\program

The script programs the firmware image into flash using blhost utility.
Once completed, follow the on-screen instructions to make use of the programmed debug
probe.

Further information about the use of blhost is available in the User Guide
https://www.nxp.com/docs/en/user-guide/MCUBLHOSTUG.pdf

Note:
    The script offers the option to repeat the programming sequence to enable
    multiple debug probes to be programmed in sequence.


Host Configuration
===================

Windows hosts
    A set of drivers is provided to allow displaying friendly names
    in Device Manager for the MCU-Link VCom Port, and CMSIS-DAP and Trace HID devices.

    To install the drivers, navigate to <Install Dir>\drivers and install the drivers
    by right-click > Install on the two *.inf files:
      - mcu-link-vcom.inf
      - mcu-link-hid.inf

Linux hosts
    A set of Udev rules is provided for MCU-Link USB Devices
    to allow read-write access for all users.
    To install the rules, copy <Install Dir>/drivers/85-mcu-link.rules to /lib/udev/rules.d
    (may need root/sudo permissions).


Probe Configuration
===================

The support for standard features can be controlled via jumpers on board:
 - For example, on MCU-Link stand-alone board:
    - VCOM can be disabled through J4 jumper
    - SWD debug including SWO trace can be disabled through J5 jumper


Testing
=======

The following features have been validated with this firmware build on an MCU-Link
stand-alone board using MCUXpresso IDE 11.2.1 on:
    - Windows 10
    - MacOSX 10.13
    - Ubuntu 18.04

Tested functionalities:
    - Probe identification
    - SWD
    - SWO (trace)
    - VCOM

Minimal testing of SWD debugging support in other CMSIS-DAP compatible IDE's:
    - IAR Embedded Workbench IDE for ARM 8.40.2
    - Keil uVision V5.28.0.0
