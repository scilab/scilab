c =========================================
c INRIA
c =========================================  
      subroutine intstr2code(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1


      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     checking variable str (number 1)
c       
      il1 = iadr(lstk(top-rhs+1))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if (istk(il1) .ne. 10) then
         fun=-1
         call funnam(ids(1,pt+1),'str2code',il1)
         return
      endif

      if (istk(il1+1)*istk(il1+2) .ne. 1) then
         err = 1
         call error(89)
         return
      endif
      n1 = istk(il1+5)-1
      l1 = il1+6
      if(.not.ref) then
         call icopy(n1,istk(l1),1,istk(l1-2),1)
         l1=sadr(ilr+4)
         call int2db(n1,istk(il1+4),-1,stk(l1),-1)
      else
         l1=sadr(ilr+4)
         call int2db(n1,istk(il1+6),1,stk(l1),1)
      endif
      istk(ilr)=1
      istk(ilr+1)=n1
      istk(ilr+2)=1
      istk(ilr+3)=0
      lstk(top+1)=l1+n1
      goto 999
 999  return
      end
c =========================================