      subroutine fgetarg(n,str)
c     Copyright INRIA
      character*(*) str
      integer n
      call getarg_(n,str)
      return
      end
