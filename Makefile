INCLUDE = -I./include
LIB = -L./
SRCS = $(wildcard src/*.c)
OBJS = $(subst src/,build/,$(patsubst %.c,%.o,$(SRCS)))
OBJS_PC = $(subst build/,build/pc-,$(OBJS))
OBJS_PI = $(subst build/,build/pi-,$(OBJS))

INCLUDE_PC = -I$(TARGET_NPC)/include -I$(TARGET_CDKPC)/include -I$(TARGET_CDKPC)/include/cdk $(INCLUDE)
LIB_PC = -L$(TARGET_NPC)/lib -L$(TARGET_CDKPC)/lib $(LIB)
CFLAGS_PC = -Wall -Wextra $(INCLUDE_PC) -Winline -pipe -DBUILD_PC
LDFLAGS_PC = $(LIB_PC) -lncurses -lcdk -ldl

INCLUDE_PI = -I$(TARGET_NPI)/include -I$(TARGET_NPI)/include/ncurses -I$(TARGET_WPI)/include -I$(TARGET_CDKPI)/include -I$(TARGET_CDKPI)/include/cdk $(INCLUDE)
LIB_PI = -L$(TARGET_NPI)/lib -L$(TARGET_CDKPI)/lib -L$(TARGET_WPI)/lib $(LIB)
CFLAGS_PI = -Wall -Wextra $(INCLUDE_PI) -Winline -pipe
LDFLAGS_PI = $(LIB_PI) -lncurses -lcdk -ldl -lwiringPi

ifeq ($(TARGET_NPC),)
	TARGET_NPC=./lib/pc/ncurses-6.2
endif

ifeq ($(TARGET_CDKPC),)
	TARGET_CDKPC=./lib/pc/cdk-5.0
endif

ifeq ($(TARGET_NPI),)
	TARGET_NPI=./lib/pi/ncurses-6.2
endif

ifeq ($(TARGET_CDKPI),)
	TARGET_CDKPI=./lib/pi/cdk-5.0
endif

ifeq ($(TARGET_WPI),)
	TARGET_WPI=./lib/pi/wiringPi/build
endif

all: pc rpi

pc: build/QuartoPC

build/QuartoPC: $(OBJS_PC)
	$(CC) $(CFLAGS_PC) $(LDFLAGS_PC) -o build/QuartoPC $(OBJS_PC)

build/pc-%.o: src/%.c
	$(CC) -c $(CFLAGS_PC) $(LDFLAGS_PC) -o $@ $<

run: ./build/QuartoPC
	./build/QuartoPC

rpi: build/QuartoPI

build/QuartoPI: $(OBJS_PI)
	$(RPI_COMPILER) $(CFLAGS_PI) $(LDFLAGS_PI) -o build/QuartoPI $(OBJS_PI)

build/pi-%.o: src/%.c
	$(RPI_COMPILER) -c $(CFLAGS_PI) $(LDFLAGS_PI) -o $@ $<

deploy: build/QuartoPI .deployed-lib .deployed
	ssh $(RPI_ADDRESS) "export DISPLAY=:0; lxterminal --command=./run.sh"

deploy-here: .deployed .deployed-lib
	ssh -X $(RPI_ADDRESS) "lxterminal --command=./run.sh"

.deployed: build/QuartoPI
	touch .deployed
	rsync build/QuartoPI $(RPI_ADDRESS):/home/pi/

.deployed-lib: | lib/pi/ncurses-6.2/ lib/pi/wiringPi/ lib/pi/cdk-5.0/
	touch .deployed-lib
	rsync -rauL lib/pi/ncurses-6.2/ pi@192.168.1.52:/home/pi/ncurses-6.2/
	rsync -rauL lib/pi/wiringPi/ pi@192.168.1.52:/home/pi/wiringPi/
	rsync -rauL lib/pi/cdk-5.0/ pi@192.168.1.52:/home/pi/cdk-5.0/

force:
	make clean
	make

clean:
	rm -rf build/*.o