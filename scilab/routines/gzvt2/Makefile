include ../../Makefile.incl 

# imlib-config --libs-gdk is needed for some gnome versions 
LDFLAGS= `pkg-config libzvt-2.0 --cflags --libs` 
#CFLAGS=  `gtk-config --cflags` `gnome-config --cflags gnome` 
CFLAGS= `pkg-config libzvt-2.0 --cflags ` 

all	::  ../../bin/zterm 

clean :: 
	@$(RM) -f zterm gtkplug 

../../bin/zterm : zterm.c 
	cc -o  ../../bin/zterm zterm.c `pkg-config libzvt-2.0 --cflags --libs` 

distclean::
	$(RM) -f ../../bin/zterm gtkplug 	



