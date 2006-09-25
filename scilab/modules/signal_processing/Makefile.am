##########
### Sylvestre Ledru <sylvestre.ledru@inria.f \r>
### INRIA - Scilab 2006 
##########

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

libsignal_processing_la_includedir=$(includedir)/signal_processing

libsignal_processing_la_include_HEADERS = includes/gw_signal.h

libsignal_processing_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libsignal_processing.la

libsignal_processing_la_SOURCES = $(SIGNAL_PROCESSING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsignal_processing_la_DEPENDENCIES = \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la \
	$(top_builddir)/modules/core/libcore.la

libsignal_processing_la_LIBADD = $(libsignal_processing_la_DEPENDENCIES)

libsignal_processing_la_PKGCONFIG = signal_processing.pc
