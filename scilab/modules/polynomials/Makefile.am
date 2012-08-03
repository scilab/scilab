# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

POLYNOMIALS_FORTRAN_SOURCES = src/fortran/residu.f \
src/fortran/imptra.f \
src/fortran/dmpadj.f \
src/fortran/impins.f \
src/fortran/dpmul1.f \
src/fortran/wprxc.f \
src/fortran/dmpcnc.f \
src/fortran/wmpmu.f \
src/fortran/mpinsp.f \
src/fortran/impext.f \
src/fortran/recbez.f \
src/fortran/sfact1.f \
src/fortran/dpodiv.f \
src/fortran/dmptra.f \
src/fortran/dmpins.f \
src/fortran/dpsimp.f \
src/fortran/dmp2pm.f \
src/fortran/dmpext.f \
src/fortran/wpmul.f \
src/fortran/bezstp.f \
src/fortran/wmpad.f \
src/fortran/dprxc.f \
src/fortran/quadsd.f \
src/fortran/wmpadj.f \
src/fortran/dmpmu.f \
src/fortran/wpmul1.f \
src/fortran/wmpcnc.f \
src/fortran/mptri.f \
src/fortran/horner.f \
src/fortran/idegre.f \
src/fortran/fxshfr.f \
src/fortran/mpdiag.f \
src/fortran/dmpcle.f \
src/fortran/rpoly.f \
src/fortran/wpodiv.f \
src/fortran/wdmpmu.f \
src/fortran/wmptra.f \
src/fortran/wmpins.f \
src/fortran/mpdegr.f \
src/fortran/ddmpev.f \
src/fortran/realit.f \
src/fortran/dpmul.f \
src/fortran/sfact2.f \
src/fortran/dmpad.f \
src/fortran/nextk.f \
src/fortran/blktit.f \
src/fortran/dimin.f \
src/fortran/newest.f \
src/fortran/dwmpmu.f \
src/fortran/impcnc.f \
src/fortran/wdmpad.f \
src/fortran/wmpcle.f \
src/fortran/quadit.f \
src/fortran/quad.f \
src/fortran/calcsc.f \
src/fortran/writebufsfact.f \
src/fortran/chkvar.f


GATEWAY_C_SOURCES = sci_gateway/c/gw_polynomials.c \
sci_gateway/c/sci_sfact.c \
sci_gateway/c/sci_cleanp.c \
sci_gateway/c/sci_sum.c \
sci_gateway/c/sci_prod.c \
sci_gateway/c/sci_diag.c \
sci_gateway/c/sci_simp_mode.c \
sci_gateway/c/sci_roots.c \
sci_gateway/c/sci_varn.c \
sci_gateway/c/sci_simp.c \
sci_gateway/c/sci_bezout.c \
sci_gateway/c/sci_tril.c \
sci_gateway/c/sci_div.c \
sci_gateway/c/sci_coeff.c \
sci_gateway/c/sci_poly.c \
sci_gateway/c/sci_triu.c \
sci_gateway/c/sci_degree.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_simp.f \
sci_gateway/fortran/sci_f_poly.f \
sci_gateway/fortran/sci_f_varn.f \
sci_gateway/fortran/sci_f_sum.f \
sci_gateway/fortran/sci_f_div.f \
sci_gateway/fortran/sci_f_diag.f \
sci_gateway/fortran/sci_f_prod.f \
sci_gateway/fortran/polops.f \
sci_gateway/fortran/sci_f_bezout.f \
sci_gateway/fortran/sci_f_triu_tril.f \
sci_gateway/fortran/sci_f_roots.f \
sci_gateway/fortran/sci_f_psimp.f \
sci_gateway/fortran/sci_f_rsimp.f \
sci_gateway/fortran/sci_f_coeff.f \
sci_gateway/fortran/sci_f_degree.f \
sci_gateway/fortran/sci_f_pclean.f \
sci_gateway/fortran/sci_f_sfact.f \
sci_gateway/fortran/sci_f_simpmd.f


libscipolynomials_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscipolynomials.la
noinst_LTLIBRARIES = libscipolynomials-algo.la
else
noinst_LTLIBRARIES = libscipolynomials-algo.la libscipolynomials.la
endif



libscipolynomials_algo_la_SOURCES = $(POLYNOMIALS_FORTRAN_SOURCES)
libscipolynomials_la_SOURCES = $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)
libscipolynomials_algo_la_CPPFLAGS = $(libscipolynomials_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= ) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscipolynomials_la_CPPFLAGS)

libscipolynomials_la_LIBADD = libscipolynomials-algo.la

#### Target ######
modulename=polynomials


#### polynomials : Conf files ####
libscipolynomials_la_rootdir = $(mydatadir)
libscipolynomials_la_root_DATA =  license.txt 


#### polynomials : init scripts ####
libscipolynomials_la_etcdir = $(mydatadir)/etc
libscipolynomials_la_etc_DATA = etc/polynomials.quit etc/polynomials.start


#### polynomials : gateway declaration ####
libscipolynomials_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscipolynomials_la_sci_gateway_DATA = sci_gateway/polynomials_gateway.xml


include $(top_srcdir)/Makefile.incl.am
