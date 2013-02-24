CC=gcc
CFLAGS=-std=gnu99 -fgnu89-inline -c -Wall -O3 -I/usr/local/include -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux/ -I/opt/vc/include/interface/vcos/pthreads/ -Iinclude/ngl -Isrc/ngl -Isrc/shaders -DNGL_DEBUG
LDFLAGS=-L/opt/vc/lib -lwiringPi -lEGL -lGLESv2 -lbcm_host
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:%.c=%.o)
TARGET=main

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o $(TARGET)
