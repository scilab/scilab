#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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



