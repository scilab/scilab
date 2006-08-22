      subroutine fgetarg(n,str)
c     Copyright INRIA
      character*(*) str
      integer n
      call getarg(n,str)
      write(06,*) n,str
      return
      end
