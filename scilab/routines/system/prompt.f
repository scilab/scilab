      subroutine prompt(pause)
c ====================================================================
c     issue prompt with optional pause
c ================================== ( Inria    ) =============
c     Copyright INRIA
      include '../stack.h'
      integer pause
      if (pause .ne. 1) then
         call basout(io,wte,' ')
         call setprlev(paus)
      else
C     version with pause ( mode(7) )
         call setprlev(-1)
         call basin(ierr,rte,buf,'*')
      endif
      return
      end
