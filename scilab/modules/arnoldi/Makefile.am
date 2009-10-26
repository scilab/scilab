##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########


ARNOLDI_FORTRAN_SOURCES = src/arpack/dsconv.f \
src/arpack/dsortc.f \
src/arpack/dsaup2.f \
src/arpack/dneigh.f \
src/arpack/znaupd.f \
src/arpack/dsesrt.f \
src/arpack/zneupd.f \
src/arpack/smout.f \
src/arpack/znaitr.f \
src/arpack/zngets.f \
src/arpack/second.f \
src/arpack/znapps.f \
src/arpack/zlarnv.f \
src/arpack/dsortr.f \
src/arpack/zmout.f \
src/arpack/dseigt.f \
src/arpack/zgetv0.f \
src/arpack/znaup2.f \
src/arpack/icnteq.f \
src/arpack/icopy.f \
src/arpack/zneigh.f \
src/arpack/zsortc.f \
src/arpack/dstatn.f \
src/arpack/dlaruv.f \
src/arpack/dstqrb.f \
src/arpack/cvout.f \
src/arpack/dvout.f \
src/arpack/dstats.f \
src/arpack/dlaqrb.f \
src/arpack/ivout.f \
src/arpack/dnaupd.f \
src/arpack/dneupd.f \
src/arpack/dsaupd.f \
src/arpack/svout.f \
src/arpack/dnaitr.f \
src/arpack/iswap.f \
src/arpack/zstatn.f \
src/arpack/dseupd.f \
src/arpack/dngets.f \
src/arpack/dnapps.f \
src/arpack/dsaitr.f \
src/arpack/dlarnv.f \
src/arpack/cmout.f \
src/arpack/dnconv.f \
src/arpack/zvout.f \
src/arpack/dmout.f \
src/arpack/dsgets.f \
src/arpack/dgetv0.f \
src/arpack/dnaup2.f \
src/arpack/dsapps.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_zneupd \
sci_gateway/c/sci_dseupd.c \
sci_gateway/c/sci_zneupd.c \
sci_gateway/c/gw_arnoldi.c \
sci_gateway/c/sci_dnaupd.c \
sci_gateway/c/sci_dneupd.c \
sci_gateway/c/sci_dsaupd.c \
sci_gateway/c/sci_znaupd.c

libsciarnoldi_la_CFLAGS= 	-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes/

libsciarnoldi_la_FFLAGS= -I$(srcdir)/src/arpack/

pkglib_LTLIBRARIES = libsciarnoldi.la

libsciarnoldi_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciarnoldi_la_SOURCES =  $(ARNOLDI_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciarnoldi_la_CFLAGS)

libsciarnoldi_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la

#### Target ######
modulename=arnoldi


#### arnoldi : Conf files ####
libsciarnoldi_la_rootdir = $(mydatadir)
libsciarnoldi_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### arnoldi : init scripts ####
libsciarnoldi_la_etcdir = $(mydatadir)/etc
libsciarnoldi_la_etc_DATA = etc/arnoldi.quit etc/arnoldi.start

#### arnoldi : gateway declaration ####
libsciarnoldi_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciarnoldi_la_sci_gateway_DATA = sci_gateway/arnoldi_gateway.xml

include $(top_srcdir)/Makefile.incl.am
