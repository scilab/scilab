      subroutine  msgstxt(txt)
c     Copyright INRIA
      include '../stack.h'
      character txt*(*)
      call basout(io,wte,txt)
      return
      end
