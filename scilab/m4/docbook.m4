dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl DOCBOOK detection
dnl
dnl ------------------------------------------------------
dnl Check if Docbook is usable and working
dnl
AC_DEFUN([AC_DOCBOOK], [

DOCBOOK_OK=no
DOCBOOK_ROOT=""

AC_ARG_WITH(docbook,
		AC_HELP_STRING([--with-docbook=DIR],[Set the path to the docbook package]),
		[with_docbook=$withval],
		[with_docbook='yes']
		)

	for dir in $with_docbook $SCI_SRCDIR_FULL/thirdparty/docbook/ /usr/share/sgml/docbook/stylesheet/xsl/nwalsh /usr/share/docbook2X/xslt/man/ /usr/share/xml/docbook/stylesheet/nwalsh/ /usr/share/xml/docbook/stylesheet/nwalsh/current/ /sw/share/xml/xsl/docbook-xsl /usr/share/xml/docbook/xsl-stylesheets-*/ /usr/share/sgml/docbook/xsl-stylesheets-*/; do
		if test -r "$dir/javahelp/javahelp.xsl" -a "$DOCBOOK_ROOT" = ""; then
			DOCBOOK_ROOT=$dir
        fi
	done
	if test -z "$DOCBOOK_ROOT"; then
		AC_MSG_ERROR([Could not find the Docbook root directory. If you have installed it on your system and we haven't been able to find it. Please report a bug])
	fi
	# xml.apache.org SVG Library (under mandriva for example)
	AC_JAVA_CHECK_PACKAGE([batik-all],[org.apache.batik.parser.Parser],[Apache SVG Library])
	BATIK=$PACKAGE_JAR_FILE

	if test -z "$BATIK"; then
		# Other other distribs
		AC_JAVA_CHECK_PACKAGE([batik],[org.apache.batik.parser.Parser],[Apache SVG Library],"yes")
		BATIK=$PACKAGE_JAR_FILE
	fi
	AC_SUBST(BATIK)


	# Saxon XSLT Processor
	AC_JAVA_CHECK_PACKAGE([saxon],[com.icl.saxon.Loader],[Saxon XSLT Processor])
	SAXON=$PACKAGE_JAR_FILE
	AC_SUBST(SAXON)

	# XML to PDF/other Translator
	AC_JAVA_CHECK_PACKAGE([fop],[org.apache.fop.pdf.PDFInfo],[XML to PDF Translator (fop)])
	FOP=$PACKAGE_JAR_FILE
	AC_SUBST(FOP)

	# MathML rendering solution
	AC_JAVA_CHECK_PACKAGE([jeuclid-core],[net.sourceforge.jeuclid.LayoutContext],[MathML rendering solution (at least version 3.1.X)])
	JEUCLID_CORE=$PACKAGE_JAR_FILE
	AC_SUBST(JEUCLID_CORE)

	# Commons I/O library
	AC_JAVA_CHECK_PACKAGE([commons-io],[org.apache.commons.io.output.CountingOutputStream],[Commons I/O library])
	COMMONS_IO=$PACKAGE_JAR_FILE
	AC_SUBST(COMMONS_IO)

	# XML graphics common
	AC_JAVA_CHECK_PACKAGE([xmlgraphics-commons],[org.apache.xmlgraphics.util.Service],[Commons graphics library])
	XMLGRAPHICS_COMMONS=$PACKAGE_JAR_FILE
	AC_SUBST(XMLGRAPHICS_COMMONS)

	# XML API EXT (conversion of a SVG => PNG)
	AC_JAVA_CHECK_PACKAGE([xml-apis-ext],[org.w3c.dom.svg.SVGDocument],[XML Commons external code])
	XML_APIS_EXT=$PACKAGE_JAR_FILE
	AC_SUBST(XML_APIS_EXT)


	# Avalon Framework (PDF)
	AC_JAVA_CHECK_PACKAGE([avalon-framework],[org.apache.avalon.framework.configuration.ConfigurationException],[Common framework for Java server application])
	AVALON_FRAMEWORK=$PACKAGE_JAR_FILE
	AC_SUBST(AVALON_FRAMEWORK)
	

AC_SUBST(DOCBOOK_ROOT)

])
