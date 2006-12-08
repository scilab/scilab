
      
      subroutine intnewfun 
c     Copyright INRIA
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getscalar,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('newfun',1,1)) return
      if(.not.checkrhs('newfun',2,2)) return

      if(.not.getscalar('newfun',topk,top,l)) return
      fptr=int(stk(l))
c     
      top=top-1
      if(.not.getsmat('newfun',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('newfun',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
c     
      call funtab(id,fptr,3,'NULL_NAME',0)
      if(err.gt.0) return
      call objvide('newfun',top)
      return
      end
