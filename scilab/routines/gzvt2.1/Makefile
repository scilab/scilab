include ../../Makefile.incl 

XLDFLAGS=pkg-config libzvt-2.0  --cflags --libs
XCFLAGS=pkg-config libzvt-2.0  --cflags

LDFLAGS=`$(XLDFLAGS)`
CFLAGS=-O `$(XCFLAGS)` -I../include 

all	::  ../../bin/zterm 

../../bin/zterm : zterm.c 
	$(CC) $(CFLAGS) -o ../../bin/zterm zterm.c $(LDFLAGS) 

test	::  child  zterm

clean :: 
	@$(RM) -f *.o child zterm 

distclean::
	$(RM) -f ../../bin/zterm zterm child 








