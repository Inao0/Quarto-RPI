#!/usr/bin/env bash
export LD_LIBRARY_PATH=/home/pi/QuartoRPI/wiringPi/lib:/home/pi/QuartoRPI/cdk/lib:/home/pi/QuartoRPI/ncurses/lib:/home/pi/QuartoRPI/sdl/lib:/home/pi/QuartoRPI/sdl_ttf/lib:/home/pi/QuartoRPI/bzip:/home/pi/QuartoRPI/freetype/lib:/home/pi/QuartoRPI/libpng/lib
export TERMINFO=/usr/share/terminfo
export SDL_RENDER_DRIVER=software
export TERM=xterm-88color
#sleep 1
/home/pi/QuartoRPI/QuartoRPI
read -p "Press Enter to close this window"
