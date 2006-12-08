
      subroutine intfunptr
c     Copyright INRIA
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('funptr',1,1)) return
      if(.not.checkrhs('funptr',1,1)) return

      if(.not.getsmat('funptr',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('funptr',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call funtab(id,fptr,1,'NULL_NAME',0)
      if(.not.cremat('funptr',top,0,1,1,l,lc)) return
      stk(l)=dble(fptr)
      return
      end

 