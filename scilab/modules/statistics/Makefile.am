##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

STATISTICS_C_SOURCES = src/c/CdfBase.c \
src/c/ipmpar1.c

STATISTICS_FORTRAN_SOURCES = src/dcdflib/psi.f \
src/dcdflib/alngam.f \
src/dcdflib/erfc1.f \
src/dcdflib/cdff.f \
src/dcdflib/spmpar.f \
src/dcdflib/cumnor.f \
src/dcdflib/cdfpoi.f \
src/dcdflib/bcorr.f \
src/dcdflib/cumchi.f \
src/dcdflib/apser.f \
src/dcdflib/gamln1.f \
src/dcdflib/cdft.f \
src/dcdflib/gaminv.f \
src/dcdflib/cumnbn.f \
src/dcdflib/cdffnc.f \
src/dcdflib/rlog1.f \
src/dcdflib/cdfchn.f \
src/dcdflib/dt1.f \
src/dcdflib/bgrat.f \
src/dcdflib/dinvnr.f \
src/dcdflib/rlog.f \
src/dcdflib/basym.f \
src/dcdflib/bfrac.f \
src/dcdflib/cumbin.f \
src/dcdflib/cumbet.f \
src/dcdflib/rcomp.f \
src/dcdflib/cdfgam.f \
src/dcdflib/gratio.f \
src/dcdflib/gam1.f \
src/dcdflib/exparg.f \
src/dcdflib/ipmpar.f \
src/dcdflib/cumf.f \
src/dcdflib/cumpoi.f \
src/dcdflib/gamln.f \
src/dcdflib/betaln.f \
src/dcdflib/stvaln.f \
src/dcdflib/cumt.f \
src/dcdflib/esum.f \
src/dcdflib/bpser.f \
src/dcdflib/cdfnor.f \
src/dcdflib/cumfnc.f \
src/dcdflib/erf.f \
src/dcdflib/alnrel.f \
src/dcdflib/cumchn.f \
src/dcdflib/rexp.f \
src/dcdflib/fpser.f \
src/dcdflib/bratio.f \
src/dcdflib/cdfchi.f \
src/dcdflib/gamma.f \
src/dcdflib/dinvr.f \
src/dcdflib/brcomp.f \
src/dcdflib/devlpl.f \
src/dcdflib/cdfnbn.f \
src/dcdflib/dzror.f \
src/dcdflib/gsumln.f \
src/dcdflib/algdiv.f \
src/dcdflib/cumgam.f \
src/dcdflib/grat1.f \
src/dcdflib/brcmp1.f \
src/dcdflib/cdfbin.f \
src/dcdflib/bup.f \
src/dcdflib/cdfbet.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_statistics.c \
sci_gateway/c/sci_cdfchi.c \
sci_gateway/c/sci_cdft.c \
sci_gateway/c/sci_cdfgam.c \
sci_gateway/c/sci_cdff.c \
sci_gateway/c/sci_cdffnc.c \
sci_gateway/c/sci_cdfpoi.c \
sci_gateway/c/sci_cdfbin.c \
sci_gateway/c/sci_cdfchn.c \
sci_gateway/c/sci_cdfbet.c \
sci_gateway/c/sci_cdfnbn.c \
sci_gateway/c/sci_cdfnor.c

libscistatistics_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes
#-Isrc/c/
pkglib_LTLIBRARIES = libscistatistics.la

libscistatistics_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscistatistics_la_SOURCES = $(STATISTICS_C_SOURCES) $(STATISTICS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(STATISTICS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscistatistics_la_CFLAGS)

libscistatistics_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 

#### Target ######
modulename=statistics


#### statistics : Conf files ####
libscistatistics_la_rootdir = $(mydatadir)
libscistatistics_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### statistics : init scripts ####
libscistatistics_la_etcdir = $(mydatadir)/etc
libscistatistics_la_etc_DATA = etc/statistics.quit etc/statistics.start

#### statistics : gateway declaration ####
libscistatistics_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscistatistics_la_sci_gateway_DATA = sci_gateway/statistics_gateway.xml


include $(top_srcdir)/Makefile.incl.am
