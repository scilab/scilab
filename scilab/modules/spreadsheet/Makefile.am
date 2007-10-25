##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

SPREADSHEET_C_SOURCES = src/c/xls.c \
src/c/olestream-unwrap.c \
src/c/pldstr.c \
src/c/ole.c \
src/c/bytedecoders.c \
src/c/logger.c \
src/c/ripole.c \
src/c/bt-int.c 


GATEWAY_C_SOURCES = sci_gateway/c/gw_spreadsheet.c \
sci_gateway/c/sci_xls_read.c \
sci_gateway/c/sci_xls_open.c \
sci_gateway/c/intreadxls.c 


MODULE_CHECKS = 

libscispreadsheet_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

pkglib_LTLIBRARIES = libscispreadsheet.la

libscispreadsheet_la_SOURCES = $(SPREADSHEET_C_SOURCES) $(GATEWAY_C_SOURCES)

libscispreadsheet_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
    $(top_builddir)/libs/libst/libscilibst.la 





#### Target ######
modulename=spreadsheet


#### spreadsheet : Conf files ####
libscispreadsheet_la_rootdir = $(mydatadir)
libscispreadsheet_la_root_DATA = changelog.txt licence.txt readme.txt version.xml


#### spreadsheet : init scripts #### 
libscispreadsheet_la_etcdir = $(mydatadir)/etc
libscispreadsheet_la_etc_DATA = etc/spreadsheet.quit etc/spreadsheet.start

#### spreadsheet : gateway declaration #### 
libscispreadsheet_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscispreadsheet_la_sci_gateway_DATA = sci_gateway/spreadsheet_gateway.xml

#### spreadsheet : include files ####
libscispreadsheet_la_includedir=$(pkgincludedir)/spreadsheet
libscispreadsheet_la_include_HEADERS = includes/gw_spreadsheet.h


#### spreadsheet : demos files ####
libscispreadsheet_la_demosdir=$(mydatadir)/demos/
libscispreadsheet_la_demos_DATA = demos/xls/Testo1.xls \
demos/xls/o1.xls \
demos/xls/Test1.xls \
demos/xls/Testobig.xls \
demos/xls/Testbig.xls \
demos/xls/t1.xls \
demos/xls/README

include $(top_srcdir)/Makefile.incl.am
