dnl DOCBOOK detection
dnl
dnl Sylvestre Ledru <sylvestre.ledru@inria.fr>
dnl INRIA - Scilab 2008
dnl ------------------------------------------------------
dnl Check if Docbook is usable and working
dnl
AC_DEFUN([AC_DOCBOOK], [

DOCBOOK_OK=no
DOCBOOK_ROOT=

AC_ARG_WITH(docbook,
		AC_HELP_STRING([--with-docbook=DIR],[Set the path to the docbook package]),
		[with_docbook=$withval],
		[with_docbook='yes']
		)

	for dir in /usr/share/sgml/docbook/stylesheet/xsl/nwalsh /usr/share/docbook2X/xslt/man/ /usr/share/xml/docbook/stylesheet/nwalsh/ /sw/share/xml/xsl/docbook-xsl /usr/share/xml/docbook/xsl-stylesheets-*/; do
		if test -d "$dir"; then
			DOCBOOK_ROOT=$dir
        fi
	done

	# xml.apache.org SVG Library
	AC_JAVA_CHECK_PACKAGE([batik],[org.apache.batik.parser.Parser],[Apache SVG Library])
	BATIK=$PACKAGE_JAR_FILE
	AC_SUBST(BATIK)

	# Saxon XSLT Processor
	AC_JAVA_CHECK_PACKAGE([saxon],[com.icl.saxon.Loader],[Saxon XSLT Processor])
	SAXON=$PACKAGE_JAR_FILE
	AC_SUBST(SAXON)

	# XML to PDF Translator
	AC_JAVA_CHECK_PACKAGE([fop],[org.apache.fop.pdf.PDFInfo],[XML to PDF Translator (fop)])
	FOP=$PACKAGE_JAR_FILE
	AC_SUBST(FOP)

	# MathML rendering solution
	AC_JAVA_CHECK_PACKAGE([jeuclid-core],[net.sourceforge.jeuclid.MathBase],[MathML rendering solution])
	JEUCLID_CORE=$PACKAGE_JAR_FILE
	AC_SUBST(JEUCLID_CORE)

AC_SUBST(DOCBOOK_ROOT)

])
