
#ifndef INCLUDE_GLOBAL_H
#define INCLUDE_GLOBAL_H
/* the variables names */
#include "../stack-c.h"
#include "C-LAB_Interf.h"


int setGvar __PARAMS(( Matrix *Mname, double *sciObj));
/* set a global variable */
/* if it doesn't exist, its created */
/* INPUT  : -name, scilab var name */
/*          -sciObj, scilab object to store */
/* OUTPUT : result of the operation. */
/*          1  -> the variable replaced an old value */
/*          0  -> the variable has been created and stored */
/*          <0 -> the operation has failed */

int getGvar  __PARAMS(( Matrix *Mname, double **ptrObj));
/* get a global variable */
/* INPUT  : -name, scilab var name */
/*          -sciObj, scilab object to store */
/* OUTPUT : result of the operation. */
/*         -1  -> the variable didn't exist */
/*              in this case, NULL is returned in ptrObj */
/*          0  -> the variable has been read */

int delGvar  __PARAMS(( Matrix *Mname ));
/* delete a global var */
/* the variable memory is freed and its entry */
/* in the global list is supressed */
/* INPUT  : name, the name of the scilab var */
/* OUTPUT : 0 is the operation was successful, -1 otherwise */


#endif /* INCLUDE_GLOBAL_H */
