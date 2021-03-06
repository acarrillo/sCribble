CFLAGS = -Werror -Wall
LFLAGS = `sdl-config --libs` -lSDL
OBJS   = client.o file.o init.o input.o graphics.o main.o
S_OBJS = src/server/server.o src/server/listener.o src/server/subserver.o
PROG = sCribble
S_PROG = sCribbleServer

# top-level rule to create the program.
all: $(PROG) server

# standalone client
client: client.o
	gcc client.o -o client

# compiling server source files.
src/server/%.o: %.c %.h src/defs.h
	gcc $(CFLAGS) -c -g -s $<

# compiling source files.
%.o: src/%.c src/%.h src/defs.h
	gcc $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	gcc $(OBJS) -o $(PROG) $(LFLAGS)

# linking the server
server: $(S_OBJS)
	gcc $(S_OBJS) -o $(S_PROG)

# cleaning everything that can be automatically recreated with "make".
clean: cleanS
	rm $(PROG) *.o

# cleaning everything that can be automatically recreated with "make server".
cleanS:
	rm $(S_PROG) src/server/*.o

cleanC:
	rm client client.o
