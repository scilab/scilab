include ../../Makefile.incl 

# imlib-config --libs-gdk is needed for some gnome versions 
LDFLAGS= `gnome-config --libs zvt`   `imlib-config --libs-gdk`
#CFLAGS=  `gtk-config --cflags` `gnome-config --cflags gnome` 
CFLAGS=  `gnome-config --cflags gnome` `imlib-config --cflags`

all	::  ../../bin/zterm gtkplug 

clean :: 
	@$(RM) -f zterm gtkplug 


../../bin/zterm : zterm.c 
	$(CC) $(CFLAGS) -o ../../bin/zterm zterm.c $(LDFLAGS) 

distclean::
	$(RM) -f ../../bin/zterm gtkplug 	
