/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include "sci_mpi.h"
#include "s_mpi_isend.h"
#include "build_buff_type.h"

void  C2F(s_mpi_isend)(int *pack, int *n,
		 double *buff,
		 int *dest,
		 int *tag,
		 int *comm,
		 int *request,
		 int *res)
{
  int errcode, errclass;	/* Error mgmt */
  char error_msg[MPI_MAX_ERROR_STRING];
  int error_msg_length;
  int entry = 0;
  MPI_Datatype buff_type;
  int l;			/* size of the buff to send in bytes */

				/* Try to locate an entry in the
				   tab_req array */

  *request = -1;
  if (*n >= SCI_MPI_MAX_PACKING_VECT_SIZE){
    (void) fprintf(stderr, "Error in mpi_isend: packing vector is too large\n");
    (void) fflush(stderr);
    *res = MPI_ERR_OTHER;
    return;
  }

  while ((entry < SCI_MPI_MAX_PENDING_REQUESTS) &&
	 (tab_req[entry].ptr_header != NULL) &&
	 (tab_req[entry].ptr_body != NULL)) {
    entry++;
  }
  if (entry == SCI_MPI_MAX_PENDING_REQUESTS){
    (void) fprintf(stderr, "Error in mpi_isend: Too much pending requests\n");
    (void) fflush(stderr);
    *res = MPI_ERR_OTHER;
    return;
  }

  errcode = build_buff_type(*n, pack, &buff_type, &l);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error in mpi_isend -build_buff_type-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
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
				/* Send the packing */
  
  errcode = MPI_Isend(pack, *n, MPI_INT, *dest, *tag, MPI_COMM_WORLD, 
		      &tab_req[entry].req_header);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_send -MPI_ISend-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
  				/* Send the data... */
  errcode = MPI_Isend(buff, 1, buff_type, *dest, *tag, MPI_COMM_WORLD, 
		      &tab_req[entry].req_body);  
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_send -MPI_ISend-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *res = errcode;
    return;
  }
				/* If ptr_header == &ptr_header the
				   entry is no free *** BUT *** you do 
				not have to free this ptr!!! just
				set them to the NULL value.*/
  tab_req[entry].ptr_header = &tab_req[entry].ptr_header;
  tab_req[entry].ptr_body =  &tab_req[entry].ptr_body;
  
  errcode = MPI_Type_free(&buff_type);
  if (errcode != MPI_SUCCESS){
    MPI_Error_string(errcode, error_msg, &error_msg_length);
    MPI_Error_class(errcode, &errclass);
    (void) fprintf(stderr, "Error mpi_isend -MPI_Type_free-: %d:%s\n", 
		   errclass, error_msg);
    (void) fflush(stderr);
    *request = entry;
    *res = errcode;
    return;
  }

  *request = entry;
  *res = MPI_SUCCESS;
} /* scimpiisend */
