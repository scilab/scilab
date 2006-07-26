      subroutine inttype
c     Copyright INRIA
      include 'stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr
c    
      iadr(l)=l+l-1
c
      if(.not.checklhs('type',1,1)) return
      if(.not.checkrhs('type',1,1)) return


      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      k=istk(il)
      if(.not.cremat('type',top,0,1,1,l,lc)) return
      stk(l)=dble(k)
      return
      end