##########
### Allan CORNET
### INRIA - Scilab 2007 
##########

DOUBLYLINKEDLIST_C_SOURCES = src/DoublyLinkedList.c

libscidoublylinkedlist_la_includedir=$(pkgincludedir)

libscidoublylinkedlist_la_include_HEADERS = includes/DoublyLinkedList.h

libscidoublylinkedlist_la_CFLAGS= -I$(srcdir)/src/ -I$(srcdir)/includes/

pkglib_LTLIBRARIES = libscidoublylinkedlist.la

libscidoublylinkedlist_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidoublylinkedlist_la_SOURCES = $(DOUBLYLINKEDLIST_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(DOUBLYLINKEDLIST_C_SOURCES)
INCLUDE_FLAGS = $(libscidoublylinkedlist_la_CFLAGS)

libscidoublylinkedlist_la_LIBADD = 

#### Target ######
modulename=doublylinkedlist

include $(top_srcdir)/Makefile.incl.am
