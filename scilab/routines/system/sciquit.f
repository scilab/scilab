      subroutine sciquit
c!Purpose
c     execute scilab.quit file, quit parser remove temporary files, 
c     clean ieeeflags,deallocate memory
c!Syntax
c     call sciquit
c!      
c     Copyright INRIA
      include '../stack.h'
      character*256 path
      integer nos1,vsiz1,quitf
      common /comnos/ nos1,vsiz1
      if(nos1.eq.0) then
         path = ' '
         call inffic( 6, path,nc)
         if (nc.gt.0) call scirun(path(1:nc))
      endif
c     cleaning ieeefalgs and quit 
      quitf=0
      call clearexit(quitf)
      return
      end
