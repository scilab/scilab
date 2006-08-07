c =========================================
c INRIA
c =========================================  
      subroutine intlength(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs*rhs.ne.1) then
         call error(39)
         return
      endif

      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      itype=istk(il1)
      if(itype.eq.1.or.itype.eq.2.or.itype.eq.4) then
c     length( )=prod(size( )) for matrices (+ polynomial and boolean)
         l=sadr(ilr+4)
         stk(l)=dble(istk(il1+1)*istk(il1+2))
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         lstk(top+1)=l+1
      elseif(itype.ge.15.and.itype.le.17) then
c     length(list)=size(list)
         l=sadr(ilr+4)
         stk(l)=dble(istk(il1+1))
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         lstk(top+1)=l+1
      elseif(itype.eq.10) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         mn1=m1*n1
         id1=il1+4
         l1=id1+mn1+1
c
         if(ref) then
            ll=sadr(ilr+4)
         else
            ll=sadr(l1)
         endif
         err=ll+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c
         do 11 k=1,mn1
            stk(ll-1+k)=dble(istk(id1+k)-istk(id1+k-1))
 11      continue
         l=sadr(ilr+4)
         if(.not.ref) call unsfdcopy(mn1,stk(ll),1,stk(l),1)
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1
         istk(ilr+3)=0
         lstk(top+1)=l+mn1
      else
         fun=-1
         call funnam(ids(1,pt+1),'length',il1)
         return
      endif
      return
      end
c =========================================  
