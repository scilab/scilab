      subroutine interrclear
c     Copyright INRIA
      include 'stack.h'
      logical checkrhs,checklhs
c
      rhs=max(0,rhs)
      if(.not.checklhs('errclear',1,1)) return
      if(.not.checkrhs('errclear',0,1)) return

      err2=0
      if(rhs.eq.1) top=top-1
      top=top+1
      call objvide('errclear',top)
      return
      end
