#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006-2008 - Sylvestre Ledru
# Copyright (C) INRIA   - 2007 - Vincent COUVERT
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# Copyright (C) DIGITEO - 2011 - Allan CORNET
# Copyright (C) DIGITEO - 2011 - Antoine ELIAS
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#


#### Target ######
modulename=console

#### console : Conf files ####
libsciconsole_la_rootdir = $(mydatadir)
libsciconsole_la_root_DATA =  license.txt

#### console : init scripts ####
libsciconsole_la_etcdir = $(mydatadir)/etc
libsciconsole_la_etc_DATA = \
	etc/console.quit \
	etc/console.start \
	etc/configuration.xml \
	etc/XConfiguration-console.xml \
	etc/XConfiguration-console.xsl \
	etc/XConfiguration-general.xml \
	etc/XConfiguration-general.xsl \
	etc/Actions-Configuration.xml

#### console : List of the cpp files ####
CONSOLE_CPP_JNI_SOURCES = src/jni/CallScilabBridge.cpp

CONSOLE_CPP_SOURCES = \
    src/cpp/ClearConsole.cpp \
    src/cpp/ClearConsolePart.cpp \
    src/cpp/ConsolePrintf.cpp \
    src/cpp/ConsoleRead.cpp \
    src/cpp/ConsoleIsWaitingForInput.cpp \
    src/cpp/GetCharWithoutOutput.cpp \
    src/cpp/PromptToHome.cpp \
    src/cpp/ScilabLinesUpdate.cpp \
    src/cpp/SetConsolePrompt.cpp

#### console : List of the c files ####
CONSOLE_C_JNI_SOURCES = \
    src/jni/GuiManagement_wrap.c \
    src/jni/DropFiles_wrap.c

GIWS_WRAPPERS = src/jni/CallScilabBridge.giws.xml

BUILT_SOURCES=

if GIWS
BUILT_SOURCES+=giws
endif

if SWIG
BUILT_SOURCES+=swig
endif

# List of the c files

libsciconsole_minimal_la_SOURCES = \
    src/noconsole/noconsole.c \
    sci_gateway/cpp/sci_clc.cpp \
    sci_gateway/cpp/sci_tohome.cpp \
    sci_gateway/cpp/sci_lines.cpp \
    sci_gateway/cpp/sci_prompt.cpp \
    sci_gateway/cpp/sci_iswaitingforinput.cpp \
    src/c/clc.c \
    src/c/tohome.c \
    src/c/more.c \
    src/c/scilines.c \
    src/c/others/gotoxy_nw.c \
    src/c/others/clrscr_nw.c \
    src/c/prompt.c \
    src/c/promptecho.c \
    src/c/getpipeline.c \
    src/c/cmdLine/autoCompletionCli.c \
    src/c/cmdLine/charactersManagement.c \
    src/c/cmdLine/cliDisplayManagement.c \
    src/c/cmdLine/cliHistory.c \
    src/c/cmdLine/cliPrompt.c \
    src/c/cmdLine/getKey.c \
    src/c/cmdLine/getNbrLine.c \
    src/c/cmdLine/gotoFunctions.c \
    src/c/cmdLine/initConsoleMode.c \
    src/c/cmdLine/termcapManagement.c \
    src/cpp/scilabRead.cpp \
    sci_gateway/cpp/console_gw.cpp

CONSOLE_C_SOURCES = \
    src/c/GuiManagement.c \
    src/c/dropFiles.c

if IS_MACOSX
CONSOLE_C_SOURCES += src/c/others/initMacOSXEnv.c
endif


libsciconsole_minimal_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/history_manager/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/completion/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/action_binding/includes/ \
    -I$(top_srcdir)/modules/fileio/src/c/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/ui_data/includes/ \
    -I$(top_srcdir)/modules/jvm/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(AM_CPPFLAGS)

# Includes need for the compilation
libsciconsole_la_CPPFLAGS = \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/action_binding/includes \
    -I$(top_srcdir)/modules/history_manager/includes/ \
    -I$(top_srcdir)/modules/ui_data/includes/ \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/completion/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/commons/src/jni \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(JAVA_JNI_INCLUDE) \
    $(AM_CPPFLAGS)


# FIXME: Plateform specific optimisation flags should be set on configure.ac
if IS_MACOSX
libsciconsole_la_CPPFLAGS += -msse2
endif

# Name of the library
pkglib_LTLIBRARIES = libsciconsole-minimal.la

if GUI
pkglib_LTLIBRARIES += libsciconsole.la
endif

libsciconsole_la_LIBADD = libsciconsole-minimal.la

# All the sources needed by libsciconsole.la
libsciconsole_la_SOURCES = $(CONSOLE_C_JNI_SOURCES) $(CONSOLE_C_SOURCES) $(CONSOLE_CPP_JNI_SOURCES) $(CONSOLE_CPP_SOURCES) $(CONSOLE_GATEWAY_C_SOURCES) $(CONSOLE_GATEWAY_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(CONSOLE_C_JNI_SOURCES) $(CONSOLE_C_SOURCES)
INCLUDE_FLAGS = $(libsciconsole_la_CPPFLAGS)

#### console : gateway declaration ####
libsciconsole_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciconsole_la_sci_gateway_DATA = sci_gateway/console_gateway.xml

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/GuiManagement.i \
	src/jni/DropFiles.i

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, add USEANT=1

if GUI
USEANT=1
endif

#### console : include files ####
libsciconsole_la_includedir=$(pkgincludedir)
libsciconsole_la_include_HEADERS = \
includes/autoCompletionCli.h \
includes/charactersManagement.h \
includes/cliDisplayManagement.h \
includes/cliHistory.h \
includes/cliPrompt.h \
includes/console_gw.hxx \
includes/ConsolePrintf.h \
includes/ConsoleRead.h \
includes/dynlib_console_gw.h \
includes/dynlib_console.h \
includes/getKey.h \
includes/getNbrLine.h \
includes/gotoFunctions.h \
includes/gw_console.h \
includes/initConsoleMode.h \
includes/InitializeConsole.h \
includes/initMacOSXEnv.h \
includes/more.h \
includes/prompt.h \
includes/scilabRead.h \
includes/scilines.h \
includes/SetConsolePrompt.h \
includes/termcapManagement.h


include $(top_srcdir)/Makefile.incl.am
