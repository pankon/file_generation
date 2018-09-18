#template generation tool
CC = /usr/bin/gcc
MAKE = /usr/bin/make
RELEASE_FLAGS = -O3 -DNDEBUG -fPIC # O3 is an optimizer
CFLAGS= -Wall -ansi -std=c89 -pedantic -Werror
FILES=file_generator.c file_generator.h file_generator_test.c file_generator_test.h template.c template.h

all: generate
	
generate: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o generate
	
clean:
	rm generate
