##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

STRING_C_SOURCES = src/c/code2str.c \
src/c/kmp.c \
src/c/stripblanks.c \
src/c/convstr.c \
src/c/men_Sutils.c \
src/c/strsplit.c \
src/c/CreateEmptystr.c \
src/c/tokens.c \
src/c/strcat-c.c \
src/c/Get_length.c \
src/c/strcatfunction.c \
src/c/InitializeString.c \
src/c/setScilabCharactersCodes.c \
src/c/cvstr.c \
src/c/getfastcode.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_string.c \
sci_gateway/c/sci_ascii.c \
sci_gateway/c/sci_code2str.c \
sci_gateway/c/sci_convstr.c \
sci_gateway/c/sci_emptystr.c \
sci_gateway/c/sci_grep.c \
sci_gateway/c/sci_length.c \
sci_gateway/c/sci_part.c \
sci_gateway/c/sci_str2code.c \
sci_gateway/c/sci_strcat.c \
sci_gateway/c/sci_strindex.c \
sci_gateway/c/sci_string.c \
sci_gateway/c/sci_stripblanks.c \
sci_gateway/c/sci_strsplit.c \
sci_gateway/c/sci_strsubst.c \
sci_gateway/c/sci_tokens.c


libscistring_la_CFLAGS= \
	-Isrc/c \
	-Iincludes/ \
	-Isrc/c/pcre/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscistring.la

#if STRING_V2
libscistring_la_SOURCES = $(STRING_C_SOURCES) $(STRING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
#else
#libscistring_la_SOURCES= 
#endif

libscistring_la_LIBADD = \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la
# Cyclic deps
#	$(top_builddir)/modules/core/libscicore.la





#### Target ######
modulename=STRING_V2


#### string : Conf files ####
libscistring_la_rootdir = $(mydatadir)
libscistring_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### string : init scripts ####
libscistring_la_etcdir = $(mydatadir)/etc
libscistring_la_etc_DATA = etc/STRING_V2.quit etc/STRING_V2.start

#### string : gateway declaration ####
libscistring_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscistring_la_sci_gateway_DATA = sci_gateway/STRING_V2_gateway.xml

#### string : include files ####
libscistring_la_includedir=$(pkgincludedir)/string
libscistring_la_include_HEADERS = includes/men_Sutils.h \
includes/gw_string.h includes/InitializeString.h includes/cvstr.h includes/getfastcode.h

include $(top_srcdir)/Makefile.incl.am
