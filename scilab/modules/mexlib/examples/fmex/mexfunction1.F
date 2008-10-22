      subroutine mexfunction(nlhs, plhs, nrhs, prhs)
c     [a,b]=mexf1(1,2)     see builder.sce file

      integer*4 plhs(*), prhs(*)
C     Uncomment for 64 bits Dec Alpha machines
C      integer*8 plhs(*), prhs(*)
C
      integer nlhs, nrhs
c
      if (nrhs.ne.2) then
         call mexerrmsgtxt('Two inputs needed')
      endif

      if (nlhs.gt.2) then
         call mexerrmsgtxt('Two many outputs')
      endif

      plhs(1)=prhs(1)
      plhs(2)=prhs(2)

      return 
      end

