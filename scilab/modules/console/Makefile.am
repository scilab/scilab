#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006-2008 - Sylvestre Ledru
# Copyright (C) INRIA   - 2007 - Vincent COUVERT
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# Copyright (C) DIGITEO - 2011 - Allan CORNET
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
CONSOLE_CXX_JNI_SOURCES = src/jni/CallScilabBridge.cpp

CONSOLE_CXX_SOURCES =  src/cpp/ClearConsole.cpp \
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

# List of the gateway c files
CONSOLE_GATEWAY_C_SOURCES = sci_gateway/c/gw_console.c \
sci_gateway/c/sci_clc.c \
sci_gateway/c/sci_tohome.c \
sci_gateway/c/sci_lines.c \
sci_gateway/c/sci_prompt.c \
sci_gateway/c/sci_iswaitingforinput.c

GIWS_WRAPPERS = src/jni/CallScilabBridge.giws.xml

BUILT_SOURCES=

if GIWS
BUILT_SOURCES+=giws
endif

if SWIG
BUILT_SOURCES+=swig
endif


# List of the c files
libsciconsole_minimal_la_SOURCES = src/noconsole/noconsole.c \
sci_gateway/c/gw_console.c \
sci_gateway/c/sci_clc.c \
sci_gateway/c/sci_tohome.c \
sci_gateway/c/sci_lines.c \
sci_gateway/c/sci_prompt.c \
sci_gateway/c/sci_iswaitingforinput.c \
src/c/clc.c \
src/c/tohome.c \
src/c/more.c \
src/c/scilines.c \
src/c/GetCommandLine.c \
src/c/others/gotoxy_nw.c \
src/c/others/clrscr_nw.c \
src/c/prompt.c \
src/c/promptecho.c \
src/c/cmdLine/autoCompletionCli.c \
src/c/cmdLine/charactersManagement.c \
src/c/cmdLine/cliDisplayManagement.c \
src/c/cmdLine/cliHistory.c \
src/c/cmdLine/cliPrompt.c \
src/c/cmdLine/getKey.c \
src/c/cmdLine/getNbrLine.c \
src/c/cmdLine/gotoFunctions.c \
src/c/cmdLine/initConsoleMode.c \
src/c/cmdLine/termcapManagement.c


CONSOLE_C_SOURCES = src/c/GuiManagement.c \
	src/c/dropFiles.c
if IS_MACOSX
CONSOLE_C_SOURCES += src/c/others/initMacOSXEnv.c
endif

if IS_MACOSX
libsciconsole_minimal_la_SOURCES += src/c/others/initMacOSXEnv.c
endif


libsciconsole_minimal_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
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
				$(AM_CPPFLAGS)

# Includes need for the compilation
libsciconsole_la_CPPFLAGS= -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(srcdir)/src/c/cmdLine \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/modules/commons/src/jni/ \
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
				-I$(top_srcdir)/modules/jvm/includes/ \
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
libsciconsole_la_SOURCES = $(CONSOLE_C_JNI_SOURCES) $(CONSOLE_C_SOURCES) $(CONSOLE_CXX_JNI_SOURCES) $(CONSOLE_CXX_SOURCES) $(CONSOLE_GATEWAY_C_SOURCES)

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

include $(top_srcdir)/Makefile.incl.am
