#Made by Butilca Rares
# set up the compiler
CC=gcc
CFLAGS=-Wall -Wextra -std=c99
LIBFLAGS= -lm

# define target and dependencies
TARGET=image_editor
CFILES=image_editor.c load.c select.c rotate.c solitary_functions.c save.c utilitary.c
HEADERS=image_editor.h

build: $(TARGET)

image_editor: $(CFILES) $(HEADERS)
	$(CC) $(CFLAGS) $(CFILES) -o $(TARGET) $(LIBFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
