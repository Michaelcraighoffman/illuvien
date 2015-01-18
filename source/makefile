CC=g++
OBJECTS = anim.o battle.o engine.o error.o fontmanager.o fps.o globals.o inventory.o main.o map.o mapxml.o messagebox.o monsterai.o monsters.o monsters_xml.o options.o player.o xml.o
LIBS = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
CPPFLAGS = -Wall -O3 -std=c++11 -I /usr/include/SDL2 -I /usr/local/include/SDL2 -L /usr/local/lib
outputdir = ../output/

illuvien : $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(outputdir)illuvien $(OBJECTS) $(LIBS)
install : 
	ln -s ./output/illuvien ../Illuvien

anim.o : globals.h fontmanager.h
battle.o : 
engine.o : anim.h error.h fps.h globals.h map.h options.h player.h xml.h
error.o : globals.h
fontmanager.o : fontmanager.h
fps.o : 
globals.o :
main.o : engine.h globals.h error.h options.h
map.o : globals.h xml.h
mapxml.o : globals.h xml.h
messagebox.o : 
monsterai.o : monsters.h
monsters.o :
monsters_xml.o : monsters.h
options.o : 
player.o : globals.h anim.h
xml.o : error.h

clean : 
	rm *.o
