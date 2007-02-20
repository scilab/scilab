##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Allan Cornet <allan.cornet@inria.fr>
### INRIA - Scilab 2007
##########

if FFTW

#### Target ######
target=$(datadir)/fftw

pkglib_LTLIBRARIES = libscifftw.la

FFTW_C_SOURCES = src/callfftw_Linux.c \
src/fftwlibname.c \
src/fftw_utilities.c

GATEWAY_C_SOURCES = sci_gateway/gw_fftw.c \
	sci_gateway/sci_fftwlibraryisloaded.c \
	sci_gateway/sci_disposefftwlibrary.c \
	sci_gateway/sci_fftw_norm.c \
	sci_gateway/sci_fftw.c \
	sci_gateway/sci_freefftw_plan.c \
	sci_gateway/sci_fftw_flags.c \
	sci_gateway/sci_loadfftwlibrary.c \
	sci_gateway/sci_fftw_inplace.c \
	sci_gateway/sci_pushfftw_plan.c


#### fftw : Conf files ####
libscifftw_la_rootdir = $(target)
libscifftw_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### fftw : init scripts #### 
libscifftw_la_etcdir = $(target)/etc
libscifftw_la_etc_DATA = etc/fftw.quit etc/fftw.start

#### fftw : gateway declaration #### 
libscifftw_la_sci_gatewaydir = $(target)/sci_gateway
libscifftw_la_sci_gateway_DATA = sci_gateway/fftw

libscifftw_la_SOURCES = $(FFTW_C_SOURCES) $(GATEWAY_C_SOURCES)

libscifftw_la_CFLAGS=  -Iincludes/  -Isrc/

libscifftw_la_LDFLAGS = $(FFTW3_LIB)

#### fftw : include files ####
libscifftw_la_includedir=$(sciincludedir)/fftw
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

else
		echo "FFTW not activated. Skip this module."
endif
