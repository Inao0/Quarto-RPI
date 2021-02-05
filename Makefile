INCLUDE = -I./include
LIB = -L./

INCLUDE_NCURSES_PC = -I$(TARGET_NPC)/include $(INCLUDE)
LIB_NCURSES_PC = -L$(TARGET_NPC)/lib $(LIB)
CFLAGS_PC = -Wall -Wextra $(INCLUDE_NCURSES_PC) -Winline -pipe
LDFLAGS_PC = $(LIB_NCURSES_PC) -lncurses -ldl -lSDL2 -lm

INCLUDE_NCURSES_PI = -I$(TARGET_NPI)/include $(INCLUDE)
LIB_NCURSES_PI = -L$(TARGET_NPI)/lib $(LIB)
CFLAGS_PI = -Wall -Wextra $(INCLUDE_NCURSES_PI) -Winline -pipe
LDFLAGS_PI = $(LIB_NCURSES_PI) -lncurses -ldl -lSDL2 -lm

ifeq ($(TARGET_NPI),)
	TARGET_NPI=./lib/pi/ncurses-6.2/
endif

ifeq ($(TARGET_NPC),)
	TARGET_NPC=./lib/pc/ncurses-6.2/
endif

ifeq ($(TARGET_WPI),)
	TARGET_WPI=./lib/wiringPi/
endif

all: PC RPI

PC:
	gcc $(CFLAGS_PC) src/main.c src/sdl_helper.c $(LDFLAGS_PC) -o build/quarto_pc.o

RPI:
	$(RPI_COMPILER) $(CFLAGS_PI) src/main.c $(LDFLAGS_PI) -o build/quarto_pi.o

clean:
	rm -rf build/*.o