# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

SPARSE_C_SOURCES = \
src/c/spUtils.c \
src/c/mspelm.c \
src/c/lu.c \
src/c/spFactor.c \
src/c/spBuild.c \
src/c/spOutput.c \
src/c/spAllocate.c \
src/c/spSolve.c
# src/c/spFortran.c

SPARSE_FORTRAN_SOURCES = src/fortran/spcompack.f \
src/fortran/wsposp.f \
src/fortran/findl.f \
src/fortran/spcho1.f \
src/fortran/dspful.f \
src/fortran/blkslv.f \
src/fortran/wspcle.f \
src/fortran/wspt.f \
src/fortran/wspssp.f \
src/fortran/lspmat.f \
src/fortran/lsosp.f \
src/fortran/blkfc1.f \
src/fortran/wspxsp.f \
src/fortran/lspful.f \
src/fortran/blkfct.f \
src/fortran/symfct.f \
src/fortran/dspt.f \
src/fortran/wspos.f \
src/fortran/dful2sp.f \
src/fortran/sputil.f \
src/fortran/lspt.f \
src/fortran/dspxs.f \
src/fortran/sp2col.f \
src/fortran/dcompa.f \
src/fortran/sz2ptr.f \
src/fortran/spt.f \
src/fortran/lcompa.f \
src/fortran/wspmat.f \
src/fortran/iperm.f \
src/fortran/wspful.f \
src/fortran/spif1b.f \
src/fortran/spextr.f \
src/fortran/lful2sp.f \
src/fortran/dspos.f \
src/fortran/dspasp.f \
src/fortran/inpnv.f \
src/fortran/spreshape.f \
src/fortran/spsort.f \
src/fortran/wperm.f \
src/fortran/dij2sp.f \
src/fortran/dspcsp.f \
src/fortran/spcho2.f \
src/fortran/lspos.f \
src/fortran/wspe2.f \
src/fortran/spif.f \
src/fortran/lspasp.f \
src/fortran/dspisp.f \
src/fortran/wful2sp.f \
src/fortran/lij2sp.f \
src/fortran/lspcsp.f \
src/fortran/wspis.f \
src/fortran/dspmsp.f \
src/fortran/dsposp.f \
src/fortran/wsmsp.f \
src/fortran/wspms.f \
src/fortran/lspisp.f \
src/fortran/dspcle.f \
src/fortran/wsosp.f \
src/fortran/dspssp.f \
src/fortran/lsposp.f \
src/fortran/dspxsp.f \
src/fortran/ordmmd.f \
src/fortran/spextr1.f \
src/fortran/wcompa.f \
src/fortran/lspxsp.f \
src/fortran/spisp.f \
src/fortran/dperm.f \
src/fortran/spind.f \
src/fortran/dspe2.f \
src/fortran/wspasp.f \
src/fortran/wij2sp.f \
src/fortran/wspcsp.f \
src/fortran/lspops.f \
src/fortran/spifp.f \
src/fortran/isort1.f \
src/fortran/dspis.f \
src/fortran/wspxs.f \
src/fortran/lspe2.f \
src/fortran/wspisp.f \
src/fortran/dsmsp.f \
src/fortran/dspms.f \
src/fortran/spord.f \
src/fortran/dsosp.f \
src/fortran/dspmat.f \
src/fortran/wspmsp.f \
src/fortran/lspis.f \
src/fortran/writebuf.f \
src/fortran/ta2lpd.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_spcompa.c \
sci_gateway/c/sci_full.c \
sci_gateway/c/sci_blkslvi.c \
sci_gateway/c/sci_inpnvi.c \
sci_gateway/c/sci_nnz.c \
sci_gateway/c/sci_ludel.c \
sci_gateway/c/sci_spmatrix.c \
sci_gateway/c/sci_msparse.c \
sci_gateway/c/sci_blkfc1i.c \
sci_gateway/c/sci_bfinit.c \
sci_gateway/c/sci_lufact.c \
sci_gateway/c/sci_symfcti.c \
sci_gateway/c/sci_mfull.c \
sci_gateway/c/sci_luget.c \
sci_gateway/c/sci_ordmmd.c \
sci_gateway/c/sci_spget.c \
sci_gateway/c/sci_spclean.c \
sci_gateway/c/sci_spchol.c \
sci_gateway/c/sci_lusolve.c \
sci_gateway/c/sci_fadj2sp.c \
sci_gateway/c/gw_sparse.c \
sci_gateway/c/sci_sfinit.c \
sci_gateway/c/sci_sparse.c \
sci_gateway/c/sci_mspget.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_spcompa.f \
sci_gateway/fortran/sci_f_sparse.f \
sci_gateway/fortran/sci_f_full.f \
sci_gateway/fortran/sci_f_blkslvi.f \
sci_gateway/fortran/sci_f_inpnvi.f \
sci_gateway/fortran/sci_f_nnz.f \
sci_gateway/fortran/sci_f_ludel.f \
sci_gateway/fortran/sci_f_bfinit.f \
sci_gateway/fortran/spops.f \
sci_gateway/fortran/sci_f_spmatrix.f \
sci_gateway/fortran/sci_f_blkfc1i.f \
sci_gateway/fortran/sci_f_lufact.f \
sci_gateway/fortran/sci_f_symfcti.f \
sci_gateway/fortran/sci_f_luget.f \
sci_gateway/fortran/sci_f_ordmmd.f \
sci_gateway/fortran/sci_f_spget.f \
sci_gateway/fortran/sci_f_spclean.f \
sci_gateway/fortran/sci_f_spchol.f \
sci_gateway/fortran/sci_f_lusolve.f \
sci_gateway/fortran/sci_f_fadj2sp.f \
sci_gateway/fortran/sci_f_sfinit.f \
sci_gateway/fortran/sci_f_ta2lpd.f

libscisparse_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/elementary_functions/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				$(AM_CPPFLAGS)

#-Isrc/c/
if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscisparse.la
noinst_LTLIBRARIES = libscisparse-algo.la
else
noinst_LTLIBRARIES = libscisparse-algo.la libscisparse.la
endif



libscisparse_algo_la_SOURCES = $(SPARSE_C_SOURCES) $(SPARSE_FORTRAN_SOURCES)
libscisparse_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscisparse_algo_la_CPPFLAGS = $(libscisparse_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC = $(SPARSE_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscisparse_la_CPPFLAGS)

libscisparse_la_LIBADD = libscisparse-algo.la

#### Target ######
modulename=sparse


#### sparse : Conf files ####
libscisparse_la_rootdir = $(mydatadir)
libscisparse_la_root_DATA =  license.txt


#### sparse : init scripts ####
libscisparse_la_etcdir = $(mydatadir)/etc
libscisparse_la_etc_DATA = etc/sparse.quit etc/sparse.start

#### sparse : gateway declaration ####
libscisparse_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisparse_la_sci_gateway_DATA = sci_gateway/sparse_gateway.xml

include $(top_srcdir)/Makefile.incl.am
