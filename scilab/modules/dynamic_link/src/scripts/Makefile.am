#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
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

# Without this, flags will be overriden
AM_CFLAGS=@CFLAGS@
AM_CXXFLAGS=@CXXFLAGS@
AM_FFLAGS=@FFLAGS@
AM_LDFLAGS=@LDFLAGS@

ACLOCAL_AMFLAGS = -I m4

# Definitions are already in machine.h, avoid double definition errors
DEFS=

# The name of the library is not important but must remains the same
pkglib_LTLIBRARIES = libsciexternal.la

# Fake sources in order to make believe to the autotool that we are going
# to compile some C/C++, fortran sources.
# Thanks to this, automake will generate compilation mydatadirs for 
# these 3 languages.
# Don't change name or order 
libsciexternal_la_SOURCES = foo.c foo2.f foo3.cxx foo4.cpp foo5.f90



