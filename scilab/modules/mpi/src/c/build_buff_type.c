/**
 * Copyright (c) 1999 - INRIA Lorraine
 * Copyright (c) 2007 - Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt
 */
#include <stdlib.h>
#include <stdio.h>
#include "sci_mpi.h"
#include "build_buff_type.h"

int build_buff_type(int n, int const * const pack, 
		MPI_Datatype * const buff_type, int * const size)
/* Given a pack vector (i.e., |int|double|+)  the function builds the
   corresponding MPI type */
{
  int *array_of_blocklenghts;	/* Virtual structure */
  MPI_Aint *array_of_displacements;
  MPI_Datatype *array_of_types;
  int displacement = 0;
  int errcode = MPI_SUCCESS;
  int i;			/* loop index */
  int nb_blocks;		/* nb of blocks  */

  *size = 0;
  if ((array_of_blocklenghts = (int *) malloc(n * sizeof(int))) ==  NULL){
    (void) fprintf(stderr, "Error malloc in mpi_send\n");
    (void) fflush(stderr);
    return MPI_ERR_OTHER;
  }
  if ((array_of_displacements = 
       (MPI_Aint *) malloc(n * sizeof(MPI_Aint))) == NULL){
    free(array_of_blocklenghts);
    (void) fprintf(stderr, "Error malloc in mpi_send\n");
    (void) fflush(stderr);
    return MPI_ERR_OTHER;
  }
  if ((array_of_types = 
       (MPI_Datatype *) malloc(n * sizeof(MPI_Datatype))) == NULL){
    free(array_of_blocklenghts);
    free(array_of_displacements);
    (void) fprintf(stderr, "Error malloc in mpi_send\n");
    (void) fflush(stderr);
    return MPI_ERR_OTHER;
  }

  for (i = 0, nb_blocks = 0; i < n; i+=2) {
    if (pack[i] > 0){		/* Have to pack some int */
      array_of_displacements[nb_blocks] = displacement;
      array_of_blocklenghts[nb_blocks] = pack[i];
      array_of_types[nb_blocks] = MPI_INT;
      nb_blocks++;
      displacement  += (((pack[i]-1)/2 + 1) * sizeof(double));
    }
    if (pack[i + 1] > 0){		/* Have to pack some double */
      array_of_displacements[nb_blocks] = displacement;
      array_of_blocklenghts[nb_blocks] = pack[i+1];
      array_of_types[nb_blocks] = MPI_DOUBLE;
      nb_blocks++;
      displacement += (pack[i+1] * sizeof(double));
    }
  }
#ifdef DEBUG
  if (debuglevel >= 10) {
    int j;
    (void) fprintf(stderr, "build_buff_type: %d|", displacement);	
    for (j = 0; j < nb_blocks; ++j) {
      (void) fprintf(stderr, "%d:%ld|", 
		     array_of_blocklenghts[j],
		     array_of_displacements[j]);
    }
    (void) fprintf(stderr, "\n");
    (void) fflush (stderr);	
  }	
#endif /* DEBUG */

  errcode = MPI_Type_struct(nb_blocks, 
			    array_of_blocklenghts,
			    array_of_displacements,
			    array_of_types,
			    buff_type);
  *size = displacement;		/* taille en byte */
  free(array_of_blocklenghts);  
  free(array_of_displacements); 
  free(array_of_types);   
  return errcode;
} /* build_buff_type */
