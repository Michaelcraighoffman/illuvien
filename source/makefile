CC=g++
OBJECTS = anim.o battle.o engine.o error.o fps.o globals.o inventory.o loadpng.o main.o map.o mapxml.o messagebox.o monsterai.o monsters.o monsters_xml.o options.o player.o regpng.o savepng.o xml.o
LIBS = -lz -lpng `sdl2-config --cflags --libs`
CPPFLAGS = -Wall -O3 -std=c++11
outputdir = ../output/

illuvien : $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(outputdir)illuvien $(OBJECTS) $(LIBS)
install : 
	ln -s ./output/illuvien ../Illuvien

anim.o : globals.h 
battle.o : 
engine.o : anim.h error.h fps.h globals.h map.h options.h player.h xml.h
error.o : globals.h
fps.o : 
globals.o :
loadpng.o : loadpng.h
main.o : engine.h globals.h error.h options.h
map.o : globals.h xml.h loadpng.h
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
