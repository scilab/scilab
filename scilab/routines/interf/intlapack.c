#include "../stack-c.h"
#include <string.h>
#include <stdio.h>

#define DOUBLE 1
#define STRING  10

extern int C2F(intlapack)  __PARAMS((void));


int intqr()
{
  int *header1;int *header2;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];

  switch (Rhs) {
  case 1:   /* ...=qr(A)   */
    if (Cmplx==0) {
      ret = C2F(intdgeqpf3)("qr",2L);
      return; }
    if (Cmplx==1) {
      ret = C2F(intzgeqpf3)("qr",2L);
      return; } 
    break;
  case 2 :   /* ...=qr(A, something)   */
    header2 = (int *) GetData(2);
    switch (header2[0]) {
    case DOUBLE :
      /*  old qr, tolerance is passed: [Q,R,rk,E]=qr(A,tol)  */
      /*   ret = C2F(intqrold)("qr",2L);  */
      return;
      break;
    case STRING  :
      /* Economy size:  ...=qr(A,"e")  */
      if (Cmplx==0) {
	ret = C2F(intdgeqpf4)("qr",2L);  
	return;}
      if (Cmplx==1) {
	ret = C2F(intzgeqpf4)("qr",2L);  
	return;} 
      break;
    }
    break;
  default :   /*  rhs > 2 */
    break;
  }
}


typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} LapackTable;

 
static LapackTable Tab[]={
  {intqr,"qr"},
};

int C2F(intlapack)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}
