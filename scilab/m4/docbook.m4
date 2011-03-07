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

	for dir in $with_docbook $SCI_SRCDIR_FULL/thirdparty/docbook/ /usr/share/sgml/docbook/stylesheet/xsl/nwalsh /usr/share/docbook2X/xslt/man/ /usr/share/xml/docbook/stylesheet/nwalsh/ /usr/share/xml/docbook/stylesheet/nwalsh/current/ /sw/share/xml/xsl/docbook-xsl /usr/share/xml/docbook/xsl-stylesheets-*/ /usr/share/sgml/docbook/xsl-stylesheets-*/ /usr/share/sgml/docbook/xsl-stylesheets; do
		if test -r "$dir/javahelp/javahelp.xsl" -a "$DOCBOOK_ROOT" = ""; then
			DOCBOOK_ROOT=$dir
        fi
	done
	if test -z "$DOCBOOK_ROOT"; then
		AC_MSG_ERROR([Could not find the Docbook root directory. If you have installed it on your system and we haven't been able to find it. Please report a bug])
	fi


	# Saxon XSLT Processor
	AC_JAVA_CHECK_PACKAGE([saxon],[com.icl.saxon.Loader],[Saxon XSLT Processor])
	SAXON=$PACKAGE_JAR_FILE
	AC_SUBST(SAXON)

    # JLaTeXMath FOP
    AC_JAVA_CHECK_PACKAGE([jlatexmath-fop],[org.scilab.forge.jlatexmath.fop.JLaTeXMathObj],[LaTex Rendering - FOP plugin])
    JLATEXMATH_FOP=$PACKAGE_JAR_FILE
    AC_SUBST(JLATEXMATH_FOP)

AC_SUBST(DOCBOOK_ROOT)

])
