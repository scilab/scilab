/* Library to add C-Functions to Scilab And/Or Matlab */
/* Bertrand Guiheneuf 1996 */

#ifndef INCLUDE_CSCIINTERF
#define INCLUDE_CSCIINTERF

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a,b) (a < b ? a : b)
#endif

#include "../machine.h"
#include "../stack-def.h"

#ifndef integer
#define integer int
#endif

/* types definition */ 
typedef double Matrix; 

struct {
  int NbParamIn;
  int NbParamOut;
  int ReturnCounter;
  int FuncIndex;
  int ReturnIndex;
  char Retour[1000];
  int Err1;
  Matrix **Param;
  Matrix **Return;
} Interf;

/****************************************** P R O T O T Y P E S *************************************/


void InterfError __PARAMS((char *mesg));
extern int MatrixMemSize __PARAMS((Matrix *m));
extern Matrix *MatrixCreate __PARAMS((int h, int w, char *type));
extern int MatrixIsNumeric __PARAMS((Matrix *m));
extern int MatrixIsReal __PARAMS((Matrix *m));
extern int MatrixIsComplex __PARAMS((Matrix *m));
extern int MatrixIsScalar __PARAMS((Matrix *m));
extern int MatrixIsList __PARAMS((Matrix *m));
extern Matrix *MatrixCreateString __PARAMS((char *s));
extern char *MatrixReadString __PARAMS((Matrix *m));
extern Matrix *ListCreate __PARAMS((void));
extern Matrix *AppendList __PARAMS((Matrix *list, Matrix *object));
extern int ListGetSize __PARAMS((Matrix *l));
extern Matrix *ListGetCell __PARAMS((int n, Matrix *l));
extern void MatrixFree __PARAMS((Matrix *m));
extern double *MatrixGetPr __PARAMS((Matrix *m));
extern double *MatrixGetPi __PARAMS((Matrix *m));
extern int MatrixGetWidth __PARAMS((Matrix *m));
extern int MatrixGetHeight __PARAMS((Matrix *m));
extern double MatrixGetScalar __PARAMS((Matrix *m));
extern void MatrixTranspose __PARAMS((Matrix *m));
extern void MatrixCopy __PARAMS((Matrix *m_source, Matrix *m_dest));
extern void ReturnParam __PARAMS((Matrix *m));
extern void InterfInit __PARAMS((void));
extern void InterfDone __PARAMS((void));
extern int MatrixIsString __PARAMS((Matrix *m));
extern void InterfError   __PARAMS((    char *mesg));
#endif /*INCLUDE_CSCIINTERF*/ 
