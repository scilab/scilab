c     The main program is written in C for gc-win32
c     this routine is mainly used for transfer to Fortran 
c     from C main 
c     ---------------------------------------------
c     Copyright INRIA
      subroutine mainsci(nos1,path1,lpath)
      integer nos,now,idisp,mem,nos1,lpath
      character*256 path,path1
      common /comnos/nos,mem
      common /starfi/path
      data now,idisp /0,0/
      nos=nos1
      mem=0
      idisp=0
      now=0
      path = ' '
      if ( lpath.ge.1 ) then 
         path(1:lpath) = path1
      endif
      mem=max(mem,180000)
      if (nos.eq.0) call settmpdir()
      call scilab(nos)
      end
