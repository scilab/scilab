##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

EXCEL_C_SOURCES = src/c/xls.c \
src/c/olestream-unwrap.c \
src/c/pldstr.c \
src/c/ole.c \
src/c/bytedecoders.c \
src/c/logger.c \
src/c/ripole.c \
src/c/bt-int.c 


GATEWAY_C_SOURCES = sci_gateway/c/sci_xls_read.c \
sci_gateway/c/sci_xls_open.c \
sci_gateway/c/intreadxls.c 


MODULE_CHECKS = 

libsciexcel_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

pkglib_LTLIBRARIES = libsciexcel.la

libsciexcel_la_SOURCES = $(EXCEL_C_SOURCES) $(GATEWAY_C_SOURCES)

libsciexcel_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
    $(top_builddir)/libs/libst/libscilibst.la 





#### Target ######
modulename=excel


#### excel : Conf files ####
libsciexcel_la_rootdir = $(mydatadir)
libsciexcel_la_root_DATA = changelog.txt licence.txt readme.txt version.xml


#### excel : init scripts #### 
libsciexcel_la_etcdir = $(mydatadir)/etc
libsciexcel_la_etc_DATA = etc/excel.quit etc/excel.start

#### excel : gateway declaration #### 
libsciexcel_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciexcel_la_sci_gateway_DATA = sci_gateway/excel_gateway.xml

#### excel : include files ####
libsciexcel_la_includedir=$(pkgincludedir)/excel
libsciexcel_la_include_HEADERS = includes/gw_excel.h


#### excel : demos files ####
libsciexcel_la_demosdir=$(mydatadir)/demos/
libsciexcel_la_demos_DATA = demos/excel/Testo1.xls \
demos/excel/o1.xls \
demos/excel/Test1.xls \
demos/excel/Testobig.xls \
demos/excel/Testbig.xls \
demos/excel/t1.xls \
demos/excel/README

include $(top_srcdir)/Makefile.incl.am
