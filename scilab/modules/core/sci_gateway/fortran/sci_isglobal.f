      subroutine intisglobal(fname)
c     Copyright INRIA
      include 'stack.h'
      logical checkrhs,checklhs,crebmat
      character*(*) fname
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(.not.checklhs('isglobal',1,1)) return
      if(.not.checkrhs('isglobal',1,1)) return

      if(.not.crebmat(fname,top,1,1,ilr)) return
      if(infstk(top).eq.2) then
         istk(ilr)=1
      else
         istk(ilr)=0
      endif
      return
      end
