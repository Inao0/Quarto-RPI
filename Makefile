INCLUDE = -I./include
LIB = -L./

INCLUDE_PC = -I$(TARGET_NPC)/include -I$(TARGET_CDKPC)/include -I$(TARGET_CDKPC)/include/cdk $(INCLUDE)
LIB_PC = -L$(TARGET_NPC)/lib -L$(TARGET_CDKPC)/lib $(LIB)
CFLAGS_PC = -Wall -Wextra $(INCLUDE_PC) -Winline -pipe
LDFLAGS_PC = $(LIB_PC) -lncurses -lcdk -ldl

INCLUDE_PI = -I$(TARGET_NPI)/include $(INCLUDE)
LIB_PI = -L$(TARGET_NPI)/lib $(LIB)
CFLAGS_PI = -Wall -Wextra $(INCLUDE_PI) -Winline -pipe
LDFLAGS_PI = $(LIB_PI) -lncurses -lcdk -ldl
LD_LIBRARY_PATH = /home/haran/Documents/Centrale/G3/IHM/Quarto-RPI/lib/pc/cdk-5.0/lib

ifeq ($(TARGET_NPI),)
	TARGET_NPI=./lib/pi/ncurses-6.2
endif

ifeq ($(TARGET_NPC),)
	TARGET_NPC=./lib/pc/ncurses-6.2
endif

ifeq ($(TARGET_NPI),)
	TARGET_NPI=./lib/pi/cdk-5.0
endif

ifeq ($(TARGET_CDKPC),)
	TARGET_CDKPC=./lib/pc/cdk-5.0
endif

ifeq ($(TARGET_WPI),)
	TARGET_WPI=./lib/wiringPi
endif

all: pc rpi

pc:
	gcc $(CFLAGS_PC) src/main.c $(LDFLAGS_PC) -o build/quarto_pc.o

rpi:
	$(RPI_COMPILER) $(CFLAGS_PI) src/main.c $(LDFLAGS_PI) -o build/quarto_pi.o

clean:
	rm -rf build/*.o