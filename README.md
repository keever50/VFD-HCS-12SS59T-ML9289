# VFD-HCS-12SS59T-ML9289
Driver based on https://github.com/qrti/VFD-HCS-12SS59T with AC filament supply. For Arduino uno, ESP32 and RP2040.

This driver is made for the board above, but can most likely work with other boards as well.
The driver is made to be compatible with the AC filament supply that requires an external clock pulse of 16kHz-17kHz, which this driver provides.
It also provides the necessary communication with the integrated VFD driver chip.

This driver uses the bit banging method instead using SPI.
The reason behind this is to send characters, you use a very few bytes. 
Having an SPI bus available for other tasks out weights the pros of having SPI for the VFD.

In order to achieve the clock pulse to drive the VFD drive pin, this driver makes use of timers. 
In case of arduino uno, it can only use a few pins since timers are connected to specific pins.
