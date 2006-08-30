c			================================================
c     Copyright INRIA
c			================================================

      subroutine matchsgn
c     
c     matrix change sign
c     
c     Copyright INRIA
      include '../stack.h'

      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
c     .  unary minus
      if(mn1.gt.0) then
         call dscal(mn1*(it1+1),-1.0d+0,stk(l1),1)
      endif
      return
      end
c			================================================
