      subroutine intgetio(fname)
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer mode(2)
      integer iadr,sadr
      logical checkrhs,checklhs,getscalar,cremat
      logical opened
      character*(*) fname
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c           
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
      top=top+1
      if(.not.cremat(fname,top,0,1,4, lr, lc)) return
      stk(lr)=rio
      stk(lr+1)=rte
      stk(lr+2)=wio
      stk(lr+3)=wte
      
      return
      end
