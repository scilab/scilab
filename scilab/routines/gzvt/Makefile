


all	::
	gcc -o zterm zterm.c `gtk-config --cflags` `gnome-config --libs zvt` `imlib-config --libs-gdk`

all	::
	gcc gtkplug.c -o gtkplug -O2 -Wall -g `gtk-config --cflags --libs`


distclean : 
	\rm -f zterm gtkplug gtksocket *.o

