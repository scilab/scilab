include ../../Makefile.incl 

# imlib-config --libs-gdk is needed for some gnome versions 
LDFLAGS= `pkg-config libzvt-2.0 --cflags --libs` -DHAS_I18N
CFLAGS= `pkg-config libzvt-2.0 --cflags ` -DHAS_I18N

all	::  ../../bin/zterm 

clean :: 
	@$(RM) -f zterm gtkplug 

../../bin/zterm : zterm.c 
	cc -o  ../../bin/zterm zterm.c $(LDFLAGS) 

distclean::
	$(RM) -f ../../bin/zterm gtkplug 	



