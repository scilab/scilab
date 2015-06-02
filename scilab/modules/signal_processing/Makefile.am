# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

SIGNAL_PROCESSING_CPP_SOURCES = \
    src/cpp/signalprocessingfunctions.cpp

SIGNAL_PROCESSING_C_SOURCES = \
    src/c/corrtable.c \
    src/c/conv2.c

SIGNAL_PROCESSING_FORTRAN_SOURCES = \
    src/fortran/degree.f \
    src/fortran/parcha.f \
    src/fortran/ouch.f \
    src/fortran/dfftbi.f \
    src/fortran/dsqrtc.f \
    src/fortran/tranze.f \
    src/fortran/tscccf.f \
    src/fortran/dsn2.f \
    src/fortran/hammin.f \
    src/fortran/bounn.f \
    src/fortran/nstabl.f \
    src/fortran/ino.f \
    src/fortran/desia.f \
    src/fortran/desi11.f \
    src/fortran/cmpse3.f \
    src/fortran/cheby.f \
    src/fortran/trbize.f \
    src/fortran/r2tx.f \
    src/fortran/deli2.f \
    src/fortran/gee.f \
    src/fortran/r4tx.f \
    src/fortran/transn.f \
    src/fortran/r8tx.f \
    src/fortran/deli11.f \
    src/fortran/compel.f \
    src/fortran/filbut.f \
    src/fortran/desi01.f \
    src/fortran/desi22.f \
    src/fortran/amell.f \
    src/fortran/desi24.f \
    src/fortran/dellk.f \
    src/fortran/syredi.f \
    src/fortran/coeft.f \
    src/fortran/tranpo.f \
    src/fortran/blnumz.f \
    src/fortran/sn.f \
    src/fortran/bldenz.f \
    src/fortran/desib.f \
    src/fortran/freque.f \
    src/fortran/desi12.f \
    src/fortran/cmpse2.f \
    src/fortran/desi14.f \
    src/fortran/delip.f \
    src/fortran/deli1.f \
    src/fortran/trbipo.f \
    src/fortran/rpem.f \
    src/fortran/dfft2.f \
    src/fortran/dgee01.f \
    src/fortran/romeg.f \
    src/fortran/snell.f \
    src/fortran/fft842.f \
    src/fortran/tg02ad.f \
    src/fortran/remez.f \
    src/fortran/poles.f \
    src/fortran/desi00.f \
    src/fortran/desi21.f \
    src/fortran/dfftmx.f\
    src/fortran/Ex-corr.f

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_rpem.cpp \
    sci_gateway/cpp/sci_corr.cpp \
    sci_gateway/cpp/sci_delip.cpp \
    sci_gateway/cpp/sci_fft.cpp \
    sci_gateway/cpp/sci_syredi.cpp

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_remez.c \
    sci_gateway/c/sci_amell.c \
    sci_gateway/c/sci_conv2.c


libscisignal_processing_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
	$(AM_CPPFLAGS)


pkglib_LTLIBRARIES = libscisignal_processing.la
noinst_LTLIBRARIES = libscisignal_processing-algo.la


libscisignal_processing_algo_la_SOURCES = $(SIGNAL_PROCESSING_FORTRAN_SOURCES) $(SIGNAL_PROCESSING_C_SOURCES) $(SIGNAL_PROCESSING_CPP_SOURCES)
libscisignal_processing_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscisignal_processing_algo_la_CPPFLAGS = $(libscisignal_processing_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(SIGNAL_PROCESSING_C_SOURCES) $(GATEWAY_C_SOURCES) $(SIGNAL_PROCESSING_CPP_SOURCES)
INCLUDE_FLAGS = $(libscisignal_processing_la_CPPFLAGS)

libscisignal_processing_la_LIBADD = libscisignal_processing-algo.la

#### Target ######
modulename=signal_processing


#### signal_processing : Conf files ####
libscisignal_processing_la_rootdir = $(mydatadir)
libscisignal_processing_la_root_DATA =  license.txt


####�signal_processing : init scripts ####
libscisignal_processing_la_etcdir = $(mydatadir)/etc
libscisignal_processing_la_etc_DATA = \
    etc/signal_processing.quit \
    etc/signal_processing.start

####�signal_processing : gateway declaration ####
libscisignal_processing_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisignal_processing_la_sci_gateway_DATA = sci_gateway/signal_processing_gateway.xml

include $(top_srcdir)/Makefile.incl.am
