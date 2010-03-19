#include <cstdlib>
#include <algorithm>
extern "C" {
#include "stack-c.h"
#include "stack-def.h" /* C2F(basbrk) */
#include "intmacr2tree.h" /*#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz) */
int C2F(eqid)(int const*x, int const*y);
void C2F(siflibs)(int* id, int* k_ptr, int* istr, int* lbibn, int* nbibn, int* ilp, int* nn, int* should_return);
void C2F(sivars)(int* id, int* should_return);
void C2F(namstr)(int* id, int* str, int* n, char const* job);
}

namespace {
  char const f_true= 1;
  char const f_false=0;
  int const percent= 56;
  int const nclas= 29;
  char const from_id=1;

  int upper_char(int id){ return std::abs((int)((id & 0x80) ? (id |0xffffff00) :  (id & 0xff))); }

  int id_char(int const* id){
    int ch(upper_char(*id));
    if(ch == percent){
      ch= upper_char((*id)>>8);
    }
    return ch;
  }
}

void C2F(siflibs)(int* id, int* k_ptr, int* istr, int* lbibn_ptr, int* nbibn_ptr, int* ilp_ptr, int* nn_ptr, int* should_return){
  static int const* const lstk_ptr = (int*)C2F(vstk).lstk-1;
  static int const* const istk_ptr = ((int*)C2F(stack).Stk)-1;
  *should_return= f_false;
  int k, ilp, nbibn, lbibn;
  for(k= Bot-1; k < C2F(vstk).isiz; ++k){
    int il=iadr(lstk_ptr[k]);
    int ip;
    if(istk_ptr[il] == sci_lib){
      nbibn=istk_ptr[il+1];
      lbibn=il+2;
      il+= nbibn+2;
      ilp=il+1;
      ip=std::max(1, id_char(id)-9);
      if(ip <= nclas){
	int n=istk_ptr[ilp+ip] -istk_ptr[ilp+ip-1];
	if(n != 0){
	  int iln= ilp+nclas+1+(istk_ptr[ilp+ip-1]-1)*nsiz;
	  for(int i= 1; i<=n; ++i, iln+= nsiz){
	    if(C2F(eqid)(id, istk_ptr+iln)){ // 39
		if((Fin == -1) || (Fin == -3)){
		  C2F(com).fun= k;
		  Fin= i;
		  *should_return= f_true;
		  return;
		}
		Fin= i;
		if(C2F(errgst).err1 != 0){
		  C2F(com).fun= Fin= 0;
		  *should_return= f_true;
		  return;
		}
		 C2F(namstr)(id,istr, nn_ptr, &from_id);
		 *k_ptr= k;
		 *lbibn_ptr= lbibn;
		 *nbibn_ptr= nbibn;
		 *ilp_ptr= ilp;
		 *should_return= f_false;
		return;
	      }
	  }
	  }

      }

    }

  }
  C2F(com).fun= Fin= 0;
  *should_return= f_true;
  return;
}


void C2F(sivars)(int* id, int* should_return){
  int* const lstk_ptr = (int*)C2F(vstk).lstk-1;

  int k;
  //  idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
  int* id_addr=C2F(vstk).idstk;
  for(  *should_return= f_false, k= Bot-1, id_addr+=(k-1)*nsiz
	  ; k <=  C2F(vstk).isiz && !C2F(eqid)(id_addr, id); ++k, id_addr+=nsiz){
  }
  if( k <=  C2F(vstk).isiz ){// eq_id
    int il=lstk_ptr[k];//iadr()
    il<<=1;
    ++il;
    if((*istk(il)!=sci_u_function) &&(*istk(il)!=sci_c_function)) {
      C2F(com).fun= Fin= 0;
      *should_return= f_true;
    }
    C2F(com).fun= -1;
    Fin= k;
    *should_return= f_true;
  }
  return; //    *should_return= f_false;
}
