 
      subroutine intzgeqpf4(fname)

c     [Q,R]=qr(A,'e')
c     [Q,R,E]=qr(A,'e')

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ONE, DZERO
      parameter ( ONE=1.0D0, DZERO=0.0D0)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )


      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(2,'z', 0, 0, lQ)) return
         if(.not.createvar(3,'z', 0, 0, lR)) return
         if(lhs.gt.2) then
            if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1)=2
         lhsvar(2)=3
         if(lhs.gt.2) then
            lhsvar(3)=4
         endif
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif      
      if(.not.createvar(2,'z', M, min(M,N), lQ)) return
      if(.not.createvar(3,'z', min(M,N), N, lR)) return
      k=4
      if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
      endif
      if(.not.createvar(k,'z', 1, min(M,N), lTAU)) return
      if(.not.createvar(k+1,'d', 1, 2*N, lRWORK)) return
      LWORKMIN = N
      LWORK=maxvol(k+2,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'z', 1, LWORK, lDWORK)) return


      if(lhs.le.2) then
         call ZGEQRF( M, N, zstk(lA), M, zstk(lTAU), zstk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE ZGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )      
      else
         do 5 i = 1, N
            istk(lJPVT+i-1) = 0 
 5       continue
         call ZGEQPF( M, N, zstk(lA), M, istk(lJPVT), zstk(lTAU),
     $        zstk(lDWORK), stk(lRWORK), INFO )
c     SUBROUTINE ZGEQPF( M, N, A, LDA, JPVT, TAU, WORK, RWORK, INFO )
      endif
      if(info.ne.0) then
         return
      endif
      
      call ZLACPY( 'U', min(M,N), N, zstk(lA), M, zstk(lR), min(M,N)
     $     )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         do 20 j = 1, N-1
            if(j+1.le.min(M,N)) then
               do 10 i = j+1, min(M,N)
                  ij = i+(j-1)*min(M,N)
                  zstk(lR+ij-1) = ZERO
 10            continue
            endif
 20      continue
      endif

      call ZLACPY( 'F', M, min(M,N), zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      call ZUNGQR( M, min(M,N), min(M,N), zstk(lQ), M, zstk(lTAU),
     $     zstk(lDWORK), LWORK, INFO )
c     SUBROUTINE ZUNGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )

      if(lhs.gt.2) then
         call DLASET( 'F', N, N, DZERO, DZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 30 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 30      continue    
      endif

      if(lhs.le.2) then
         lhsvar(1)=2
         lhsvar(2)=3
      else
         lhsvar(1)=2
         lhsvar(2)=3
         lhsvar(3)=4
      endif
c     
      end
      