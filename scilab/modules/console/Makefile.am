##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Vincent Couvert <vincent.couvert@inria.fr>
### INRIA - Scilab 2007
##########

if GUI

# List of the cpp files
CONSOLE_CPP_SOURCES =  src/jni/ShellRead.cpp 

# List of the c files
CONSOLE_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c \
src/jni/GuiManagement_wrap.c

CONSOLE_C_SOURCES = src/c/InterpreterManagement.c \
src/c/GuiManagement.c \
src/c/ClearConsole.c \
src/c/ClearConsolePart.c \
src/c/PromptToHome.c \
src/c/ShellPrintf.c

else

# List of the c files
CONSOLE_C_SOURCES = src/noconsole/noconsole.c

endif

# Includes need for the compilation
libsciconsole_la_CPPFLAGS =  $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/cpp/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

# Includes need for the compilation
libsciconsole_la_CFLAGS=  $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/ 

# Name of the library
pkglib_LTLIBRARIES = libsciconsole.la

# All the sources needed by libsciconsole.la
libsciconsole_la_SOURCES = $(CONSOLE_JNI_SOURCES) $(CONSOLE_C_SOURCES) $(CONSOLE_CPP_SOURCES)

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

#### SWIG Declaration ####
BUILT_SOURCES = src/jni/InterpreterManagement.i \
src/jni/GuiManagement.i

#### console : include files ####
libscishell_la_includedir=$(pkgincludedir)/console
libscishell_la_include_HEADERS = includes/ShellRead.h \
includes/ShellPrintf.h


# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, add USEANT=1

if GUI
USEANT=1
endif

include $(top_srcdir)/Makefile.incl.am

