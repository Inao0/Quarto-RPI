#------------------------------Standard flags----------------------------#
CFLAGS = -std=c99 -Wall -Wextra -Winline -pipe
LDFLAGS = -lncurses -lcdk -ldl -lSDL2 -lSDL2_ttf -lm

#-----------------------------Libraries for PC---------------------------#
NCURSES_PC=./lib/pc/ncurses
CDK_PC=./lib/pc/cdk
SDL_PC=./lib/pc/sdl
SDLTTF_PC=./lib/pc/sdl_ttf

#--------------------------Libraries for Raspberry-----------------------#
NCURSES_RPI=./lib/rpi/ncurses
CDK_RPI=./lib/rpi/cdk
WIRING_RPI=./lib/rpi/wiringPi/build
SDL_RPI=./lib/rpi/sdl
SDLTTF_RPI=./lib/rpi/sdl_ttf
FREETYPE_RPI=./lib/rpi/freetype
LIBPNG_RPI=./lib/rpi/libpng
ZLIB_RPI=./lib/rpi/zlib

#---------------------/!\ Do not edit past this point /!\-----------------#
INCLUDE = -I./include
LIB = -L./lib
SRCS = $(wildcard src/*.c)
OBJS = $(subst src/,build/,$(patsubst %.c,%.o,$(SRCS)))
OBJS_PC = $(subst build/,build/pc-,$(OBJS))
OBJS_RPI = $(subst build/,build/rpi-,$(OBJS))

INCLUDE_PC = -I$(NCURSES_PC)/include -I$(NCURSES_PC)/include/ncurses -I$(CDK_PC)/include -I$(CDK_PC)/include/cdk -I$(SDL_PC)/include -I$(SDL_PC)/include/SDL2 -I$(SDLTTF_PC)/include $(INCLUDE)
LIB_PC = -L$(NCURSES_PC)/lib -L$(CDK_PC)/lib -L$(SDL_PC)/lib -L$(SDLTTF_PC)/lib $(LIB)
CFLAGS_PC = $(CFLAGS) $(INCLUDE_PC) -DBUILD_PC
LDFLAGS_PC = $(LIB_PC) $(LDFLAGS)

INCLUDE_RPI = -I$(NCURSES_RPI)/include -I$(NCURSES_RPI)/include/ncurses -I$(CDK_RPI)/include -I$(CDK_RPI)/include/cdk -I$(SDL_RPI)/include -I$(SDL_RPI)/include/SDL2 -I$(SDLTTF_RPI)/include -I$(WIRING_RPI)/include $(INCLUDE)
LIB_RPI = -L$(NCURSES_RPI)/lib -L$(NCURSES_RPI)/lib -L$(WIRING_RPI)/lib -L$(CDK_RPI)/lib -L$(SDL_RPI)/lib -L$(SDLTTF_RPI)/lib -L$(FREETYPE_RPI)/lib -L$(LIBPNG_RPI)/lib -L$(ZLIB_RPI)/lib $(LIB)
CFLAGS_RPI = $(CFLAGS) $(INCLUDE_RPI)
LDFLAGS_RPI = $(LIB_RPI) $(LDFLAGS) -lwiringPi -lfreetype -lpng -lz

all: pc rpi

pc: build/QuartoPC

build/QuartoPC: $(OBJS_PC)
	$(CC) $(CFLAGS_PC) $(LDFLAGS_PC) -o build/QuartoPC $(OBJS_PC)

build/pc-%.o: src/%.c
	$(CC) -c $(CFLAGS_PC) $(LDFLAGS_PC) -o $@ $<

run: ./build/QuartoPC
	./build/QuartoPC

rpi: build/QuartoRPI

build/rpi-%.o: src/%.c
	$(RPI_COMPILER) -c $(CFLAGS_RPI) $(LDFLAGS_RPI) -o $@ $<

build/QuartoRPI: $(OBJS_RPI)
	$(RPI_COMPILER) $(CFLAGS_RPI) $(LDFLAGS_RPI) -o build/QuartoRPI $(OBJS_RPI)

deploy: build/QuartoRPI .deployed-lib .deployed
	ssh $(RPI_ADDRESS) "export DISPLAY=:0; lxterminal --command=./run.sh"

deploy-lib: .deployed-lib

deploy-here: .deployed .deployed-lib
	ssh -X $(RPI_ADDRESS) "lxterminal --command=./run.sh"

.deployed: build/QuartoRPI
	touch .deployed
	rsync build/QuartoRPI $(RPI_ADDRESS):/home/pi/
  
.deployed-lib: | lib/rpi/*
	touch .deployed-lib
	rsync -rauL --info=progress2 lib/rpi/* $(RPI_ADDRESS):/home/pi

force:
	make clean
	make

clean:
	rm -rf build/*.o