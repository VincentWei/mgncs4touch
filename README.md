# mGNCS4Touch

mGNCS4Touch - This component provides some new controls, which are compliant
to new control API spec of mGNCS, for devices with a touch screen.

This is the mainline release of mGNCS4Touch for MiniGUI V3.2.x and mGNCS V1.2.x.

## Prerequisites

    * MiniGUI: v3.2.10 or later
    * mGPlus: v1.4.0 or later
    * mGEff: v1.2.0 or later
    * mGNCS: V1.2.6 or later

## Building

mGNCS4Touch uses GNU autoconf/automake scripts to configure and build the project.

Run

    $ ./configure; make; sudo make install

to configure, make, and install the headers and the libraries (libmgncs4touch).

mGNCS4Touch also provides some configuration options to customize the features.
For more information, please run

    $ ./configure --help

After installed mGNCS4Touch successfully, you can change to samples/ to make
the samples:

    $ cd samples/
    $ make

## Copying

Copyright (C) 2008 ~ 2018, Beijing FMSoft Technologies Co., Ltd.


