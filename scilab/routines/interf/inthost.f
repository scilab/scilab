      subroutine inthost
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

      if(rhs.ne.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif
c     
      il=iadr(lstk(top))
      if(istk(il).ne.10) then
         err=1
         call error(55)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=1
         call error(89)
         return
      endif

      ilf=il+6
      lc=istk(il+5)-istk(il+4)
      if (lc.gt.0) call cvstr(lc,istk(ilf),buf,1)
      call bashos(buf,lc,ls,ierr)
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=ierr
      lstk(top+1)=l+1

      return
      end
