      subroutine scitod(x,mx,nx)
C    scilab to fortran
C
c     Copyright INRIA
      double precision  x(*)
      integer  mx
      integer  nx
C.. Local Scalars .. 
      integer  il,l,lx
C.. External Calls .. 
      external dcopy, error
C.. Include Statements ..
      include "../stack.h"
C.. Statement Functions ..
      integer  iadr
      integer  sadr
c
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
C
      if(mx*nx.eq.0) then
         top=top-1
         return
      endif
      il = iadr(lstk(top))
      if (istk(il+1)*istk(il+2).eq.0) then
         top=top-1
         return
      endif
      if (istk(il) .ne. 1) then
        call error(98)
      elseif (istk(il+1).ne.mx.or.istk(il+2).ne.nx.or.istk(il+3).ne.0)
     &then
        call error(98)
      else
        lx = sadr(il+4)
        call dcopy(nx*mx,stk(lx),1,x,1)
        top = top - 1
      endif
      end 
