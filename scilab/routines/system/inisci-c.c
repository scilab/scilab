#include <string.h>
#include "../machine.h" 
#include "../sun/Sun.h" 

/*-------------------------------------------
 *  get configure options used for compilation 
 *  used in inisci.f
 *  Copyright ENPC/ Jean-Philipe Chancelier 
 *-------------------------------------------*/

int C2F(withtk)(int *rep) { 
#ifdef WITH_TK
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}

int C2F(withpvm)(int *rep) { 
#ifdef WITH_PVM
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}

int C2F(withgtk)(int *rep) { 
#ifdef WITH_GTK
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}

int C2F(withocaml)(int *rep) { 
#ifdef WITH_OCAML
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}

int C2F(getcomp)(char *buf,int *nbuf,long int lbuf)
{
  int ierr,iflag=0,l1buf=lbuf;
  C2F(getenvc)(&ierr,"COMPILER",buf,&l1buf,&iflag);
  if ( ierr== 1) strncpy(buf,"NO",lbuf);
  *nbuf = strlen(buf);
  return 0;
}

