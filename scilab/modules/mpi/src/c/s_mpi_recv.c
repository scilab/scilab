/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include "sci_mpi.h"
#include "s_mpi_recv.h"


void C2F(s_mpi_recv)(double * const beginvar,   /* I/O */
		const int * const maxsize, /* INPUT */
		int * const size,          /* OUTPUT */
		const int * const source,  /* INPUT */
		const int * const tag,     /* INPUT */
		const int * const comm,    /* INPUT */
		int stat[2], int *res)   /* OUTPUT */
{
  int errcode, errclass;	/* Error mgmt */
  char error_msg[MPI_MAX_ERROR_STRING];
  int error_msg_length;
 
  MPI_Status status;
  MPI_Datatype buff_type;

  int n;			/* size of the pack vector */
  int *pack;			/* the pack vector */
  int l;			/* size of the buff in bytes */

#ifdef DEBUG
  if (debuglevel >= 5) {
    (void) fprintf(stderr, "MPI_RECV: |%f:%d|%d:%d:%d|\n", 
		   beginvar[0], *maxsize, *source, *tag, *comm);	
    (void) fflush (stderr);	
  }	
#endif /* DEBUG */

  *size = 2;
  if (*size <= *maxsize){		/* on retourne en cas d erreur une
					   matrice de taille vide */
    int *ptr_int = (int*)beginvar;
    
    ptr_int[0] = 1;	/* scalar type */
    ptr_int[1] = 0;	/* m */
    ptr_int[2] = 0;	/* n */
    ptr_int[3] = 0;	/* real */
  } 
  else {
    (void) fprintf(stderr, "Error in mpi_recv: Not enough memory: available=%d:needed=%d\n", *maxsize, *size);
    (void) fflush(stderr);
    *res = MPI_ERR_OTHER;
    return;
  }
  (void) fprintf(stderr, "Error ICI 1 \n");
    (void) fflush(stderr);
	// BUG ICI, VOIR PK CA MERDE LA ?
	errcode = MPI_Probe(*source, *tag, MPI_COMM_WORLD, &status);
  (void) fprintf(stderr, "Error ICI 2 \n");
    (void) fflush(stderr);

  if (errcode != MPI_SUCCESS){
  (void) fprintf(stderr, "Error ICI 3 \n");
    (void) fflush(stderr);

    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_recv -MPI_Probe-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
  (void) fprintf(stderr, "Error ICI 4 \n");
    (void) fflush(stderr);

  errcode = MPI_Get_count(&status, MPI_INT, &n);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_recv -MPI_Get_count-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
#ifdef DEBUG
  if (debuglevel >= 10) {
    int i;
    (void) fprintf(stderr, "MPI_RECV: %d\n", n);	
    (void) fflush(stderr);	
  }	
#endif /* DEBUG */  
  if ((pack = (int *) malloc(n * sizeof(int))) == NULL){
    (void) fprintf(stderr, "Error malloc in mpi_recv\n");
    (void) fflush(stderr);
    *res = MPI_ERR_OTHER;
    return; 
  }
  (void) fprintf(stderr, "Error ICI 5 \n");
    (void) fflush(stderr);

  errcode = MPI_Recv(pack, n, MPI_INT, *source, *tag, 
		     MPI_COMM_WORLD, &status);
  (void) fprintf(stderr, "Error ICI 6 \n");
    (void) fflush(stderr);

  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_recv -MPI_Recv-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    free(pack);
    return;
  }
#ifdef DEBUG
  if (debuglevel >= 10) {
    int i;
    (void) fprintf(stderr, "MPI_RECV: ");	
    for (i = 0; i < n; ++i) {
      (void) fprintf(stderr, "%d:", 
		     pack[i]);
    }
    (void) fprintf(stderr, "\n");
    (void) fflush(stderr);	
  }	
#endif /* DEBUG */  
  errcode = build_buff_type(n, pack, &buff_type, &l);
  free(pack);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_recv -build_buff_type-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
  if ((l/sizeof(double)) > *maxsize){ 
    (void) fprintf(stderr, "Error in mpi_recv: Not enough memory: available=%d:needed=%d\n", *maxsize, *size);
    (void) fflush(stderr);
    *res = MPI_ERR_OTHER;
    return;
  }

  errcode = MPI_Type_commit(&buff_type);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_send -MPI_Type_commit-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
  
  errcode = MPI_Recv(beginvar, 1, buff_type, *source, *tag,
		     MPI_COMM_WORLD, &status);
  stat[0] = status.MPI_SOURCE;
  stat[1] = status.MPI_TAG;
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_recv -MPI_Recv-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
#ifdef DEBUG
  if (debuglevel >= 10) {
    int i;
    char *ptr = (char*) beginvar;
    (void) fprintf(stderr, "SEND:");	
    for (i = 0; i < l; ++i) {
      (void) fprintf(stderr, "%d:", ptr[i]);
    }
    (void) fprintf(stderr, "\n");	
    (void) fflush (stderr);	
  }	
#endif /* DEBUG */


  errcode = MPI_Type_free(&buff_type);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_send -MPI_Type_free-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
  *size = l/sizeof(double);	
  *res = errcode;
} 
