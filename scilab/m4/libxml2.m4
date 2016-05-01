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
dnl libxml is mandatory in Scilab 
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_LIBXML2], [

AC_ARG_WITH(libxml2,
		AC_HELP_STRING([--with-libxml2=PREFIX],[Set the path to your libxml2 installation]),
		[with_libxml2=$withval],
		[with_libxml2='yes']
		)

if test "$with_libxml2" != 'yes' -a "$with_libxml2" != 'no'; then
    # Look if xml-config xml2_config (which provides cflags and ldflags) is available
    AC_MSG_CHECKING([libxml2, for xml-config])
    XML_CONFIG="$with_libxml2/bin/xml2-config"
    if test -x "$XML_CONFIG"; then
        AC_MSG_RESULT([$XML_CONFIG])
    else
        AC_MSG_ERROR([Unable to find $XML_CONFIG. Please check the path you provided])
    fi
else
    if $WITH_DEVTOOLS; then # Scilab thirparties
        XML_CONFIG="$DEVTOOLS_BINDIR/xml2-config"
    else
        AC_CHECK_PROGS(XML_CONFIG,xml2-config,no)
    fi
    if test "x$XML_CONFIG" = "xno"; then
        AC_MSG_ERROR([Unable to find xml2-config in the path. Please check your installation of libxml2])
    fi
fi
saved_cflags=$CFLAGS
saved_LIBS="$LIBS"

XML_FLAGS=`$XML_CONFIG --cflags`
XML_LIBS=`$XML_CONFIG --libs`
XML_VERSION=`$XML_CONFIG --version`

CFLAGS="$CFLAGS $XML_FLAGS"
LIBS="$LIBS $XML_LIBS"

AC_CHECK_LIB([xml2], [xmlReaderForFile],
               [],
               [AC_MSG_ERROR([libxml2 : library missing. (Cannot find symbol xmlReaderForFile). Check if libxml2 is installed and if the version is correct])]
               )
AC_CHECK_LIB([xml2], [xmlBufferDetach],
               [],
               [AC_MSG_ERROR([libxml2 : library missing. (Cannot find symbol xmlBufferDetach). Check if libxml2 is installed and if the version is correct])]
               )

#AC_CHECK_HEADERS([libxml/xmlreader.h])
#AC_CHECK_HEADERS([libxml/parser.h])
#AC_CHECK_HEADERS([libxml/tree.h])

#AC_CHECK_HEADERS([libxml/xpath.h])
#AC_CHECK_HEADERS([libxml/xpathInternals.h])

CFLAGS=$saved_cflags
LIBS="$saved_LIBS"

AC_SUBST(XML_FLAGS)
AC_SUBST(XML_LIBS)
AC_SUBST(XML_VERSION)

AC_DEFINE_UNQUOTED([LIBXML_FLAGS],["$XML_FLAGS"],[libXML2 flags])
AC_DEFINE_UNQUOTED([LIBXML_LIBS],["$XML_LIBS"],[libXML2 library])

#CFLAGS="$CFLAGS $XML_FLAGS"
#AC_CHECK_LIB(xml2,xmlInitParserCtxt,,[AC_MSG_ERROR([libxml2 : library missing])])


#AC_CHECK_HEADERS([libxml/tree.h],,[AC_MSG_ERROR([libxml2 : library missing missing])])

# Gets compilation and library flags
])
