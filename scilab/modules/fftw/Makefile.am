
datadir=${prefix}/modules
#### Target ######
target=$(datadir)/fftw


#### fftw : Conf files ####
libscifftw_la_rootdir = $(target)
libscifftw_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### fftw : Scilab macros ###
libscifftw_la_macrodir = $(target)/macros
libscifftw_la_macro_DATA22 =   macros/names macros/lib macros/buildmacros.sce


#### fftw : init scripts #### 
libscifftw_la_etcdir = $(target)/etc
libscifftw_la_etc_DATA = etc/fftw.quit etc/fftw.start

#### fftw : gateway declaration #### 
libscifftw_la_sci_gatewaydir = $(target)/sci_gateway
libscifftw_la_sci_gateway_DATA = sci_gateway/fftw

#### fftw : include files ####
libscifftw_la_includedir=$(includedir)/fftw
libscifftw_la_include_HEADERS = includes/sci_loadfftwlibrary.h \
includes/sci_fftwlibraryisloaded.h \
includes/sci_pushfftw_plan.h \
includes/sci_fftw_inplace.h \
includes/sci_fftw_norm.h \
includes/sci_disposefftwlibrary.h \
includes/sci_fftw_flags.h \
includes/sci_freefftw_plan.h \
includes/gw_fftw.h \
includes/sci_fftw.h

include $(top_srcdir)/Makefile.incl.am
