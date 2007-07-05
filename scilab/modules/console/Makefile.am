##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Vincent Couvert <vincent.couvert@inria.fr>
### INRIA - Scilab 2007
##########

if GUI
# List of the c files
CONSOLE_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c \
src/jni/HistoryManagement_wrap.c \
src/jni/GuiManagement_wrap.c

CONSOLE_C_SOURCES = src/c/InterpreterManagement.c \
src/c/HistoryManagement.c \
src/c/GuiManagement.c
endif

# Includes need for the compilation
libsciconsole_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/ 

# Name of the library
pkglib_LTLIBRARIES = libsciconsole.la

# All the sources needed by libsciconsole.la
libsciconsole_la_SOURCES = $(CONSOLE_JNI_SOURCES) $(CONSOLE_C_SOURCES)

## Libraries necessary
## Change to include other Scilab modules/lib
libsciconsole_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Name of the module and the path ######
modulename=console

#### console : Conf files ####
libsciconsole_la_rootdir = $(mydatadir)
libsciconsole_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### console : init scripts #### 
libsciconsole_la_etcdir = $(mydatadir)/etc
libsciconsole_la_etc_DATA = etc/console.quit etc/console.start

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
