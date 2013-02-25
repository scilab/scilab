/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifdef _MSC_VER
#include <Windows.h>
#endif

#include <cstdlib>
#include <algorithm>
#include "unrolled_algorithms.hxx"
extern "C" {
#include "stack-c.h"
#include "stack-def.h" /* C2F(basbrk) */
#include "intmacr2tree.h" /*#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz) */

    void C2F(siflibs)(int* id, int* k_ptr, int* istr, int* lbibn, int* nbibn,
        int* ilp, int* nn, int* should_return);
    void C2F(sivars)(int* id, int* should_return);
    void C2F(namstr)(int* id, int* str, int* n, int const* job);
}


/*
 * These are C++ reimplementations of some (as few as possible) code from funs.f
 * The goal was to improve speed with :
 * 1°) avoiding a call to namstr_ when only the first (two) caracters where needed
 * 2°) allowing the compiler to replace eqid() calls with inlined faster C++ implementation of eq_n<nsiz>
 */

namespace
{
    char const f_true = 1;
    char const f_false = 0;
    int const percent = 56;
    int const nclas = 29;
    int const from_id = 1;

    /* directly convert from id to upper char */
    int upper_char(int id)
    {
        /* scilab character encoding is using signed bytes packed into an int.
         * testing for the 7th bit with & 0x80 gives us the sign
         * & 0xff extracts the byte if positive,
         * |0xffffff00 extracts the byte if negative (assuming two's complement negative numbers representations)
         * abs() takes the upper value in scilab character encoding.
         */
        return std::abs((int)((id & 0x80) ? (id |0xffffff00) :  (id & 0xff)));
    }

    /* gives the discriminating char (either first of second if first=percent) */
    int id_char(int const* id)
    {
        int ch(upper_char(*id));
        if (ch == percent)
        {
            return upper_char((*id)>>8);
        }
        return ch;
    }
}
/* search for an id in the libraries
 * as we reimplement part of Fortran function, we now use a 'bool' (for Fortran) should_return to tell calling
 * function that it should return at once.
 * other variables are lifted straight from Fortran code.
 */
void C2F(siflibs)(int* id, int* k_ptr, int* istr, int* lbibn_ptr, int* nbibn_ptr,
                  int* ilp_ptr, int* nn_ptr, int* should_return)
{

    int const* const lstk_ptr = (int*)C2F(vstk).lstk-1;
    int const* const istk_ptr = ((int*)C2F(stack).Stk)-1;
    int k, ilp, nbibn, lbibn;

    *should_return= f_false;

    for (k= Bot; k < C2F(vstk).isiz; ++k)
    {
        int il = iadr(lstk_ptr[k]);
        int ip;

        if (istk_ptr[il] == sci_lib)
        {
            nbibn = istk_ptr[il+1];
            lbibn = il+2;
            il += nbibn+2;
            ilp = il+1;
#ifdef _MSC_VER
            ip = min(nclas,max(1, id_char(id)-9));
#else
            ip =  std::min(nclas,std::max(1, id_char(id)-9));
#endif
            if (ip <= nclas)
            {

                int n = istk_ptr[ilp+ip] -istk_ptr[ilp+ip-1];
                if (n != 0)
                {
                    int iln= ilp+nclas+1+(istk_ptr[ilp+ip-1]-1)*nsiz;

                    for (int i = 1; i<=n; ++i, iln+= nsiz)
                    {
                        if (eq_n<nsiz>(id, istk_ptr+iln))
                        { /* 39 */
                            if ((Fin == -1) || (Fin == -3))
                            {
                                C2F(com).fun= k;
                                Fin= i;
                                *should_return= f_true;
                                return;
                            }

                            Fin= i;
                            if (C2F(errgst).err1 != 0)
                            {
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

/* search for an id in vars, also lifted from Fortran code in funs.f 
 30   k=bot-1
 31   k=k+1
      if(k.gt.isiz) goto 35
      if(.not.eqid(idstk(1,k),id)) goto 31
      il=iadr(lstk(k))
c     modif 1.3 SS
      if(istk(il).ne.11.and.istk(il).ne.13) then
         fin=0
         fun=0
         return
      endif
      fin=k
      fun=-1
      return
*/
void C2F(sivars)(int* id, int* should_return)
{
    int* const lstk_ptr = (int*)C2F(vstk).lstk-1;

    int k;
    /*  idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz) */
    int* id_addr=C2F(vstk).idstk;
    for (*should_return= f_false, k= Bot, id_addr+=(k-1)*nsiz;
         k <=  C2F(vstk).isiz && !eq_n<nsiz>(id_addr, id);
         ++k, id_addr+=nsiz)
    {
    }

    if (k <=  C2F(vstk).isiz)
    {/* eq_id */
        int il=lstk_ptr[k];
        il=il+il-1;/* iadr() */
        if ((*istk(il) != sci_u_function) && (*istk(il) != sci_c_function))
        {
            C2F(com).fun= Fin= 0;
        }
        else
        {
          C2F(com).fun= -1;
          Fin= k;
        }
        *should_return= f_true;
    }
    return; 
}
