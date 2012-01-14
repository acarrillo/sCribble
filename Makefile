CFLAGS = -Werror -Wall
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image
OBJS   = init.o input.o graphics.o main.o
PROG = sCribble

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h
	gcc $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	gcc $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o
