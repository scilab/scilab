##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

STRING_C_SOURCES = src/c/strcat-c.c \
src/c/men_Sutils.c

STRING_FORTRAN_SOURCES =src/fortran/strord.f src/fortran/strdsp.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_strindex.c \
sci_gateway/c/sci_strcat.c \
sci_gateway/c/sci_tokens.c \
sci_gateway/c/gw_string.c \
sci_gateway/c/sci_string.c \
sci_gateway/c/sci_part.c \
sci_gateway/c/sci_ssort.c \
sci_gateway/c/sci_length.c \
sci_gateway/c/sci_strsplit.c \
sci_gateway/c/sci_str2code.c \
sci_gateway/c/sci_code2str.c \
sci_gateway/c/sci_emptystr.c \
sci_gateway/c/sci_ascii.c \
sci_gateway/c/sci_strsubst.c \
sci_gateway/c/sci_grep.c \
sci_gateway/c/sci_convstr.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_convstr.f \
sci_gateway/fortran/sci_grep.f \
sci_gateway/fortran/sci_strindex.f \
sci_gateway/fortran/sci_strcat.f \
sci_gateway/fortran/sci_tokens.f \
sci_gateway/fortran/sci_string.f \
sci_gateway/fortran/sci_part.f \
sci_gateway/fortran/sci_length.f \
sci_gateway/fortran/sci_strsplit.f \
sci_gateway/fortran/sci_str2code.f \
sci_gateway/fortran/sci_code2str.f \
sci_gateway/fortran/sci_sort.f \
sci_gateway/fortran/sci_emptystr.f \
sci_gateway/fortran/sci_ascii.f \
sci_gateway/fortran/sci_strsubst.f \
sci_gateway/fortran/strops.f

libscistring_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

pkglib_LTLIBRARIES = libscistring.la

libscistring_la_SOURCES = $(STRING_C_SOURCES) $(STRING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libscistring_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la
# Cyclic deps
#	$(top_builddir)/modules/core/libscicore.la





#### Target ######
target=$(datadir)/string


#### string : Conf files ####
libscistring_la_rootdir = $(target)
libscistring_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### string : Scilab macros ###
libscistring_la_macrodir = $(target)/macros
MACROS = macros/dec2hex.sci \
macros/hex2dec.sci \
macros/tokenpos.sci \
macros/evstr.sci \
macros/oct2dec.sci \
macros/eval.sci \
macros/base2dec.sci \
macros/stripblanks.sci \
macros/justify.sci \
macros/arobasestring2strings.sci \
macros/ascii2string.sci \
macros/fun2string.sci macros/arobasestring2strings.bin \
macros/ascii2string.bin \
macros/base2dec.bin \
macros/dec2hex.bin \
macros/eval.bin \
macros/evstr.bin \
macros/fun2string.bin \
macros/hex2dec.bin \
macros/justify.bin \
macros/oct2dec.bin \
macros/stripblanks.bin \
macros/tokenpos.bin macros/names macros/lib macros/buildmacros.sce


#### string : init scripts #### 
libscistring_la_etcdir = $(target)/etc
libscistring_la_etc_DATA = etc/string.quit etc/string.start

#### string : gateway declaration #### 
libscistring_la_sci_gatewaydir = $(target)/sci_gateway
libscistring_la_sci_gateway_DATA = sci_gateway/string

#### string : include files ####
libscistring_la_includedir=$(sciincludedir)/string
libscistring_la_include_HEADERS = includes/men_Sutils.h \
includes/gw_string.h

include $(top_srcdir)/Makefile.incl.am
