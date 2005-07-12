#ifndef SCI_SOX 
#define SCI_SOX 

#include "wav.h" 

/* Allan CORNET 18/01/2004 */
int C2F(playsound) __PARAMS((char *filename,char *command,unsigned long fname_len));
int C2F(loadwave) __PARAMS((char *filename, double *res, integer *size_res, integer flag,WavInfo *, integer *ierr));
int C2F(savewave) __PARAMS((char *filename, double *res, integer *rate, integer *size_res,integer *nchannels, integer *ierr));

void C2F(mopen) __PARAMS((int *fd, char *file, char *status, int *f_swap, double *res, int *error));
void C2F(mclose) __PARAMS((integer *fd, double *res));
void C2F(meof) __PARAMS((integer *fd, double *res));
void C2F(merror) __PARAMS((integer *fd, double *res));
void C2F(mclearerr) __PARAMS((integer *fd));
void C2F(mseek) __PARAMS((integer *fd, integer *offset, char *flag, integer *err));
void C2F(mtell) __PARAMS((integer *fd, double *offset, integer *err));
void C2F(mput) __PARAMS((integer *fd, double *res, integer *n, char *type, integer *ierr));
void C2F(mget) __PARAMS((integer *fd, double *res, integer *n, char *type, integer *ierr));
void C2F(mgetstr) __PARAMS((integer *fd, char **start, integer *n, integer *ierr));
void C2F(mgetstr1) __PARAMS((integer *fd, char *start, integer *n, integer *ierr));
void C2F(mputstr) __PARAMS((int *fd, char *str, double *res, int *ierr));


#endif
