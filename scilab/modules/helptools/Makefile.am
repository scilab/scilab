##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2008
##########

HELPTOOLS_SOURCES = src/jni/BuildDocObject.cpp
#	src/cpp/buildDoc.cpp

GATEWAY_C_SOURCES = sci_gateway/cpp/sci_buildDoc.cpp \
			sci_gateway/c/gw_helptools.c

pkglib_LTLIBRARIES = libscihelptools.la

libscihelptools_la_CFLAGS=  	-Iincludes/ 

libscihelptools_la_CPPFLAGS= -Isrc/jni/ \
				-Iincludes/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_builddir)/modules/output_stream/includes/

libscihelptools_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscihelptools_la_SOURCES =  $(GATEWAY_C_SOURCES) $(HELPTOOLS_SOURCES)

GIWS_WRAPPERS = src/jni/BuildDocObject.giws.xml

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
bin/sci2pdf \
bin/sci2jh \
bin/manrev2sci \
bin/sci2html \
bin/sciviewhelp


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
doc/addonprefs.png \
doc/doc.pdf \
doc/doc.xml

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

libscihelptools_la_doc_html_imagesdir = $(mydatadir)/doc/html/images
libscihelptools_la_doc_html_images_DATA = doc/html/images/draft.png

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

libscihelptools_la_doc_html_resourcesdir = $(mydatadir)/doc/html/resources
libscihelptools_la_doc_html_resources_DATA = doc/html/resources/addonprefs.png


include $(top_srcdir)/Makefile.incl.am
