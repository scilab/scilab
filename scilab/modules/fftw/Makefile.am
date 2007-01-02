
datadir=${prefix}/modules
#### Target ######
target=$(datadir)/fftw


#### fftw : Conf files ####
libfftw_la_rootdir = $(target)
libfftw_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### fftw : Scilab macros ###
libfftw_la_macrodir = $(target)/macros
libfftw_la_macro_DATA =   macros/names macros/lib macros/buildmacros.sce


#### fftw : init scripts #### 
libfftw_la_etcdir = $(target)/etc
libfftw_la_etc_DATA = etc/fftw.quit etc/fftw.start

#### fftw : gateway declaration #### 
libfftw_la_sci_gatewaydir = $(target)/sci_gateway
libfftw_la_sci_gateway_DATA = sci_gateway/fftw


#### fftw : documentation FR #### 
libfftw_la_helpfrdir = $(target)/help/fr
libfftw_la_helpfr_DATA2 = 


#### fftw : documentation EN #### 
libfftw_la_helpendir = $(target)/help/eng
libfftw_la_helpen_DATA2 = 


#### fftw : include files ####
libfftw_la_includedir=$(includedir)/fftw
libfftw_la_include_HEADERS = includes/sci_loadfftwlibrary.h \
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
