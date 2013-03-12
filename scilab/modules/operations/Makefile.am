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
			src/c/matrix_finite.c \
            src/c/matrix_kronecker.c \
            src/c/matrix_transpose.c \
			src/c/operations_tools.c

OPERATIONS_CPP_SOURCES =  src/cpp/operations.cpp \
			src/cpp/types_substraction.cpp \
			src/cpp/types_multiplication.cpp \
			src/cpp/types_divide.cpp \
			src/cpp/types_ldivide.cpp \
			src/cpp/types_addition.cpp \
			src/cpp/types_power.cpp \
			src/cpp/types_finite.cpp \
			src/cpp/types_comparison_eq.cpp \
			src/cpp/types_comparison_lt_le_gt_ge.cpp \
			src/cpp/types_kronecker.cpp \
			src/cpp/types_comparison_ne.cpp \
			src/cpp/types_bitwiseOp.cpp

pkglib_LTLIBRARIES = libscioperations.la

libscioperations_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioperations_la_CPPFLAGS = \
	-I$(top_srcdir)/modules/operations/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/types/includes \
	-I$(top_srcdir)/modules/symbol/includes \
	-I$(top_srcdir)/modules/ast/includes \
	-I$(top_srcdir)/modules/core/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/system_env/includes \
	$(EIGEN_CPPFLAGS) \
	$(AM_CPPFLAGS)

libscioperations_la_SOURCES = $(OPERATIONS_C_SOURCES) $(OPERATIONS_CPP_SOURCES)

libscioperations_la_LIBADD =

#### Target ######
modulename=operations


#### operations : Conf files ####
#### libscioperations_la_rootdir = $(mydatadir)
#### libscioperations_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### operations : init scripts ####
libsciboolean_la_etcdir = $(mydatadir)/etc
libsciboolean_la_etc_DATA = etc/operations.quit etc/operations.start

include $(top_srcdir)/Makefile.incl.am
