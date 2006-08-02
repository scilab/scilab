c     ********************
c     Copyright INRIA
c     ********************
      subroutine intcurblk
c
c
      include 'stack.h'
      integer kfun
      common /curblk/ kfun
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=kfun
      lstk(top+1)=l+1
      return
      end
c     ********************
