include ../../Makefile.incl 

LDFLAGS= `gtk-config --libs`  `gnome-config --libs zvt`  
# `imlib-config --libs-gdk`
CFLAGS=  `gtk-config --cflags` 

all	::  ../../bin/zterm gtkplug 

clean :: 
	@$(RM) -f zterm gtkplug 


../../bin/zterm : zterm.c 
	$(CC) $(CFLAGS) -o ../../bin/zterm zterm.c $(LDFLAGS) 

distclean::
	$(RM) -f ../../bin/zterm gtkplug 	