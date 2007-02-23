      subroutine mexfunction(nlhs,plhs,nrhs,prhs)
c     simple example of fortran mex.
c     usage a=pipo3(2)
      integer*4 plhs(*), prhs(*)
c     FOR 64 bits MACHINES SHOULD BE integer*8
      integer nlhs, nrhs
c
      if (nrhs.ne.1) then
         call mexerrmsgtxt('Requires ONE input')
      endif
      if (nlhs.ne.1) then
         call mexerrmsgtxt('Requires ONE output!')
      endif
      plhs(1)=prhs(1)
      return
      end
