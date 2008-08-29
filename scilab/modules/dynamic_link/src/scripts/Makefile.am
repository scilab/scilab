##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

# Without this, flags will be overriden
AM_CFLAGS=@CFLAGS@
AM_CXXFLAGS=@CXXFLAGS@
AM_FFLAGS=@FFLAGS@
AM_LDFLAGS=@LDFLAGS@

# The name of the library is not important but must remains the same
pkglib_LTLIBRARIES = libsciexternal.la

# Fake sources in order to make believe to the autotool that we are going
# to compile some C/C++, fortran sources.
# Thanks to this, automake will generate compilation mydatadirs for 
# these 3 languages.
# Don't change name or order 
libsciexternal_la_SOURCES = foo.c foo2.f foo3.cxx


