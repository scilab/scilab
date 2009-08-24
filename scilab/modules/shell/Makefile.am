# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Allan CORNET
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU
# Copyright (C) 2008 - INRIA - Bruno JOFRET
#
# This file is distributed under the same license as the Scilab package.

# List of the c files
SHELL_C_SOURCES = src/c/clc.c \
src/c/tohome.c \
src/c/more.c \
src/c/scilines.c \
src/c/GetCommandLine.c \
src/c/others/zzledt.c \
src/c/others/gotoxy_nw.c \
src/c/others/clrscr_nw.c \
src/c/InitializeShell.c \
src/c/TerminateShell.c \
src/c/prompt.c \
src/c/promptecho.c


if IS_MACOSX
SHELL_C_SOURCES += src/c/others/initMacOSXEnv.c
endif

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_shell.c \
sci_gateway/c/sci_clc.c \
sci_gateway/c/sci_tohome.c \
sci_gateway/c/sci_lines.c \
sci_gateway/c/sci_prompt.c \
sci_gateway/c/sci_iswaitingforinput.c

# Includes need for the compilation
libscishell_la_CPPFLAGS =  $(JAVA_JNI_INCLUDE) -I$(srcdir)/includes/ -I$(srcdir)/src/cpp/ -I$(srcdir)/src/c/ \
-I$(srcdir)/src/c/others/ -I$(top_srcdir)/libs/MALLOC/includes/

# Includes need for the compilation
libscishell_la_CFLAGS= 	$(JAVA_JNI_INCLUDE) \
			-Iincludes/ \
			-Isrc/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/console/includes/ \
			-I$(top_srcdir)/modules/history_manager/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/core/includes \
			-I$(top_srcdir)/modules/completion/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/action_binding/includes/ \
			-I$(top_srcdir)/modules/fileio/src/c/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/operations/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscishell.la

libscishell_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(TERMCAP_LIB)

# All the sources needed by libscishell.la
libscishell_la_SOURCES = $(SHELL_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscishell_la_SOURCES)
INCLUDE_FLAGS = $(libscishell_la_CFLAGS)

## Libraries necessary
# cyclic dep $(top_builddir)/modules/console/libsciconsole.la
libscishell_la_LIBADD =   $(top_builddir)/modules/action_binding/libsciaction_binding.la $(top_builddir)/modules/history_manager/libscihistory_manager.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
# $(top_builddir)/modules/completion/libscicompletion.la
modulename=shell

#### shell : Conf files ####
libscishell_la_rootdir = $(mydatadir)
libscishell_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### shell : init scripts ####
libscishell_la_etcdir = $(mydatadir)/etc
libscishell_la_etc_DATA = etc/shell.quit etc/shell.start

#### shell : gateway declaration ####
libscishell_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscishell_la_sci_gateway_DATA = sci_gateway/shell_gateway.xml

#### shell : include files ####
libscishell_la_includedir=$(pkgincludedir)/shell
libscishell_la_include_HEADERS = includes/gw_shell.h \
includes/scilines.h \
includes/InitializeShell.h \
includes/TerminateShell.h \
includes/more.h \
includes/PutChar.h \
includes/tohome.h \
includes/zzledt.h \
includes/prompt.h \
includes/promptecho.h


if GUI

USEANT=1

endif

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am

