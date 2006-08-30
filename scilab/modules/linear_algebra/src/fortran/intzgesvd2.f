
      subroutine intzgesvd2(fname)

c     [U,S,V]=svd(A)
c     s = svd(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite
      
      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )
      intrinsic conjg

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         endif
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(vfinite(M*N*2,zstk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'z', M, min(M,N), lU)) return
         if(.not.createvar(4,'d', min(M,N), min(M,N), lS)) return
         if(.not.createvar(5,'z', N, min(M,N), lV)) return
         if(.not.createvar(6,'z', min(M,N), N, lVT)) return
         k = 7
      endif
      LRWRK = 5*min(M,N)
      if(.not.createvar(k,'d',1,LRWRK,lRWORK)) return
      LWORKMIN = 2*min(M,N)+max(M,N)
      LWORK=maxvol(k+1,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
         JOBU = 'S'
         JOBVT = 'S'
      endif
      if(lhs.eq.1) then
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lA)
     $        ,M, zstk(lA), N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
C     Next lines introduced to patch an error of  ZGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lU)
     $        ,M, zstk(lVT), min(M,N), zstk(lDWORK), LWORK, stk(lRWORK)
     $        ,INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      endif 
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

      if(lhs.gt.1) then
         call DLASET( 'F', min(M,N), min(M,N), ZERO, ZERO, stk(lS),
     $        min(M,N) )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*min(M,N)
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, min(M,N)
            do 20 i = 1, N
               ij = i+(j-1)*N
               ji = j+(i-1)*min(M,N)
               zstk(lV+ij-1) = conjg(zstk(lVT+ji-1))
 20         continue
 30      continue     
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 2
      else 
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
      endif
c     
      end
