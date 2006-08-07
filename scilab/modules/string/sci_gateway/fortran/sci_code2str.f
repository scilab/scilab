c =========================================
c INRIA
c =========================================  
      subroutine intcode2str(id)
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
      if (istk(il1) .ne. 1) then
         fun=-1
         call funnam(ids(1,pt+1),'code2str2',il1)
         return
      endif
      ref=ilr.ne.il1
      if (istk(il1+3) .ne. 0) then
         err = 1
         call error(52)
         return
      endif
      n1 = istk(il1+1)*istk(il1+2)
      l1 = sadr(il1+4)
      if(.not.ref) then
         call unsfdcopy(n1,stk(l1),-1,stk(l1+2),-1)
         l1=l1+2
      endif
      istk(ilr+4)=1
      istk(ilr+5)=n1+1
      do 81 i=1,n1
         istk(ilr+5+i)=stk(l1-1+i)
 81   continue
      istk(ilr)=10
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      lstk(top+1)=sadr(ilr+6+n1)
      goto 999
 999  return
      end
c =========================================  