   		subroutine intzgesv3(fname)

c     a\b

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      integer vfinite
      external dlamch, zlange, vfinite
      intrinsic sqrt
c     
      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
      if(.not.getrhsvar(2,'z', MB, NRHS, lB)) return
      if(MB.eq.0) then
         lhsvar(1) = 2
         return
      endif
      if(MA .ne. MB) then
         call error(265)
         return
      endif
      M = MA
      N = NA
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         lhsvar(1) = 3
         return
      endif
c     Check if A and B matrices contains Inf or NaN's
      if (vfinite(M*N*2, zstk(lA)).eq.0) then
         call error(229)
         return
      endif
      if (vfinite(MB*NRHS*2, zstk(lB)).eq.0) then
         call error(229)
         return
      endif

      if(.not.createvar(3,'z', M, N, lAF)) return
      if(.not.createvar(4,'z', N, NRHS, lX)) return
      if(.not.createvar(5,'z', max(M,N), NRHS, lXB)) return
      if(.not.createvar(6,'i', 1, 1, lRANK)) return
      if(.not.createvar(7,'i', 1, N, lIPIV)) return
      if(.not.createvar(8,'i', 1, N, lJPVT)) return
      if(.not.createvar(9,'d',1,2*N,lRWORK)) return       
      LWORKMIN = max( 2*N, min(M,N)+max( 2*min(M,N), N+1,
     $     min(M,N)+NRHS ) )
      LWORK=maxvol(10,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(10,'z',1,LWORK,lDWORK)) return
      
      EPS = dlamch('eps')
      ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )
c     
      if(M.eq.N) then
c     
c     M = N
c     
         call ZLACPY( 'F', N, N, zstk(lA), N, zstk(lAF), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZLACPY( 'F', N, NRHS, zstk(lB), N, zstk(lXB), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZGETRF( N, N, zstk(lAF), N, istk(lIPIV), INFO )         
c     SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
         if(INFO.eq.0) then
            call ZGECON( '1', N, zstk(lAF), N, ANORM, RCOND,
     $           zstk(lDWORK), stk(lRWORK), INFO )
c     SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        RWORK, INFO )
            if(RCOND.gt.sqrt(EPS)) then
               call ZGETRS( 'N', N, NRHS, zstk(lAF), N, istk(lIPIV),
     $              zstk(lXB), N, INFO ) 
c     SUBROUTINE ZGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c     B, LDB, INFO )
               call ZLACPY( 'F', N, NRHS, zstk(lXB), N, zstk(lX), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
               lhsvar(1) = 4
               return
            else
c     .        ill conditioned problem
               call writebufzgesv3(buf,RCOND)
               call msgs(5,1)
            endif
         endif
      endif
c     
c     M.ne.N or A  singular
c     
      RCOND = sqrt(EPS)
      call ZLACPY( 'F', M, NRHS, zstk(lB), M, zstk(lXB), max(M,N) )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
          istk(lJPVT+i-1) = 0
 10   continue
      call ZGELSY1( M, N, NRHS, zstk(lA), M, zstk(lXB), max(M,N),
     $     istk(lJPVT), RCOND, istk(lRANK), zstk(lDWORK),
     $     LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGELSY1( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                    RANK, WORK, LWORK, RWORK, INFO )
      if(info.ne.0) then
         return
      endif
      if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call msgs(9,istk(lRANK))
      endif
      call ZLACPY( 'F', N, NRHS, zstk(lXB), max(M,N), zstk(lX), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )

      lhsvar(1)=4
      return
c     
      end

