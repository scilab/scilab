##########
### Antoine ELIAS <antoine.elias@scilab.org>
### INRIA - Scilab 2008
##########

OPERATIONS_C_SOURCES = 	src/c/doublecomplex.c \
			src/c/matrix_multiplication.c \
			src/c/matrix_addition.c \
			src/c/matrix_substraction.c \
			src/c/matrix_division.c \
			src/c/matrix_power.c \
			src/c/matrix_finite.c

pkglib_LTLIBRARIES = libscioperations.la

libscioperations_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioperations_la_CFLAGS = 	-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/elementary_functions/includes \
				-I$(top_srcdir)/modules/output_stream/includes

libscioperations_la_CXXFLAGS = 	-I$(top_srcdir)/modules/operations/includes \
						-I$(top_srcdir)/modules/types/includes \
						-I$(top_srcdir)/modules/core/includes


libscioperations_la_SOURCES = $(OPERATIONS_C_SOURCES)

libscioperations_la_LIBADD =

#### Target ######
modulename=operations


#### operations : Conf files ####
#### libscioperations_la_rootdir = $(mydatadir)
#### libscioperations_la_root_DATA = changelog.txt license.txt readme.txt version.xml


include $(top_srcdir)/Makefile.incl.am
