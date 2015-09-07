# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

DIFFERENTIAL_EQUATIONS_CPP_SOURCES = \
    src/cpp/scifunctions.cpp \
    src/cpp/checkodeerror.cpp \
    src/cpp/differentialequationfunctions.cpp


DIFFERENTIAL_EQUATIONS_C_SOURCES = \
    src/c/colnewmsgs.c \
    src/c/dassl.c \
    src/c/feval.c \
    src/c/arnol.c \
    src/c/rk4.c \
    src/c/Ex-odedc.c \
    src/c/Ex-ode.c \
    src/c/Ex-daskr.c \
    src/c/errmds.c

DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES = \
    src/fortran/rscar1.f \
    src/fortran/bcomp.f \
    src/fortran/lcomp.f \
    src/fortran/loren.f \
    src/fortran/prja.f \
    src/fortran/vnorm.f \
    src/fortran/lsoda.f \
    src/fortran/lsodar.f \
    src/fortran/ainvg.f \
    src/fortran/lsode.f \
    src/fortran/svcom1.f \
    src/fortran/dqk21.f \
    src/fortran/solsy.f \
    src/fortran/lsodi.f \
    src/fortran/ddassl.f \
    src/fortran/dqpsrt.f \
    src/fortran/dqelg.f \
    src/fortran/cfode.f \
    src/fortran/rscma1.f \
    src/fortran/colnew.f \
    src/fortran/dcutet.f \
    src/fortran/xsetf.f \
    src/fortran/dgbsl.f \
    src/fortran/rkf45.f \
    src/fortran/rchek.f \
    src/fortran/dqags.f \
    src/fortran/xerrwv.f \
    src/fortran/twodq.f \
    src/fortran/dqagse.f \
    src/fortran/greatr.f \
    src/fortran/hpdel.f \
    src/fortran/hpins.f \
    src/fortran/svcar1.f \
    src/fortran/rscom1.f \
    src/fortran/rksimp.f \
    src/fortran/roots.f \
    src/fortran/stoda.f \
    src/fortran/bnorm.f \
    src/fortran/rchek2.f \
    src/fortran/stode.f \
    src/fortran/vmnorm.f \
    src/fortran/prepj.f \
    src/fortran/lsdisc.f \
    src/fortran/fnorm.f \
    src/fortran/ddasrt.f \
    src/fortran/ddaskr.f \
    src/fortran/daux.f \
    src/fortran/stodi.f \
    src/fortran/xsetun.f \
    src/fortran/intdy.f \
    src/fortran/ewset.f \
    src/fortran/roots2.f \
    src/fortran/prepji.f \
    src/fortran/svcma1.f \
    src/fortran/lsrgk.f \
    src/fortran/odeint.f \
    src/fortran/rkqc.f \
    src/fortran/writbufode.f

GATEWAY_FORTRAN_SOURCES = \
    sci_gateway/fortran/Ex-impl.f \
    sci_gateway/fortran/Ex-int2d.f \
    sci_gateway/fortran/Ex-int3d.f \
    sci_gateway/fortran/Ex-intg.f \
    sci_gateway/fortran/Ex-dasrt.f \
    sci_gateway/fortran/Ex-dassl.f \
    sci_gateway/fortran/Ex-bvode.f

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/differential_equations_gw.cpp \
    sci_gateway/cpp/sci_int2d.cpp \
    sci_gateway/cpp/sci_int3d.cpp \
    sci_gateway/cpp/sci_intg.cpp \
    sci_gateway/cpp/sci_ode.cpp \
    sci_gateway/cpp/sci_odedc.cpp \
    sci_gateway/cpp/sci_feval.cpp \
    sci_gateway/cpp/sci_bvode.cpp \
    sci_gateway/cpp/sci_impl.cpp \
    sci_gateway/cpp/sci_dassl.cpp \
    sci_gateway/cpp/sci_dasrt.cpp \
    sci_gateway/cpp/sci_daskr.cpp

libscidifferential_equations_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscidifferential_equations.la
noinst_LTLIBRARIES = libscidifferential_equations-algo.la
else
noinst_LTLIBRARIES = libscidifferential_equations-algo.la libscidifferential_equations.la
endif



libscidifferential_equations_algo_la_SOURCES = $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES) $(DIFFERENTIAL_EQUATIONS_CPP_SOURCES)
libscidifferential_equations_la_SOURCES = $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_CPP_SOURCES) $(GATEWAY_C_SOURCES)
libscidifferential_equations_algo_la_CPPFLAGS = $(libscidifferential_equations_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_CPP_SOURCES)
INCLUDE_FLAGS = $(libscidifferential_equations_la_CPPFLAGS)

libscidifferential_equations_la_LIBADD = libscidifferential_equations-algo.la


#### Target ######
modulename=differential_equations


#### differential_equations : Conf files ####
libscidifferential_equations_la_rootdir = $(mydatadir)
libscidifferential_equations_la_root_DATA =  license.txt

#### differential_equations : init scripts ####
libscidifferential_equations_la_etcdir = $(mydatadir)/etc
libscidifferential_equations_la_etc_DATA = etc/differential_equations.quit etc/differential_equations.start

#### differential_equations : gateway declaration ####
libscidifferential_equations_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidifferential_equations_la_sci_gateway_DATA = sci_gateway/differential_equations_gateway.xml

include $(top_srcdir)/Makefile.incl.am
