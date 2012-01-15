CFLAGS = -Werror -Wall -I./lib/
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_ttf -lSDL_image
OBJS   = init.o input.o graphics.o main.o client.o
PROG = sCribble

# top-level rule to create the program.
all: $(PROG) server

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h
	gcc $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	gcc $(OBJS) -o $(PROG) $(LFLAGS)

# Cribble server
server: src/server.c
	gcc src/server.c -o cribbleServer

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) cribbleServer *.o
