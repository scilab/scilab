##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

STRING_C_SOURCES = src/c/code2str.c \
src/c/stripblanks.c \
src/c/convstr.c \
src/c/men_Sutils.c \
src/c/tokens.c \
src/c/strcatfunction.c \
src/c/partfunction.c \
src/c/InitializeString.c \
src/c/setScilabCharactersCodes.c \
src/c/cvstr.c \
src/c/getfastcode.c \
src/c/pcre_private.c \
src/c/pcre_error.c \
src/c/isletter.c \
src/c/isdigit.c \
src/c/strsubst.c \
src/c/stringsstrrchr.c \
src/c/stringsstrrev.c \
src/c/stringsCompare.c \
src/c/isalphanum.c \
src/c/NumTokens.c \
src/c/StringConvert.c \
src/c/scistrtostr.c

STRING_FORTRAN_SOURCES =

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
sci_gateway/c/sci_strtok.c \
sci_gateway/c/sci_regexp.c \
sci_gateway/c/sci_NumTokens.c


GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_string.f \
sci_gateway/fortran/strops.f

libscistring_la_CFLAGS= $(PCRE_CFLAGS) \
	-I$(srcdir)/src/c \
	-I$(srcdir)/includes/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/operations/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscistring.la

libscistring_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(PCRE_LIBS)

libscistring_la_SOURCES = $(STRING_C_SOURCES) $(STRING_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)


# For the code check (splint)
CHECK_SRC= $(STRING_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscistring_la_CFLAGS)

# commented since it is the first module to be processed
#  $(top_builddir)/modules/polynomials/libscipolynomials.la
libscistring_la_LIBADD = $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=string


#### string : Conf files ####
libscistring_la_rootdir = $(mydatadir)
libscistring_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### string : init scripts ####
libscistring_la_etcdir = $(mydatadir)/etc
libscistring_la_etc_DATA = etc/string.quit etc/string.start

#### string : gateway declaration ####
libscistring_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscistring_la_sci_gateway_DATA = sci_gateway/string_gateway.xml

#### string : include files ####
libscistring_la_includedir=$(pkgincludedir)/string
libscistring_la_include_HEADERS = includes/men_Sutils.h \
includes/gw_string.h includes/InitializeString.h \
includes/cvstr.h includes/getfastcode.h \
includes/stricmp.h includes/strsubst.h \
includes/code2str.h \
includes/NumTokens.h \
includes/StringConvert.h \
includes/scistrtostr.h

include $(top_srcdir)/Makefile.incl.am
