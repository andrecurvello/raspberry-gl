CC=gcc
CFLAGS=-c -Wall -O3 -I/usr/local/include -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads/ -Iinclude/ngl -Isrc/ngl -Isrc/shaders -DNGL_DEBUG
LDFLAGS=-L/opt/vc/lib -lEGL -lGLESv2 -lbcm_host
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:%.c=%.o)
TARGET=main

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o $(TARGET)
