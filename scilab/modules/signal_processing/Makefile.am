##########
### Sylvestre Ledru <sylvestre.ledru@inria.f \r>
### INRIA - Scilab 2006
##########

SIGNAL_PROCESSING_C_SOURCES = src/c/corrtable.c

SIGNAL_PROCESSING_FORTRAN_SOURCES = src/fortran/sci_ffir.f \
src/fortran/degree.f \
src/fortran/parcha.f \
src/fortran/ouch.f \
src/fortran/dfftbi.f \
src/fortran/dsqrtc.f \
src/fortran/tranze.f \
src/fortran/chebc.f \
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
src/fortran/kaiser.f \
src/fortran/deli11.f \
src/fortran/compel.f \
src/fortran/filbut.f \
src/fortran/desi01.f \
src/fortran/desi22.f \
src/fortran/amell.f \
src/fortran/desi24.f \
src/fortran/dellk.f \
src/fortran/syredi.f \
src/fortran/dpspln.f \
src/fortran/coeft.f \
src/fortran/tranpo.f \
src/fortran/blnumz.f \
src/fortran/sn.f \
src/fortran/bldenz.f \
src/fortran/dspln.f \
src/fortran/belan.f \
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
src/fortran/dfftmx.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_remez.c \
sci_gateway/c/sci_rpem.c \
sci_gateway/c/sci_corr.c \
sci_gateway/c/sci_ffir.c \
sci_gateway/c/sci_fiir.c \
sci_gateway/c/sci_amell.c \
sci_gateway/c/gw_signal.c \
sci_gateway/c/sci_delip.c \
sci_gateway/c/sci_fft.c \
sci_gateway/c/sci_syredi.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_delip.f \
sci_gateway/fortran/sci_fft.f \
sci_gateway/fortran/sci_syredi.f \
sci_gateway/fortran/sci_remez.f \
sci_gateway/fortran/sci_rpem.f \
sci_gateway/fortran/sci_corr.f \
sci_gateway/fortran/sci_fiir.f \
sci_gateway/fortran/sci_amell.f \
sci_gateway/fortran/bgetx.f \
sci_gateway/fortran/bgety.f \
sci_gateway/fortran/Ex-corr.f

libscisignal_processing_la_CFLAGS=  	-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/dynamic_link/includes

pkglib_LTLIBRARIES = libscisignal_processing.la

libscisignal_processing_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscisignal_processing_la_SOURCES = $(SIGNAL_PROCESSING_FORTRAN_SOURCES) $(SIGNAL_PROCESSING_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(SIGNAL_PROCESSING_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscisignal_processing_la_CFLAGS)

libscisignal_processing_la_LIBADD =  $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/dynamic_link/libscidynamic_link.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 

#### Target ######
modulename=signal_processing


#### signal_processing : Conf files ####
libscisignal_processing_la_rootdir = $(mydatadir)
libscisignal_processing_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�signal_processing : init scripts ####
libscisignal_processing_la_etcdir = $(mydatadir)/etc
libscisignal_processing_la_etc_DATA = etc/signal_processing.quit etc/signal_processing.start

####�signal_processing : gateway declaration ####
libscisignal_processing_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisignal_processing_la_sci_gateway_DATA = sci_gateway/signal_processing_gateway.xml

include $(top_srcdir)/Makefile.incl.am
