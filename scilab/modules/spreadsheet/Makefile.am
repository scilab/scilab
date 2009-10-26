##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

SPREADSHEET_C_SOURCES = src/c/xls.c

RIPOLE_SOURCES = src/c/ripole/olestream-unwrap.c \
src/c/ripole/pldstr.c \
src/c/ripole/ole.c \
src/c/ripole/bytedecoders.c \
src/c/ripole/logger.c \
src/c/ripole/ripole.c \
src/c/ripole/bt-int.c


GATEWAY_C_SOURCES = sci_gateway/c/gw_spreadsheet.c \
sci_gateway/c/sci_xls_read.c \
sci_gateway/c/sci_xls_open.c

libscispreadsheet_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(srcdir)/src/c/ripole \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/io/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/fileio/includes

pkglib_LTLIBRARIES = libscispreadsheet.la

libscispreadsheet_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscispreadsheet_la_SOURCES = $(SPREADSHEET_C_SOURCES) $(GATEWAY_C_SOURCES) $(RIPOLE_SOURCES)

# For the code check (splint)
CHECK_SRC= $(SPREADSHEET_C_SOURCES) $(GATEWAY_C_SOURCES) $(RIPOLE_SOURCES)
INCLUDE_FLAGS = $(libscispreadsheet_la_CFLAGS)

libscispreadsheet_la_LIBADD =  $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=spreadsheet


#### spreadsheet : Conf files ####
libscispreadsheet_la_rootdir = $(mydatadir)
libscispreadsheet_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�spreadsheet : init scripts ####
libscispreadsheet_la_etcdir = $(mydatadir)/etc
libscispreadsheet_la_etc_DATA = etc/spreadsheet.quit etc/spreadsheet.start

####�spreadsheet : gateway declaration ####
libscispreadsheet_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscispreadsheet_la_sci_gateway_DATA = sci_gateway/spreadsheet_gateway.xml

include $(top_srcdir)/Makefile.incl.am
