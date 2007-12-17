##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

STRING_C_SOURCES = src/c/code2str.c \
src/c/stripblanks.c \
src/c/convstr.c \
src/c/men_Sutils.c \
src/c/strsplit.c \
src/c/CreateEmptystr.c \
src/c/tokens.c \
src/c/strcatfunction.c \
src/c/InitializeString.c \
src/c/setScilabCharactersCodes.c \
src/c/cvstr.c \
src/c/getfastcode.c \
src/c/pcre_private.c \
src/c/isletter.c \
src/c/isdigit.c \
src/c/strsubst.c \
src/c/stringsstrrchr.c \
src/c/stringsstrrev.c

STRING_FORTRAN_SOURCES =src/fortran/strdsp.f

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
sci_gateway/c/sci_tokens.c \
sci_gateway/c/sci_strcmp.c \
sci_gateway/c/sci_isletter.c \
sci_gateway/c/sci_isdigit.c \
sci_gateway/c/sci_isalphanum.c \
sci_gateway/c/sci_isascii.c \
sci_gateway/c/sci_strchr.c \
sci_gateway/c/sci_strcspn.c \
sci_gateway/c/sci_strncpy.c \
sci_gateway/c/sci_strrchr.c \
sci_gateway/c/sci_strrev.c \
sci_gateway/c/sci_strspn.c \
sci_gateway/c/sci_strstr.c \
sci_gateway/c/sci_strtod.c \
sci_gateway/c/sci_strtok.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_string.f \
sci_gateway/fortran/strops.f

libscistring_la_CFLAGS= \
	-Isrc/c \
	-Iincludes/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscistring.la

#if STRING_V2
libscistring_la_SOURCES = $(STRING_C_SOURCES) $(STRING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
#else
#libscistring_la_SOURCES= 
#endif

libscistring_la_LIBADD = 
#	$(top_builddir)/modules/polynomials/libscipolynomials.la \
#	$(top_builddir)/libs/MALLOC/libscimalloc.la
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
