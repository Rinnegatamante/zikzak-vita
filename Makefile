GCC=g++
COMPILERFLAGS=-Wall -L/usr/X11R6/lib -L/usr/lib -I/usr/include -I/usr/include/SDL -O0 -fmessage-length=0 `sdl-config --cflags`
LINKERFLAGS=`sdl-config --libs` -lGL -lGLU -lSDL_mixer 
OBJS=Config.o Bullet.o Dynent.o Gamestate.o Graphics.o Input.o Level.o Main.o Player.o Sound.o Coord3d.o
BINARY=zikzak

default: all

%.o: %.cpp Gfx.h
	$(GCC) $(COMPILERFLAGS) -c -o $@ $<

all: $(OBJS)
	$(GCC) $(LINKERFLAGS) -o $(BINARY) $(OBJS)

clean:
	-rm -f $(OBJS) $(BINARY)

