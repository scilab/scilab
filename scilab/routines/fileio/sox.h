#ifndef SCI_SOX 
#define SCI_SOX 

#include "wav.h" 

/* Allan CORNET 18/01/2004 */
int C2F(playsound) __PARAMS((char *filename,char *command,unsigned long fname_len));
int C2F(loadwave) __PARAMS((char *filename, double *res, integer *size_res, integer flag,WavInfo *, integer *ierr));
int C2F(savewave) __PARAMS((char *filename, double *res, integer *rate, integer *size_res,integer *nchannels, integer *ierr));
#endif
