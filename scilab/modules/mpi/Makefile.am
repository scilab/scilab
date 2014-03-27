# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

MPI_C_SOURCES =     src/c/serialization.c \
    src/c/deserialization.c

# List of the gateway c files
GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_mpi_init.c \
    sci_gateway/c/sci_mpi_finalize.c \
    sci_gateway/c/sci_mpi_comm_size.c \
    sci_gateway/c/sci_mpi_comm_rank.c \
    sci_gateway/c/gw_mpi.c \
    sci_gateway/c/sci_mpi_recv.c \
    sci_gateway/c/sci_mpi_send.c \
    sci_gateway/c/sci_mpi_irecv.c \
    sci_gateway/c/sci_mpi_isend.c \
    sci_gateway/c/sci_mpi_bcast.c \
    sci_gateway/c/sci_mpi_wait.c \
    sci_gateway/c/sci_mpi_get_processor_name.c

# Includes need for the compilation
# MPI code needs to be compile with the openmpi 'compiler' (it is a wrapper)
CC = $(OPENMPI_CC)
libscimpi_la_CFLAGS=  -Iincludes/ \
	-Isrc/c/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-DDEBUG
# libscimpi_la_LDFLAGS = $(LAMPI_LDFLAGS)
libscimpi_la_LIBADD = $(OPENMPI_LIBS)

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libscimpi.la

# All the sources needed by libscimpi.la
libscimpi_la_SOURCES =   $(GATEWAY_C_SOURCES)  $(MPI_C_SOURCES)
#$(GATEWAY_FORTRAN_SOURCES) $(MPI_FORTRAN_SOURCES)


#### Target ######
modulename=mpi

#### mpi : Conf files ####
libscimpi_la_rootdir = $(mydatadir)
libscimpi_la_root_DATA = readme.txt

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
