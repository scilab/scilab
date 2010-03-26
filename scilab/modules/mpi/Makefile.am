# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006-2008 - INRIA - Sylvestre LEDRU
# Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

# List of the c files
#MPI_C_SOURCES = src/c/s_mpi_irecv.c \
#src/c/build_buff_type.c \
#src/c/s_mpi_recv.c \
#src/c/s_mpi_send.c \
#src/c/s_mpi_isend.c


# List of the fortran files
#MPI_FORTRAN_SOURCES = src/fortran/intsmpi_isend.f \
#src/fortran/intmpi.f \
#src/fortran/intsmpi_send.f \
#src/fortran/intsmpi_recv.f \
#src/fortran/intsmpi_comm_rank.f \
#src/fortran/intsmpi_irecv.f \
#src/fortran/intsmpi_init.f \
#src/fortran/intsmpi_finalize.f \
#src/fortran/intsmpi_comm_size.f
#src/fortran/mpi_init.f

MPI_TEST_EXE_SOURCES = src/c/mpitest.c
bin_PROGRAMS = mpitest mpi_pi
mpitest_SOURCES = $(MPI_TEST_EXE_SOURCES)
mpitest_LDFLAGS = $(OPENMPI_LIBS)


MPI_PI_SOURCES = src/c/mpi_pi_send.c src/c/dboard.c
mpi_pi_SOURCES = $(MPI_PI_SOURCES)
mpi_pi_LDFLAGS = $(OPENMPI_LIBS)

MPI_C_SOURCES = src/c/mappingScilabMPI.c

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/sci_mpi_init.c \
sci_gateway/c/sci_mpi_finalize.c \
sci_gateway/c/sci_mpi_comm_size.c \
sci_gateway/c/sci_mpi_comm_rank.c \
sci_gateway/c/gw_mpi.c \
sci_gateway/c/sci_mpi_recv.c \
sci_gateway/c/sci_mpi_send.c \
sci_gateway/c/sci_mpi_serialize.c \
sci_gateway/c/sci_mpi_unserialize.c \
src/c/build_buff_type.c \
src/c/s_mpi_recv.c \
src/c/s_mpi_send.c

#sci_gateway/c/sci_mpi_isend.c \
#sci_gateway/c/sci_mpi_irecv.c \
#sci_gateway/c/sci_mpi_comm_rank.c \
#sci_gateway/c/sci_mpi_comm_size.c


# Includes need for the compilation
# MPI code needs to be compile with the openmpi 'compiler' (it is a wrapper)
CC = $(OPENMPI_CC)
libscimpi_la_CFLAGS=  -Iincludes/ \
	-Isrc/c/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-DDEBUG
libscimpi_la_LDFLAGS = $(OPENMPI_LIBS)
# $(LAMPI_LDFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libscimpi.la

# All the sources needed by libscimpi.la
libscimpi_la_SOURCES =   $(GATEWAY_C_SOURCES)  $(MPI_C_SOURCES)
#$(GATEWAY_FORTRAN_SOURCES) $(MPI_FORTRAN_SOURCES)


## Libraries necessary
libscimpi_la_LIBADD = \
	$(top_builddir)/modules/localization/libscilocalization.la \
	$(top_builddir)/modules/core/libscicore.la
#	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Target ######

#### mpi : Conf files ####
libscimpi_la_rootdir = $(mydatadir)
libscimpi_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### mpi : init scripts #### 
libscimpi_la_etcdir = $(mydatadir)/etc
libscimpi_la_etc_DATA = etc/mpi.quit etc/mpi.start

#### mpi : gateway declaration #### 
libscimpi_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscimpi_la_sci_gateway_DATA = sci_gateway/mpi_gateway.xml

#### mpi : include files ####
libscimpi_la_includedir=$(pkgincludedir)/mpi
libscimpi_la_include_HEADERS = includes/gw_mpi.h

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
