##########
### Antoine ELIAS <antoine.elias@scilab.org>
### INRIA - Scilab 2008
##########

OPERATIONS_C_SOURCES = 	src/c/doublecomplex.c \
			src/c/matrix_multiplication.c \
			src/c/matrix_division.c

OPERATIONS_CPP_SOURCES =  src/cpp/operations.cpp \
			src/cpp/types_substraction.cpp \
			src/cpp/types_multiplication.cpp \
			src/cpp/types_divide.cpp \
			src/cpp/types_addition.cpp

pkglib_LTLIBRARIES = libscioperations.la

libscioperations_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioperations_la_CFLAGS = 	-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/elementary_functions/includes

libscioperations_la_CXXFLAGS = 	-I$(top_srcdir)/modules/operations/includes \
						-I$(top_srcdir)/modules/types/includes \
						-I$(top_srcdir)/modules/core/includes


libscioperations_la_SOURCES = $(OPERATIONS_C_SOURCES) $(OPERATIONS_CPP_SOURCES)

libscioperations_la_LIBADD = 
if USE_EMBEDDED_LAPACK
libscioperations_la_LIBADD += $(top_builddir)/libs/lapack/libscilapack.la
endif

if USE_EMBEDDED_BLAS
libscioperations_la_LIBADD += $(top_builddir)/libs/blas/libsciblas.la
endif

#### Target ######
modulename=operations


#### operations : Conf files ####
#### libscioperations_la_rootdir = $(mydatadir)
#### libscioperations_la_root_DATA = changelog.txt license.txt readme.txt version.xml


include $(top_srcdir)/Makefile.incl.am
