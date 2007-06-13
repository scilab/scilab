##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### TEMPLATE MAKEFILE 
### INRIA - Scilab 2007
##########

# List of the c files
RENDERER_C_SOURCES = src/c/

# List of the gateway c files
GATEWAY_C_SOURCES = 

# Includes need for the compilation
libscirenderer_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libscirenderer.la

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = $(RENDERER_C_SOURCES) $(GATEWAY_C_SOURCES) 

## Libraries necessary
## Change to include other Scilab modules/lib
libscirenderer_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Name of the module and the path ######
modulename=renderer

#### renderer : Conf files ####
libscirenderer_la_rootdir = $(mydatadir)
libscirenderer_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### renderer : init scripts #### 
libscirenderer_la_etcdir = $(mydatadir)/etc
libscirenderer_la_etc_DATA = etc/renderer.quit etc/renderer.start

#### renderer : gateway declaration #### 
libscirenderer_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscirenderer_la_sci_gateway_DATA = sci_gateway/renderer_gateway.xml

#### renderer : include files ####
libscirenderer_la_includedir=$(pkgincludedir)/renderer
libscirenderer_la_include_HEADERS = includes/gw_renderer.h

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1

USEANT=1

include $(top_srcdir)/Makefile.incl.am
