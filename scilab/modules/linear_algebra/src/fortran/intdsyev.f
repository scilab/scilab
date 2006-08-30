      subroutine intdsyev(fname)

c     [D,V]=spec(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBZ, UPLO
      double precision ZERO
      parameter ( ZERO=0.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lV)) return
            lhsvar(1) = 2
            lhsvar(2) = 1
            return
         endif
      endif
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      if(lhs.eq.1) then
         if(.not.createvar(2,'d', N, 1, lD)) return              
      else
         if(.not.createvar(2,'d', N, N, lD)) return
      endif
      if(.not.createvar(3,'d', N, 1, lW)) return
      LWORKMIN = 3*N - 1
      LWORK=maxvol(4,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(4,'d',1,LWORK,lDWORK)) return
      if(lhs.eq.1) then
         JOBZ = 'N'
      else
         JOBZ = 'V'
      endif 
      UPLO = 'U'
      call DSYEV( JOBZ, UPLO, N, stk(lA), N, stk(lW),
     $     stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.eq.1) then
         do 10 i = 1, N
            stk(lD+i-1) = stk(lW+i-1)
 10      continue   
         lhsvar(1)=2  
      else 
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lD), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 20 i = 1, N
            ii = i+(i-1)*N
            stk(lD+ii-1) = stk(lW+i-1)
 20      continue   
         lhsvar(1)=1
         lhsvar(2)=2

      endif 
c     
      end

