##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2008
##########

if GUI
if BUILD_HELP

USEANT=1

HELPTOOLS_SOURCES = src/jni/BuildDocObject.cpp \
	src/jni/GiwsException.cpp

GATEWAY_C_SOURCES = sci_gateway/c/gw_helptools.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_buildDoc.cpp

GIWS_WRAPPERS = src/jni/BuildDocObject.giws.xml

libscihelptools_la_LIBADD = $(top_builddir)/modules/jvm/libscijvm.la

if GIWS
BUILT_SOURCES=giws
endif

else
GATEWAY_C_SOURCES = sci_gateway/nogui/nogui.c
endif

else
GATEWAY_C_SOURCES = sci_gateway/nogui/nogui.c
endif

pkglib_LTLIBRARIES = libscihelptools.la

libscihelptools_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/jvm/includes


libscihelptools_la_CPPFLAGS= $(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/src/jni/ \
				-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes/

libscihelptools_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscihelptools_la_SOURCES =  $(GATEWAY_C_SOURCES) $(HELPTOOLS_SOURCES) $(GATEWAY_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscihelptools_la_CFLAGS)


#### Target ######
modulename=helptools


#### helptools : Conf files ####
libscihelptools_la_rootdir = $(mydatadir)
libscihelptools_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### helptools : init scripts ####
libscihelptools_la_etcdir = $(mydatadir)/etc
libscihelptools_la_etc_DATA = etc/helptools.quit etc/helptools.start

#### helptools : bin files ####
libscihelptools_la_bindir = $(mydatadir)/bin
libscihelptools_la_bin_DATA = bin/scivalid \
bin/man2sci \
bin/manrev2sci 

#### helptools : gateway declaration ####
libscihelptools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihelptools_la_sci_gateway_DATA = sci_gateway/helptools_gateway.xml


#### helptools : css files ####
libscihelptools_la_cssdir = $(mydatadir)/css
libscihelptools_la_css_DATA = css/html.css \
css/htmlhelp.css \
css/javahelp.css


#### helptools : xsl files ####
libscihelptools_la_xsl_convertdir = $(mydatadir)/xsl/convert
libscihelptools_la_xsl_convert_DATA = \
xsl/convert/man2manrev.xsl \
xsl/convert/manrev2sci.xsl

libscihelptools_la_xsl_fodir = $(mydatadir)/xsl/fo
libscihelptools_la_xsl_fo_DATA = xsl/fo/docbook.xsl


#### helptools : schema files ####
libscihelptools_la_schemadir = $(mydatadir)/schema
libscihelptools_la_schema_DATA = schema/scilab.rnc


#### helptools : doc files ####
libscihelptools_la_docdir = $(mydatadir)/doc
libscihelptools_la_doc_DATA = \
doc/addonprefs.png \
doc/doc.pdf \
doc/doc.xml


#### helptools : doc [html] files ####
libscihelptools_la_doc_htmldir = $(mydatadir)/doc/html
libscihelptools_la_doc_html_DATA = \
doc/html/ch01.html \
doc/html/ch02.html \
doc/html/ch03.html \
doc/html/ch04.html \
doc/html/ch05.html \
doc/html/ch06.html \
doc/html/ch07.html \
doc/html/html.css \
doc/html/index.html


#### helptools : doc [images] files ####
libscihelptools_la_doc_html_imagesdir = $(mydatadir)/doc/html/images
libscihelptools_la_doc_html_images_DATA = doc/html/images/draft.png


#### helptools : doc [images - callouts] files ####
libscihelptools_la_doc_html_images_calloutsdir = $(mydatadir)/doc/html/images/callouts
libscihelptools_la_doc_html_images_callouts_DATA = \
doc/html/images/callouts/1.png \
doc/html/images/callouts/10.png \
doc/html/images/callouts/11.png \
doc/html/images/callouts/12.png \
doc/html/images/callouts/13.png \
doc/html/images/callouts/14.png \
doc/html/images/callouts/15.png \
doc/html/images/callouts/2.png \
doc/html/images/callouts/3.png \
doc/html/images/callouts/4.png \
doc/html/images/callouts/5.png \
doc/html/images/callouts/6.png \
doc/html/images/callouts/7.png \
doc/html/images/callouts/8.png \
doc/html/images/callouts/9.png


#### helptools : doc [html - resources] files ####
libscihelptools_la_doc_html_resourcesdir = $(mydatadir)/doc/html/resources
libscihelptools_la_doc_html_resources_DATA = doc/html/resources/addonprefs.png

include $(top_srcdir)/Makefile.incl.am

