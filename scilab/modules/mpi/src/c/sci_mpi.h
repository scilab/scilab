/* Copyright (c) 1999 by Inria Lorraine.  All Rights Reserved */

/***
	NAME 
	MPI feature into Scilab
   PURPOSE
     provide point-to-point communication between MPI process.
   NOTES

   HISTORY
     fleury - Mar 12, 1999: Created.
     $Log: mpi_pp.c,v $
     Revision 1.3  1999/10/19 06:42:32  fleury
     Mise a jour de l archive du LORIA.
     WARNING: modif de type inconnue....

     Revision 1.2  1999/03/19 18:01:02  fleury
     Send & Recv OK.
     TODO: finalize.
     TODO: prendre en compte les communicators
     TODO: prendre en compte les status
     TODO: error handler in MPI.
     TODO: test sur #ex.
     TODO: mpi_mp.c (-;

     Revision 1.1  1999/03/19 11:07:51  fleury
     Ajout de l interface MPI.
     ceci est un premier ajout qui marche
     TODO: pouvoir lancer des scilex avec/sans fenetre
     TODO: gérer les Communicators

***/

#ifndef __SCIMPI
#define __SCIMPI
#include <stdlib.h>
#include <mpi.h>
//#include "mpi/mpi.h"
#include "machine.h"

#define SCI_MPI_MAX_COMMUNICATORS    256
#define SCI_MPI_MAX_PENDING_REQUESTS 256
#define SCI_MPI_MAX_PACKING_VECT_SIZE 1024

typedef struct pending_req {
  MPI_Request req_header;
  MPI_Request req_body;
  void* ptr_header;
  void* ptr_body;
} Pending_Req;

extern int packing_vect[SCI_MPI_MAX_PACKING_VECT_SIZE];


MPI_Comm tab_comm[SCI_MPI_MAX_COMMUNICATORS];
//int current_comm = 0;

Pending_Req tab_req[SCI_MPI_MAX_PENDING_REQUESTS];
//int current_req = 0;

#define debuglevel 10

#endif /* __SCIMPI */
