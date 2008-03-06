##########
### Allan CORNET
### INRIA - Scilab 2007 
##########

DOUBLYLINKEDLIST_C_SOURCES = src/DoublyLinkedList.c

libscidoublylinkedlist_la_includedir=$(pkgincludedir)/doublylinkedlist

libscidoublylinkedlist_la_include_HEADERS = includes/DoublyLinkedList.h

libscidoublylinkedlist_la_CFLAGS= -Isrc/ -Iincludes/

pkglib_LTLIBRARIES = libscidoublylinkedlist.la

libscidoublylinkedlist_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidoublylinkedlist_la_SOURCES = $(DOUBLYLINKEDLIST_C_SOURCES)

libscidoublylinkedlist_la_LIBADD = 

#### Target ######
modulename=doublylinkedlist

include $(top_srcdir)/Makefile.incl.am
