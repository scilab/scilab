      subroutine intclearfun
c     Copyright INRIA
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('clearfun',1,1)) return
      if(.not.checkrhs('clearfun',1,1)) return

      if(.not.getsmat('clearfun',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('clearfun',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call funtab(id,fptr,4)
      call objvide('clearfun',top)
      return
      end
      