# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

INTEGER_C_SOURCES = \
src/c/genconcatcol.c \
src/c/genprod.c \
src/c/genrdiv.c \
src/c/i_nextj.c \
src/c/genextrac1.c \
src/c/genextrac2.c \
src/c/gencopy.c \
src/c/gentril.c \
src/c/gencheck.c \
src/c/tpconv.c \
src/c/genscal.c \
src/c/gentriu.c \
src/c/genvmul.c \
src/c/genconcatrow.c \
src/c/geninsert1.c \
src/c/geninsert2.c \
src/c/gendot.c \
src/c/db2int.c \
src/c/genadd.c \
src/c/genset.c \
src/c/gencusum.c \
src/c/gensubst.c \
src/c/genmdsp.c \
src/c/gensum.c \
src/c/genmmul.c \
src/c/genmsum.c \
src/c/genmprod.c \
src/c/genmtran.c \
src/c/genisany.c \
src/c/genmcopy.c \
src/c/gencuprod.c \
src/c/genbitops.c \
src/c/genimpl.c

INTEGER_FORTRAN_SOURCES = \
src/fortran/unarybit.f \
src/fortran/i_mput.f \
src/fortran/i_logic.f \
src/fortran/i_a_i.f \
src/fortran/i_b_i.f \
src/fortran/i_c_i.f \
src/fortran/i_d_i.f \
src/fortran/intinttype.f \
src/fortran/i_f_i.f \
src/fortran/i_i_i.f \
src/fortran/i_l_i.f \
src/fortran/i_m_i.f \
src/fortran/i_prod.f \
src/fortran/i_q_i.f \
src/fortran/i_r_i.f \
src/fortran/i_tril.f \
src/fortran/i_s_i.f \
src/fortran/i_x_i.f \
src/fortran/uppertype.f \
src/fortran/i_triu.f \
src/fortran/i_e.f \
src/fortran/memused.f \
src/fortran/bitops.f \
src/fortran/i_mget.f \
src/fortran/i_p.f \
src/fortran/i_t.f \
src/fortran/i_sum.f \
src/fortran/stack_convert.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_integer.c

GATEWAY_CPP_SOURCES = \
				sci_gateway/cpp/integer_gw.cpp \
				sci_gateway/cpp/sci_inttype.cpp \
				sci_gateway/cpp/sci_double.cpp \
				sci_gateway/cpp/sci_int.cpp

GATEWAY_FORTRAN_SOURCES = \
sci_gateway/fortran/sci_i_mput.f \
sci_gateway/fortran/sci_int32.f \
sci_gateway/fortran/sci_int16.f \
sci_gateway/fortran/sci_mgeti.f \
sci_gateway/fortran/sci_i_p.f \
sci_gateway/fortran/sci_uint8.f \
sci_gateway/fortran/sci_i_sum.f \
sci_gateway/fortran/sci_i_prod.f \
sci_gateway/fortran/sci_i_tril.f \
sci_gateway/fortran/sci_int8.f \
sci_gateway/fortran/sci_uint32.f \
sci_gateway/fortran/sci_uint16.f \
sci_gateway/fortran/sci_i_triu.f \
sci_gateway/fortran/sci_inttype.f

libsciinteger_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/libs/dynamiclibrary/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/operations/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/types/includes/ \
				-I$(top_srcdir)/modules/ast/includes/ \
				-I$(top_srcdir)/modules/symbol/includes/ \
				$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciinteger.la
noinst_LTLIBRARIES = libsciinteger-algo.la
else
noinst_LTLIBRARIES = libsciinteger-algo.la libsciinteger.la
endif


libsciinteger_algo_la_SOURCES = $(INTEGER_C_SOURCES) $(INTEGER_FORTRAN_SOURCES)
libsciinteger_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_CPP_SOURCES)
libsciinteger_algo_la_CPPFLAGS = $(libsciinteger_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC=  $(INTEGER_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciinteger_la_CPPFLAGS)

# cyclic $(top_builddir)/modules/polynomials/libscipolynomials.la
libsciinteger_la_LIBADD = libsciinteger-algo.la



#### Target ######
modulename=integer


#### integer : Conf files ####
libsciinteger_la_rootdir = $(mydatadir)
libsciinteger_la_root_DATA =  license.txt


#### integer : init scripts ####
libsciinteger_la_etcdir = $(mydatadir)/etc
libsciinteger_la_etc_DATA = etc/integer.quit etc/integer.start

#### integer : gateway declaration ####
libsciinteger_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciinteger_la_sci_gateway_DATA = sci_gateway/integer_gateway.xml

include $(top_srcdir)/Makefile.incl.am
