dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl 
dnl Copyright (C) 2012 - 2016 - Scilab Enterprises
dnl
dnl This file is hereby licensed under the terms of the GNU GPL v2.0,
dnl pursuant to article 5.3.4 of the CeCILL v.2.1.
dnl This file was originally licensed under the terms of the CeCILL v2.1,
dnl and continues to be available under such terms.
dnl For more information, see the COPYING file which you should have received
dnl along with this program.
dnl
dnl DOCBOOK detection
dnl
dnl ------------------------------------------------------
dnl Check if Docbook is usable and working
dnl
AC_DEFUN([AC_DOCBOOK], [

DOCBOOK_OK=no
DOCBOOK_ROOT=""

if test "x$ac_java_jvm_name" != "x" ; then
    # if JDK is not enabled, skip the docbook detection

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
	AC_JAVA_CHECK_JAR([saxon9he],[net.sf.saxon.Version],[Saxon XSLT Processor],"yes")
	SAXON=$PACKAGE_JAR_FILE
	if test -z "$SAXON"; then
		AC_JAVA_CHECK_JAR([saxon],[net.sf.saxon.Version],[Saxon XSLT Processor],"yes")
		SAXON=$PACKAGE_JAR_FILE
	fi
	if test -z "$SAXON"; then
		AC_JAVA_CHECK_JAR([saxon],[com.icl.saxon.Loader],[Saxon XSLT Processor])
		SAXON=$PACKAGE_JAR_FILE
	fi
	AC_SUBST(SAXON)

fi # JDK

])
