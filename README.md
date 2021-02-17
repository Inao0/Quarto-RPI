# Quarto-RPI
Quarto Game to be played on a Joy-IT raspberry case, utilizing the SBC-ButtonMatrix.

## Compiling
The project needs a few requirements to work properly.
It was designed to use cross-compiling on a personnal computer.
Thus you need to cross-compile these requirements to be able to build Quarto-RPI:
* wiringPi
* ncurses
* cdk
* SDL2
* SDL2_ttf (along with freetype, libpng, and zlib)

These should be placed under the folder `./lib/rpi` but you can place them anywhere, as long as you define the `<LIBNAME>_RPI` variables in the Makefile. Beware also of the name of the libraries, considering they will be used in the `assets/run.sh` script to locate the libraries.

You also need to define the following environment variables:
* `RPI_COMPILER=/path/to/cross-compiler` to cross-compile
* `RPI_ADDRESS=<login>@<ipadress>` to send automatically the built files

*Note: you can also use a standard compiler, if you need to test UI related code on your computer.*

A few Makefile commands:
* `make` compiles the whole project
* `make run` compiles the PC project (without wiringPi and GPIO support), and runs the application on the PC
* `make deploy` compiles the RPI project, sends it to the raspberry, and runs the application on the RPI
* `make deploy-here` does the same as above, but the UI is displayed on the PC (unstable)
* `make deploy-lib` sends the libraries and `run.sh` to the RPI

*Note: the first deployement usually takes a while considering that all the libraries need to be sent to the RPI.*

## Demo
:construction: Work in progress :construction:

## Note
This repository and its files are currently very barebones. It is a simple student project. This is all just made for fun and nothing too serious.

## Credits
This software uses the following open source libraries:
* [wiringPi](http://wiringpi.com/)
* [sdl](https://wiki.libsdl.org/FrontPage)
* [sdl_ttf](https://www.libsdl.org/projects/SDL_ttf/)
* [cdk](https://invisible-island.net/cdk/)