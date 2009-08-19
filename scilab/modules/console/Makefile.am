#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006-2008 - Sylvestre Ledru
# Copyright (C) INRIA   - 2007 - Vincent COUVERT
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#


#### Target ######
modulename=console

#### console : Conf files ####
libsciconsole_la_rootdir = $(mydatadir)
libsciconsole_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### console : init scripts ####
libsciconsole_la_etcdir = $(mydatadir)/etc
libsciconsole_la_etc_DATA = etc/console.quit etc/console.start etc/configuration.xml



#### console : List of the cpp files ####
CONSOLE_CPP_JNI_SOURCES = src/jni/CallScilabBridge.cpp \
src/jni/GiwsException.cpp

CONSOLE_CPP_SOURCES =  src/cpp/ClearConsole.cpp \
src/cpp/ClearConsolePart.cpp \
src/cpp/ConsolePrintf.cpp \
src/cpp/ConsoleRead.cpp \
src/cpp/ConsoleIsWaitingForInput.cpp \
src/cpp/GetCharWithoutOutput.cpp \
src/cpp/PromptToHome.cpp \
src/cpp/ScilabLinesUpdate.cpp \
src/cpp/SetConsolePrompt.cpp

#### console : List of the c files ####
CONSOLE_C_JNI_SOURCES = src/jni/GuiManagement_wrap.c \
src/jni/DropFiles_wrap.c

CONSOLE_C_SOURCES = src/c/InitializeConsole.c \
src/c/GuiManagement.c \
src/c/dropFiles.c

GIWS_WRAPPERS = src/jni/CallScilabBridge.giws.xml

BUILT_SOURCES=

if GIWS
BUILT_SOURCES+=giws
endif

if SWIG
BUILT_SOURCES+=swig
endif


# List of the c files
libsciconsole_disable_la_SOURCES = src/noconsole/noconsole.c
libsciconsole_disable_la_CFLAGS = -Iincludes/


# Includes need for the compilation
libsciconsole_la_CPPFLAGS =  $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/modules/api_scilab/includes -I$(srcdir)/includes/ -I$(srcdir)/src/cpp/ -I$(srcdir)/src/c/ -I$(srcdir)/src/jni/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/jvm/includes/ -I$(top_srcdir)/modules/action_binding/includes

# Includes need for the compilation
libsciconsole_la_CFLAGS=  	$(JAVA_JNI_INCLUDE) \
				-Iincludes/ \
				-Isrc/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes

# Name of the library
pkglib_LTLIBRARIES = libsciconsole-disable.la

if GUI
pkglib_LTLIBRARIES += libsciconsole.la 
endif

libsciconsole_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

# All the sources needed by libsciconsole.la
libsciconsole_la_SOURCES = $(CONSOLE_C_JNI_SOURCES) $(CONSOLE_C_SOURCES) $(CONSOLE_CPP_JNI_SOURCES) $(CONSOLE_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(CONSOLE_C_JNI_SOURCES) $(CONSOLE_C_SOURCES)
INCLUDE_FLAGS = $(libsciconsole_la_CFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libsciconsole_la_LIBADD =  $(top_builddir)/modules/action_binding/libsciaction_binding.la $(top_builddir)/modules/shell/libscishell.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/libs/MALLOC/libscimalloc.la 
# java code is needed $(top_builddir)/modules/jvm/libscijvm.la


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/GuiManagement.i

#### console : include files ####
libsciconsole_la_includedir=$(pkgincludedir)/console
libsciconsole_la_include_HEADERS = includes/ClearConsole.h \
includes/ClearConsolePart.h \
includes/ConsolePrintf.h \
includes/ConsoleRead.h \
includes/GetCharWithoutOutput.h \
includes/InitializeConsole.h \
includes/PromptToHome.h \
includes/ScilabLinesUpdate.h \
includes/SetConsolePrompt.h \
includes/ClearConsole.hxx \
includes/ClearConsolePart.hxx \
includes/ConsoleIsWaitingForInput.h \
includes/ConsoleIsWaitingForInput.hxx \
includes/ConsolePrintf.hxx \
includes/ConsoleRead.hxx \
includes/GetCharWithoutOutput.hxx \
includes/PromptToHome.hxx \
includes/ScilabLinesUpdate.hxx \
includes/SetConsolePrompt.hxx

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, add USEANT=1

if GUI
USEANT=1
endif

include $(top_srcdir)/Makefile.incl.am
