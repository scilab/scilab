#include "../machine.h" 

/*-------------------------------------------
 *  get configure options used for compilation 
 *  used in inisci.f
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

