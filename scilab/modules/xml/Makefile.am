#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=xml


#### xml : Conf files ####
libscixml_la_rootdir = $(mydatadir)
libscixml_la_root_DATA =  license.txt


#### scinotes : init scripts & configuration files ####
libscixml_la_etcdir = $(mydatadir)/etc
libscixml_la_etc_DATA = \
    etc/xml.quit \
    etc/xml.start

include $(top_srcdir)/Makefile.incl.am

XML_JNI_SOURCES =

XML_CXX_SOURCES = \
    src/cpp/VariableScope.cpp \
    src/cpp/XMLObject.cpp \
    src/cpp/XMLDocument.cpp \
    src/cpp/XMLElement.cpp \
    src/cpp/XMLAttr.cpp \
    src/cpp/XMLNs.cpp \
    src/cpp/XMLNodeList.cpp \
    src/cpp/XMLNodeSet.cpp \
    src/cpp/XMLXPath.cpp \
    src/cpp/XMLList.cpp \
    src/cpp/XMLNotHandledElement.cpp \
    src/cpp/XMLValidation.cpp \
    src/cpp/XMLValidationDTD.cpp \
    src/cpp/XMLValidationSchema.cpp \
    src/cpp/XMLValidationRelaxNG.cpp \
    src/cpp/XMLMlistsManagement.cpp

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_xmlRead.cpp \
    sci_gateway/cpp/sci_xmlXPath.cpp \
    sci_gateway/cpp/sci_extraction.cpp \
    sci_gateway/cpp/sci_insertion.cpp \
    sci_gateway/cpp/sci_print.cpp \
    sci_gateway/cpp/sci_xmlGetOpenDocs.cpp \
    sci_gateway/cpp/sci_xmlDelete.cpp \
    sci_gateway/cpp/sci_xmlDump.cpp \
    sci_gateway/cpp/sci_xmlReadStr.cpp \
    sci_gateway/cpp/sci_xmlFormat.cpp \
    sci_gateway/cpp/sci_percent_XMLList_e.cpp \
    sci_gateway/cpp/sci_percent_XMLAttr_e.cpp \
    sci_gateway/cpp/sci_percent_XMLList_size.cpp \
    sci_gateway/cpp/sci_percent_XMLAttr_size.cpp \
    sci_gateway/cpp/sci_percent_XMLList_length.cpp \
    sci_gateway/cpp/sci_percent_XMLAttr_length.cpp \
    sci_gateway/cpp/sci_percent_c_i_XMLAttr.cpp \
    sci_gateway/cpp/sci_percent_s_i_XMLList.cpp \
    sci_gateway/cpp/sci_xmlElement.cpp \
    sci_gateway/cpp/sci_xmlDocument.cpp \
    sci_gateway/cpp/sci_xmlNs.cpp \
    sci_gateway/cpp/sci_xmlAddNs.cpp \
    sci_gateway/cpp/sci_xmlGetNsByHref.cpp \
    sci_gateway/cpp/sci_xmlGetNsByPrefix.cpp \
    sci_gateway/cpp/sci_xmlWrite.cpp \
    sci_gateway/cpp/sci_percent_foo_i_XMLList.cpp \
    sci_gateway/cpp/sci_xmlValidationFile.cpp \
    sci_gateway/cpp/sci_xmlValidate.cpp \
    sci_gateway/cpp/sci_xmlIsValidObject.cpp \
    sci_gateway/cpp/sci_xmlAsNumber.cpp \
    sci_gateway/cpp/sci_xmlAsText.cpp \
    sci_gateway/cpp/sci_xmlRemove.cpp \
    sci_gateway/cpp/sci_xmlSetAttributes.cpp \
    sci_gateway/cpp/sci_xmlName.cpp \
    sci_gateway/cpp/sci_xmlAppend.cpp \
    sci_gateway/cpp/sci_htmlRead.cpp \
    sci_gateway/cpp/sci_htmlReadStr.cpp \
    sci_gateway/cpp/sci_htmlWrite.cpp \
    sci_gateway/cpp/sci_htmlDump.cpp

libscixml_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscixml.la
noinst_LTLIBRARIES = libscixml-algo.la

libscixml_algo_la_SOURCES = $(XML_JNI_SOURCES) $(XML_CXX_SOURCES)
libscixml_la_SOURCES = $(GATEWAY_CXX_SOURCES)
libscixml_algo_la_CPPFLAGS = $(libscixml_la_CPPFLAGS)

libscixml_la_LDFLAGS = $(AM_LDFLAGS)
libscixml_la_LIBADD = libscixml-algo.la $(XML_LIBS)

# For the code check (splint)
INCLUDE_FLAGS = $(libscixml_la_CPPFLAGS)

#### xml : include files ####
libscixml_la_includedir=$(pkgincludedir)

#### xml : gateway declaration ####
libscixml_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixml_la_sci_gateway_DATA = sci_gateway/xml_gateway.xml
