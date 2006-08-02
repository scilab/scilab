c     ********************
c     Copyright INRIA
c     ********************
      subroutine intvar2vec
      
      include 'stack.h'
      integer iadr,sadr
      
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
      il=iadr(lstk(top))
      il1=il
      l1=sadr(il1+4)
      n=lstk(top+1)-lstk(top)
      call unsfdcopy(n,stk(lstk(top)),-1,stk(l1),-1)
      istk(il1)=1
      istk(il1+1)=n
      istk(il1+2)=1
      istk(il1+3)=0
      lstk(top+1)=l1+n
      
      end
c     ********************
