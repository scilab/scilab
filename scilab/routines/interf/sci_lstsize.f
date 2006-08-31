      subroutine intlstsize
c     WARNING : argument of this interface may be passed by reference
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).lt.15.or.istk(il).gt.17) then
         err=1
         call error(56)
         return
      endif
      n=istk(il+1)

      if(lhs*rhs.ne.1) then
         err=1
         call error(39)
         return
      endif
      ilr=iadr(lstk(top))
      l=sadr(ilr+4)
      err=l+1-lstk(bot)
      if (err.gt.0) then
         call error(17)
         return
      endif
      istk(ilr)=1
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      l=sadr(ilr+4)
      stk(l)=dble(n)
      lstk(top+1)=l+1
c
      return
      end
