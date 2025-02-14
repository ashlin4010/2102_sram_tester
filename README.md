# 2102 SRAM Tester #

A basic tester to quickly check 2102 RAM chips. This tester only performs rudimentary tests and is very lax with its timing so it may not fully detect marginal RAM chips.

![RAM tester PCB](/images/tester.jpg)
![RAM schematics](/images/schematic.jpg)

## Firmware ##
The firmware is a quick and dirty Arduino program and makes use of `digitalWrite` and is subsequently very slow. Things could be sped up significantly by writing the firmware in AVR and using timers and cycle counting. The circuit would also benefit from an external latch for the address lines.
