      subroutine intnotify
c     Copyright INRIA
      include 'stack.h'
      logical checkrhs,checklhs,getscalar
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('notify',1,1)) return
      if(.not.checkrhs('notify',1,1)) return

      if(.not.getscalar('notify',top,top,l)) return
      ntfy=stk(l)
      call objvide('notify',top)
      return
      end