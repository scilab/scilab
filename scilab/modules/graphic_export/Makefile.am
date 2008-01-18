##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Graphic export module
### INRIA - Scilab 2008
##########

if GUI
# List of the c files
GRAPHIC_EXPORT_CPP_SOURCES = 

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_graphic_export.c

# Includes need for the compilation
libscigraphic_export_la_CPPFLAGS =  	$(JAVA_JNI_INCLUDE) \
				-Iincludes/ \
				-Isrc/cpp/ \
				-Isrc/jni/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/graphic/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes

# All the sources needed by libscigraphic_export.la
libscigraphic_export_la_SOURCES = $(GRAPHIC_EXPORT_CPP_SOURCES) $(GATEWAY_C_SOURCES)

## Libraries necessary
## Change to include other Scilab modules/lib
#libscigraphic_export_la_LIBADD = $(top_builddir)/modules/graphic_export/libscigraphic_export.la

GIWS_WRAPPERS=


else

# All the sources needed by libscigraphic_export.la
libscigraphic_export_la_SOURCES = src/nographic_export/nographic_export.c

libscigraphic_export_la_CPPFLAGS = -I$(top_srcdir)/modules/output_stream/includes
endif

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscigraphic_export.la


#### Name of the module and the path ######
modulename=graphic_export

#### graphic_export : Conf files ####
libscigraphic_export_la_rootdir = $(mydatadir)
libscigraphic_export_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### graphic_export : init scripts ####
libscigraphic_export_la_etcdir = $(mydatadir)/etc
libscigraphic_export_la_etc_DATA = etc/graphic_export.quit etc/graphic_export.start

#### graphic_export : gateway declaration ####
#libscigraphic_export_la_sci_gatewaydir = $(mydatadir)/sci_gateway
#libscigraphic_export_la_sci_gateway_DATA = sci_gateway/graphic_export_gateway.xml

#### graphic_export : include files ####
libscigraphic_export_la_includedir=$(pkgincludedir)/graphic_export
libscigraphic_export_la_include_HEADERS = includes/gw_graphic_export.h

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1

USEANT=1

include $(top_srcdir)/Makefile.incl.am
