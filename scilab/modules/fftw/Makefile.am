##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Allan Cornet <allan.cornet@inria.fr>
### INRIA - Scilab 2007
##########

if FFTW

#### Target ######
modulename=fftw

pkglib_LTLIBRARIES = libscifftw.la

FFTW_C_SOURCES = src/c/callfftw_Linux.c \
src/c/fftwlibname.c \
src/c/fftw_utilities.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_fftw.c \
	sci_gateway/c/sci_fftwlibraryisloaded.c \
	sci_gateway/c/sci_disposefftwlibrary.c \
	sci_gateway/c/sci_fftw.c \
	sci_gateway/c/sci_fftw_flags.c \
	sci_gateway/c/sci_loadfftwlibrary.c \
	sci_gateway/c/sci_get_fftw_wisdom.c \
	sci_gateway/c/sci_set_fftw_wisdom.c \
	sci_gateway/c/sci_fftw_forget_wisdom.c


#### fftw : Conf files ####
libscifftw_la_rootdir = $(mydatadir)
libscifftw_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### fftw : init scripts #### 
libscifftw_la_etcdir = $(mydatadir)/etc
libscifftw_la_etc_DATA = etc/fftw.quit etc/fftw.start

#### fftw : gateway declaration #### 
libscifftw_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifftw_la_sci_gateway_DATA = sci_gateway/fftw_gateway.xml

libscifftw_la_SOURCES = $(FFTW_C_SOURCES) $(GATEWAY_C_SOURCES)

libscifftw_la_CFLAGS=  -Iincludes/  -Isrc/c/

libscifftw_la_LDFLAGS = $(FFTW3_LIB)

#### fftw : include files ####
libscifftw_la_includedir=$(pkgincludedir)/fftw
libscifftw_la_include_HEADERS = includes/sci_loadfftwlibrary.h \
includes/sci_fftwlibraryisloaded.h \
includes/sci_disposefftwlibrary.h \
includes/sci_fftw_flags.h \
includes/gw_fftw.h \
includes/sci_fftw.h \
includes/sci_get_fftw_wisdom.h \
includes/sci_set_fftw_wisdom.h \
includes/sci_fftw_forget_wisdom.h

include $(top_srcdir)/Makefile.incl.am

else
		echo "FFTW not activated. Skip this module."
endif
