INCPATH = -I /usr/X11R6/include/ -I .
LIBS = -lglut -lGLU -lGL -lXi -lX11 -lm -lpthread -ljpeg -lGLEW
LIBPATH = -L/usr/lib -L/usr/X11R6/lib/
CXXFLAGS = -g -Wall -DGL_SILENCE_DEPRECATION

all:	 eitsimGL Makefile Physics1.h Physics2.h Physics3.h

mantle:	mantle.cpp Makefile Physics1.h Physics2.h Physics3.h
	g++ $(INCPATH) $(LIBPATH) mantle.cpp $(LIBS) $(CXXFLAGS) -o mantle

eitsimGL:	eitsimGL.cpp Makefile Physics1.h Physics2.h Physics3.h
	g++ $(INCPATH) $(LIBPATH) eitsimGL.cpp $(LIBS) $(CXXFLAGS) -o eitsimGL

Trabalho:	Trabalho.cpp Makefile Physics1.h Physics2.h Physics3.h
	g++ $(INCPATH) $(LIBPATH) Trabalho.cpp $(LIBS) $(CXXFLAGS) -o Trabalho

clean:
	rm -rfv mostra *~ eitsimGL.exe MinGW  eitsimGL mantle *.dSYM

distclean:	clean
	rm -rfv save/*.bmp

win:
	rm -rf eitsimGL.exe
	wineconsole --backend=curses compila.bat 
	rm -rf MinGW

