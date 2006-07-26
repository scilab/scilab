      subroutine inthavewindow
c     Copyright INRIA
      include 'stack.h'
      logical checkrhs,checklhs,crebmat
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('havewindow',1,1)) return
      if(.not.checkrhs('havewindow',0,0)) return

      call xscion(iflag)

      top=top+1
      if(.not.crebmat('havewindow',top,1,1,l)) return
      istk(l)=iflag
      return
      end
