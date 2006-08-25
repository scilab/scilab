      subroutine getdimfromvar(k,num,n)
      integer k,num,n
c     Copyright INRIA
c     -------------------------------
c			INRIA
c     -------------------------------
      INCLUDE 'stack.h'
      integer iadr,sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      
      if(istk(il).eq.1) then
         if(istk(il+3).ne.0) then
            err=num
            call  error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=num
            call error(89)
            return
         endif
         n=max(int(stk(sadr(il+4))),0)
      elseif(istk(il).eq.8) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=num
            call error(89)
            return
         endif
         call tpconv(istk(il+3),4,1,istk(il+4),1,n,1)
         n=max(n,0)
      else
         err=num
         call  error(53)
         return
      endif
c     
      end
c     -------------------------------
      