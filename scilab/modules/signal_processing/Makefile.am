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

libscisignal_processing_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
pkglib_LTLIBRARIES = libscisignal_processing.la

libscisignal_processing_la_SOURCES = $(SIGNAL_PROCESSING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libscisignal_processing_la_LIBADD = \
	$(top_builddir)/modules/elementaries_functions/libscielementaries_functions.la 
# Cyclic deps
#	$(top_builddir)/modules/core/libscicore.la

libscisignal_processing_la_PKGCONFIG = signal_processing.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/signal_processing


#### signal_processing : Conf files ####
libscisignal_processing_la_rootdir = $(target)
libscisignal_processing_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### signal_processing : Scilab macros ###
libscisignal_processing_la_macrodir = $(target)/macros
libscisignal_processing_la_macro_DATA22 = macros/srkf.sci \
macros/intdec.sci \
macros/phc.sci \
macros/lindquist.sci \
macros/system.sci \
macros/detrend.sci \
macros/ifft.sci \
macros/jmat.sci \
macros/mfft.sci \
macros/group.sci \
macros/sincd.sci \
macros/czt.sci \
macros/cheb2mag.sci \
macros/casc.sci \
macros/eqiir.sci \
macros/hilb.sci \
macros/faurre.sci \
macros/lev.sci \
macros/%asn.sci \
macros/zpell.sci \
macros/trans.sci \
macros/zpbutt.sci \
macros/wiener.sci \
macros/yulewalk.sci \
macros/find_freq.sci \
macros/findm.sci \
macros/iirgroup.sci \
macros/analpf.sci \
macros/lgfft.sci \
macros/cepstrum.sci \
macros/buttmag.sci \
macros/sskf.sci \
macros/wfir.sci \
macros/cheb1mag.sci \
macros/frfit.sci \
macros/fsfirlin.sci \
macros/dft.sci \
macros/iirlp.sci \
macros/mrfit.sci \
macros/cspect.sci \
macros/lattn.sci \
macros/iirmod.sci \
macros/kalm.sci \
macros/convol.sci \
macros/srfaur.sci \
macros/ffilt.sci \
macros/eqfir.sci \
macros/window.sci \
macros/chepol.sci \
macros/levin.sci \
macros/fftshift.sci \
macros/hank.sci \
macros/ell1mag.sci \
macros/iir.sci \
macros/remezb.sci \
macros/zpch1.sci \
macros/%sn.sci \
macros/zpch2.sci \
macros/mese.sci \
macros/bilt.sci \
macros/wigner.sci \
macros/frmag.sci \
macros/pspect.sci \
macros/%k.sci \
macros/fft2.sci macros/analpf.bin \
macros/%asn.bin \
macros/bilt.bin \
macros/buttmag.bin \
macros/casc.bin \
macros/cepstrum.bin \
macros/cheb1mag.bin \
macros/cheb2mag.bin \
macros/chepol.bin \
macros/convol.bin \
macros/cspect.bin \
macros/czt.bin \
macros/detrend.bin \
macros/dft.bin \
macros/ell1mag.bin \
macros/eqfir.bin \
macros/eqiir.bin \
macros/faurre.bin \
macros/ffilt.bin \
macros/fft2.bin \
macros/fftshift.bin \
macros/find_freq.bin \
macros/findm.bin \
macros/frfit.bin \
macros/frmag.bin \
macros/fsfirlin.bin \
macros/group.bin \
macros/hank.bin \
macros/hilb.bin \
macros/ifft.bin \
macros/iirgroup.bin \
macros/iirlp.bin \
macros/iirmod.bin \
macros/iir.bin \
macros/intdec.bin \
macros/jmat.bin \
macros/kalm.bin \
macros/%k.bin \
macros/lattn.bin \
macros/levin.bin \
macros/lev.bin \
macros/lgfft.bin \
macros/lindquist.bin \
macros/mese.bin \
macros/mfft.bin \
macros/mrfit.bin \
macros/phc.bin \
macros/pspect.bin \
macros/remezb.bin \
macros/sincd.bin \
macros/%sn.bin \
macros/srfaur.bin \
macros/srkf.bin \
macros/sskf.bin \
macros/system.bin \
macros/trans.bin \
macros/wfir.bin \
macros/wiener.bin \
macros/wigner.bin \
macros/window.bin \
macros/yulewalk.bin \
macros/zpbutt.bin \
macros/zpch1.bin \
macros/zpch2.bin \
macros/zpell.bin macros/names macros/lib macros/buildmacros.sce


#### signal_processing : init scripts #### 
libscisignal_processing_la_etcdir = $(target)/etc
libscisignal_processing_la_etc_DATA = etc/signal_processing.quit etc/signal_processing.start

#### signal_processing : gateway declaration #### 
libscisignal_processing_la_sci_gatewaydir = $(target)/sci_gateway
libscisignal_processing_la_sci_gateway_DATA = sci_gateway/signal_processing



#### signal_processing : include files ####
libscisignal_processing_la_includedir=$(sciincludedir)/signal_processing
libscisignal_processing_la_include_HEADERS = includes/gw_signal.h


#### signal_processing : demos files ####
libscisignal_processing_la_demosdir=$(target)
nobase_libscisignal_processing_la_demos_DATA = demos/arma/arma1.dem \
demos/arma/arma2.dem \
demos/arma/arma3.dem \
demos/winfilt.dem \
demos/signal.dem \
demos/spect.dem \
demos/remezfilt.dem \
demos/iirfilt.dem \
demos/iir.dem \
demos/bode.dem \
demos/wiener.dem

include $(top_srcdir)/Makefile.incl.am
