# SpaceIvaders
Space invaders emulator with C++ and SFML

## Build the project
Edit `Makefile` and select between using SFML
```
SYSLIB := SFML
```
or using SDL:
```
SYSLIB := SDL
```
After that, just call `make`:
```
$ make install
```
## Run the project
```
$ emu8080
```
## Build the unit tests
Tests are currently broken.
```
$ cd unit
$ make
$ ./unit
```
