##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2008
##########

pkglib_LTLIBRARIES = libsciscicos_blocks.la

#### Target ######
modulename=scicos_blocks

include $(top_srcdir)/Makefile.incl.am

if SCICOS

SCICOS_BLOCKS_C_SOURCES = 

SCICOS_BLOCKS_FORTRAN_SOURCES = 

GATEWAY_C_SOURCES = 

GATEWAY_FORTRAN_SOURCES = 

libsciscicos_blocks_la_CFLAGS= -Iincludes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/scicos/includes


libsciscicos_la_SOURCES = $(SCICOS_BLOCKS_C_SOURCES) $(SCICOS_BLOCKS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsciscicos_la_LIBADD = \
  $(top_builddir)/modules/scicos/libsciscicos.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la


#### scicos : Conf files ####
libsciscicos_la_rootdir = $(mydatadir)
libsciscicos_la_root_DATA = changelog.txt licence.txt readme.txt version.xml


#### scicos : init scripts ####
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos_blocks.quit etc/scicos_blocks.start



#### scicos : include files ####
libsciscicos_blocks_la_includedir=$(pkgincludedir)/scicos_blocks
libsciscicos_blocks_la_include_HEADERS = 

endif


#### WORKAROUND ####
# Do not use the global clean-local rule because Scicos help is not managed
# like the rest of Scilab
#clean-local:
#	@for dir in $(MACRODIRS) $(MACROSDIRSEXT) ; do \
#		echo "rm -f $(srcdir)/$$dir/$(MACROBINMASK)"; \
#        rm -f $(srcdir)/$$dir/$(MACROBINMASK); \
#    done;
