dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

	for dir in $with_docbook $SCI_SRCDIR_FULL/thirdparty/docbook/ /usr/share/sgml/docbook/stylesheet/xsl/nwalsh /usr/share/docbook2X/xslt/man/ /usr/share/xml/docbook/stylesheet/nwalsh/ /usr/share/xml/docbook/stylesheet/nwalsh/current/ /sw/share/xml/xsl/docbook-xsl /usr/share/xml/docbook/xsl-stylesheets-*/ /usr/share/sgml/docbook/xsl-stylesheets-*/ /usr/share/sgml/docbook/xsl-stylesheets /usr/share/sgml/docbook/xsl-ns-stylesheets-*/ /usr/share/xml/docbook/stylesheet/docbook-xsl /opt/local/share/xsl/docbook-xsl; do
		if test -r "$dir/fo/docbook.xsl" -a "$DOCBOOK_ROOT" = ""; then
			DOCBOOK_ROOT=$dir
        fi
	done
	if test -z "$DOCBOOK_ROOT"; then
		AC_MSG_ERROR([Could not find the Docbook root directory. If you have installed it on your system and we haven't been able to find it. Please report a bug])
	fi
	AC_SUBST(DOCBOOK_ROOT)


	# Saxon XSLT Processor, as the JVM implementation is unable to parse the docbook xsl files
	# check Saxon-HE 9.5 first then fallback to Saxon-6.5
	AC_JAVA_CHECK_PACKAGE([saxon9he],[net.sf.saxon.Version],[Saxon XSLT Processor],"yes")
	SAXON=$PACKAGE_JAR_FILE
	if test -z "$SAXON"; then
		AC_JAVA_CHECK_PACKAGE([saxon],[net.sf.saxon.Version],[Saxon XSLT Processor],"yes")
		SAXON=$PACKAGE_JAR_FILE
	fi
	if test -z "$SAXON"; then
		AC_JAVA_CHECK_PACKAGE([saxon],[com.icl.saxon.Loader],[Saxon XSLT Processor])
		SAXON=$PACKAGE_JAR_FILE
	fi
	AC_SUBST(SAXON)

])
