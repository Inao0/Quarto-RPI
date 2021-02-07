INCLUDE = -I./include
LIB = -L./
RPI_ADDRESS = pi@192.168.1.52

INCLUDE_PC = -I$(TARGET_NPC)/include -I$(TARGET_CDKPC)/include -I$(TARGET_CDKPC)/include/cdk $(INCLUDE)
LIB_PC = -L$(TARGET_NPC)/lib -L$(TARGET_CDKPC)/lib $(LIB)
CFLAGS_PC = -Wall -Wextra $(INCLUDE_PC) -Winline -pipe
LDFLAGS_PC = $(LIB_PC) -lncurses -lcdk -ldl

INCLUDE_PI = -I$(TARGET_NPI)/include -I$(TARGET_NPI)/include/ncurses -I$(TARGET_WPI)/include -I$(TARGET_CDKPI)/include -I$(TARGET_CDKPI)/include/cdk $(INCLUDE)
LIB_PI = -L$(TARGET_NPI)/lib -L$(TARGET_CDKPI)/lib -L$(TARGET_WPI)/lib $(LIB)
CFLAGS_PI = -Wall -Wextra $(INCLUDE_PI) -Winline -pipe
LDFLAGS_PI = $(LIB_PI) -lncurses -lcdk -ldl

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

ifeq ($(TARGET_CDKPI),)
	TARGET_CDKPI=./lib/pi/cdk-5.0
endif

ifeq ($(TARGET_WPI),)
	TARGET_WPI=./lib/pi/wiringPi
endif

all: pc rpi modules

modules: build/modules_pc.o build/modules_rpi.o

pc: build/QuartoPC

build/QuartoPC: build/modules_pc.o src/main.c
	gcc $(CFLAGS_PC) src/main.c $(LDFLAGS_PC) -o build/main_pc.o -c
	gcc $(CFLAGS_PC) $(LDFLAGS_PC) -o build/QuartoPC build/modules_pc.o build/main_pc.o

build/modules_pc.o: src/cdk_helper.c
	gcc $(CFLAGS_PC) src/cdk_helper.c $(LDFLAGS_PC) -o build/modules_pc.o -c

run: ./build/QuartoPC
	./build/QuartoPC

rpi: build/QuartoPI

build/QuartoPI: build/modules_pi.o src/main.c
	$(RPI_COMPILER) $(CFLAGS_PI) src/main.c $(LDFLAGS_PI) -o build/main_pi.o -c
	$(RPI_COMPILER) $(CFLAGS_PI) $(LDFLAGS_PI) -o build/QuartoPI build/modules_pi.o build/main_pi.o

build/modules_pi.o: src/cdk_helper.c
	$(RPI_COMPILER) $(CFLAGS_PI) src/cdk_helper.c $(LDFLAGS_PI) -o build/modules_pi.o -c

deploy: build/QuartoPI deploy-lib
	rsync build/QuartoPI $(RPI_ADDRESS):/home/pi/
	ssh $(RPI_ADDRESS) "export DISPLAY=:0; lxterminal --command=./run.sh"

force:
	make clean
	make

clean:
	rm -rf build/*.o