      subroutine doldqr(tol,fname)

c     [Q,R,rk,E]=qr(A,tol)
c     [Q,R,rk,E]=qr(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE,eps,tt,tol
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c     
      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=4
      eps=stk(leps)
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lQ)) return
         if(.not.createvar(3,'d', 0, 0, lR)) return
         if(.not.createvar(4,'d', 1, 1, lrk)) return
         stk(lrk)=0.0d0
         if(.not.createvar(5,'d', 0, 0, lE)) return
         lhsvar(1) = 2
         lhsvar(2) = 3
         lhsvar(3) = 4
         lhsvar(4) = 5
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'d', M, M, lQ)) return
      if(.not.createvar(3,'d', M, N, lR)) return
      if(.not.createvar(4,'d', N, N, lE)) return
      if(.not.createvar(5,'i', 1, N, lJPVT)) return
      if(.not.createvar(6,'d', 1, 1, lrk)) return
      k=7
      if(.not.createvar(k,'d', 1, min(M,N), lTAU)) return
      if(lhs.le.2) then
         LWORKMIN = MAX(1,N)
      else
         LWORKMIN = 3*N
      endif
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=LWORK-LWORKMIN
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      if(lhs.le.2) then
         call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO ) 
      else
         do 5 i = 1, N
            istk(lJPVT+i-1) = 0     
 5       continue
         call DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU),
     $        stk(lDWORK), INFO )
c     SUBROUTINE DGEQPF( M, N, A, LDA, JPVT, TAU, WORK, INFO )
      endif
      if(info.ne.0) then
         return
      endif
      call DLACPY( 'U', M, N, stk(lA), M, stk(lR), M )
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         if(M.gt.N) then
            do 20 j = 1, N
               do 10 i = j+1, M
                  ij = i+(j-1)*M
                  stk(lR+ij-1) = ZERO
 10            continue
 20         continue
         else
            do 40 j = 1, M-1
               do 30 i = j+1, M
                  ij = i+(j-1)*M
                  stk(lR+ij-1) = ZERO
 30            continue
 40         continue 
         endif
      endif
      
      if(M.gt.N) then
         call DLACPY( 'F', M, N, stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
         do 60 j = N+1, M
            do 50 i = 1, M
               ij = i+(j-1)*M
               stk(lQ+ij-1) = ZERO
 50         continue
 60      continue
      else
         call DLACPY( 'F', M, M, stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      endif
      call DORGQR( M, M, min(M,N), stk(lQ), M, stk(lTAU), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DORGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )
      
      if(lhs.gt.2) then
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
      endif
      
c     evaluating the rank
      tt = abs(stk(lR))
      if(tol.eq.-1.0d0)  tol = dble(max(m,n)) * eps * tt
      k = 0
      ls = lr
      m1 = min(m,n)
      do 450 j = 1,m1
         tt = abs(stk(ls))
         if (tt .le. tol) goto 460
         k = j
         ls = ls + m + 1
 450  continue
 460  continue

c     
      stk(lrk)=dble(k)
      lhsvar(1)=2
      lhsvar(2)=3
      lhsvar(3)=6
      lhsvar(4)=4
c     
      end

      subroutine zoldqr(tol,fname)

c     [Q,R,rk,E]=qr(A,tol)
c     [Q,R,rk,E]=qr(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ONE, DZERO,eps,tt,tol
      parameter ( ONE=1.0D0, DZERO=0.0D0)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )


      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=4
      eps=stk(leps)
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'z', 0, 0, lQ)) return
         if(.not.createvar(3,'z', 0, 0, lR)) return
         if(.not.createvar(4,'d', 1, 1, lrk)) return
         stk(lrk)=0.0d0
         if(.not.createvar(5,'d', 0, 0, lE)) return
         lhsvar(1) = 2
         lhsvar(2) = 3
         lhsvar(3) = 4
         lhsvar(4) = 5
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'z', M, M, lQ)) return
      if(.not.createvar(3,'z', M, N, lR)) return
      if(.not.createvar(4,'d', N, N, lE)) return
      if(.not.createvar(5,'i', 1, N, lJPVT)) return
      if(.not.createvar(6,'d', 1, 1, lrk)) return
      k=7
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
         call ZGEQRF(M,N,zstk(lA),M,zstk(lTAU),zstk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE ZGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )
      else
         do 5 i = 1, N
            istk(lJPVT+i-1) = 0
 5       continue
         call ZGEQPF(M,N,zstk(lA), M,istk(lJPVT),zstk(lTAU),
     $        zstk(lDWORK), stk(lRWORK), INFO )
c     SUBROUTINE ZGEQPF( M, N, A, LDA, JPVT, TAU, WORK, RWORK, INFO )
      endif
      if(info.ne.0) then
         return
      endif

      call ZLACPY( 'U', M, N, zstk(lA), M, zstk(lR), M )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         if(M.gt.N) then
            do 20 j = 1, N
               do 10 i = j+1, M
                  ij = i+(j-1)*M
                  zstk(lR+ij-1) = ZERO
 10            continue
 20         continue
         else
            do 40 j = 1, M-1
               do 30 i = j+1, M
                  ij = i+(j-1)*M
                  zstk(lR+ij-1) = ZERO
 30            continue
 40         continue
         endif 
      endif
      
      if(M.gt.N) then
         call ZLACPY( 'F', M, N, zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         do 60 j = N+1, M
            do 50 i = 1, M
               ij = i+(j-1)*M 
               zstk(lQ+ij-1) = ZERO
 50         continue
 60      continue
      else
         call ZLACPY( 'F', M, M, zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      endif
      call ZUNGQR( M, M, min(M,N), zstk(lQ), M, zstk(lTAU),
     $     zstk(lDWORK), LWORK, INFO )
c     SUBROUTINE ZUNGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )

      if(lhs.gt.2) then
         call DLASET( 'F', N, N, DZERO, DZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
      endif
c     evaluating the rank
      tt = abs(zstk(lR))
      if(tol.eq.-1.0d0)  tol = dble(max(m,n)) * eps * tt
      k = 0
      ls = lr
      m1 = min(m,n)
      do 450 j = 1,m1
         tt = abs(zstk(ls))
         if (tt .le. tol) goto 460
         k = j
         ls = ls + m + 1
 450  continue
 460  continue

c     
      stk(lrk)=dble(k)
      lhsvar(1)=2
      lhsvar(2)=3
      lhsvar(3)=6
      lhsvar(4)=4
c     
      end

      subroutine intdgeqpf3(fname)

c     [Q,R]=qr(A)
c     [Q,R,E]=qr(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lQ)) return
         if(.not.createvar(3,'d', 0, 0, lR)) return
         if(lhs.gt.2) then
            if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1) = 2
         lhsvar(2) = 3
         if(lhs.gt.2) then
            lhsvar(3) = 4
         endif
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'d', M, M, lQ)) return
      if(.not.createvar(3,'d', M, N, lR)) return
      k=4
      if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
      endif
      if(.not.createvar(k,'d', 1, min(M,N), lTAU)) return
      if(lhs.le.2) then
         LWORKMIN = MAX(1,N)
      else
         LWORKMIN = 3*N
      endif
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=LWORK-LWORKMIN
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      if(lhs.le.2) then
         call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO ) 
      else
         do 5 i = 1, N
            istk(lJPVT+i-1) = 0   
 5       continue
         call DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU),
     $        stk(lDWORK), INFO )
c     SUBROUTINE DGEQPF( M, N, A, LDA, JPVT, TAU, WORK, INFO )
      endif
      if(info.ne.0) then
         return
      endif
      call DLACPY( 'U', M, N, stk(lA), M, stk(lR), M )
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         if(M.gt.N) then
            do 20 j = 1, N
               do 10 i = j+1, M
                  ij = i+(j-1)*M
                  stk(lR+ij-1) = ZERO
 10            continue
 20         continue
         else
            do 40 j = 1, M-1
               do 30 i = j+1, M
                  ij = i+(j-1)*M
                  stk(lR+ij-1) = ZERO
 30            continue
 40         continue 
         endif
      endif
      
      if(M.gt.N) then
         call DLACPY( 'F', M, N, stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
         do 60 j = N+1, M
            do 50 i = 1, M
               ij = i+(j-1)*M
               stk(lQ+ij-1) = ZERO
 50         continue
 60      continue
      else
         call DLACPY( 'F', M, M, stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      endif
      call DORGQR( M, M, min(M,N), stk(lQ), M, stk(lTAU), stk(lDWORK)
     $     ,LWORK, INFO )
c     SUBROUTINE DORGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )
      
      if(lhs.gt.2) then
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
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




      subroutine intdgeqpf4(fname)

c     [Q,R]=qr(A,'e')
c     [Q,R,E]=qr(A,'e')

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c     
      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lQ)) return
         if(.not.createvar(3,'d', 0, 0, lR)) return
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
      if(.not.createvar(2,'d', M, min(M,N), lQ)) return
      if(.not.createvar(3,'d', min(M,N), N, lR)) return
      k=4
      if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
      endif
      if(.not.createvar(k,'d', 1, min(M,N), lTAU)) return
      if(lhs.le.2) then
         LWORKMIN = MAX(1,N)
      else 
         LWORKMIN = 3*N
      endif
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=LWORK-LWORKMIN
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      if(lhs.le.2) then
         call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )
      else
         do 5 i = 1, N
 5          istk(lJPVT+i-1) = 0     
            call DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU),
     $           stk(lDWORK), INFO )
c     SUBROUTINE DGEQPF( M, N, A, LDA, JPVT, TAU, WORK, INFO )
         endif
         if(info.ne.0) then
            return
         endif
         call DLACPY( 'U', min(M,N), N, stk(lA), M, stk(lR), min(M,N) )
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
         if(N.ge.2) then
            do 20 j = 1, N-1
               if(j+1.le.min(M,N)) then
                  do 10 i = j+1, min(M,N)
                     ij = i+(j-1)*min(M,N)
                     stk(lR+ij-1) = ZERO
 10               continue
               endif
 20         continue
         endif
         
         call DLACPY( 'F', M, min(M,N), stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
         call DORGQR( M, min(M,N), min(M,N), stk(lQ), M, stk(lTAU),
     $        stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DORGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )
         
         if(lhs.gt.2) then      
            call DLASET( 'F', N, N, ZERO, ZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
            do 30 j = 1, N
               i = istk(lJPVT+j-1)
               ij = i + (j-1)*N
               stk(lE+ij-1) = ONE
 30         continue    
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



      subroutine intzgeqpf3(fname)

c     [Q,R]=qr(A)
c     [Q,R,E]=qr(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ONE, DZERO
      parameter ( ONE=1.0D0, DZERO=0.0D0)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )


      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'z', 0, 0, lQ)) return
         if(.not.createvar(3,'z', 0, 0, lR)) return
         if(lhs.gt.2) then
            if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1) = 2
         lhsvar(2) = 3
         if(lhs.gt.2) then
            lhsvar(3) = 4
         endif
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'z', M, M, lQ)) return
      if(.not.createvar(3,'z', M, N, lR)) return
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

      call ZLACPY( 'U', M, N, zstk(lA), M, zstk(lR), M )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         if(M.gt.N) then
            do 20 j = 1, N
               do 10 i = j+1, M
                  ij = i+(j-1)*M
                  zstk(lR+ij-1) = ZERO
 10            continue
 20         continue
         else
            do 40 j = 1, M-1
               do 30 i = j+1, M
                  ij = i+(j-1)*M
                  zstk(lR+ij-1) = ZERO
 30            continue
 40         continue
         endif 
      endif
      
      if(M.gt.N) then
         call ZLACPY( 'F', M, N, zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         do 60 j = N+1, M
            do 50 i = 1, M
               ij = i+(j-1)*M 
               zstk(lQ+ij-1) = ZERO
 50         continue
 60      continue
      else
         call ZLACPY( 'F', M, M, zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      endif
      call ZUNGQR( M, M, min(M,N), zstk(lQ), M, zstk(lTAU),
     $     zstk(lDWORK), LWORK, INFO )
c     SUBROUTINE ZUNGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )

      if(lhs.gt.2) then
         call DLASET( 'F', N, N, DZERO, DZERO, stk(lE), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
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
      
      subroutine intzgeqpf4(fname)

c     [Q,R]=qr(A,'e')
c     [Q,R,E]=qr(A,'e')

      include '../stack.h'
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
      
      subroutine intdgesvd1(fname)

c     [U,S,V]=svd(A)
c     s = svd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )

      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', M, N, lS)) return
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
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'d', N, N, lV)) return
         if(.not.createvar(6,'d', N, N, lVT)) return
         k = 7
      endif
      LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=LWORK-LWORKMIN
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
         JOBU = 'A'
         JOBVT = 'A'
      endif
      if(lhs.eq.1) then
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $        M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
c     next lines to patch an error of DGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $        M, stk(lVT), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      endif  
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

      if(lhs.gt.1) then
         call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*M
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, N
            do 20 i = j, N
               ij = i+(j-1)*N
               ji = j+(i-1)*N
               stk(lV+ij-1) = stk(lVT+ji-1)
               stk(lV+ji-1) = stk(lVT+ij-1)
 20         continue
 30      continue  
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
      else
         lhsvar(1) = 2
      endif
      end

      subroutine intdgesvd2(fname)

c     [U,S,V]=svd(A)
c     s = svd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', M, N, lA)) return
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
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, min(M,N), lU)) return
         if(.not.createvar(4,'d', min(M,N), min(M,N), lS)) return
         if(.not.createvar(5,'d', N, min(M,N), lV)) return
         if(.not.createvar(6,'d', min(M,N), N, lVT)) return
         k = 7
      endif
      LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=LWORK-LWORKMIN
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
         JOBU = 'S'
         JOBVT = 'S'
      endif
      if(lhs.eq.1) then
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $        M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
c     next lines to patch an error of DGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $        M, stk(lVT), min(M,N), stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
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
               stk(lV+ij-1) = stk(lVT+ji-1)
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

      subroutine intzgesvd1(fname)

c     [U,S,V]=svd(A)
c     s = svd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite
      
      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      complex*16 void

      parameter ( ZERO=0.0D0 )
      intrinsic conjg

      minrhs=1
      maxrhs=1
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
         if(.not.createvar(3,'z', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'z', N, N, lV)) return
         if(.not.createvar(6,'z', N, N, lVT)) return
         k = 7
      endif
      LRWRK = max(3*min(M,N),5*min(M,N)-4)
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
         JOBU = 'A'
         JOBVT = 'A'
      endif
      if(lhs.eq.1) then
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), void
     $        ,M, void, N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
C     Next lines introduced to patch an error of  ZGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lU)
     $        ,M, zstk(lVT), N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      endif 
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

      if(lhs.gt.1) then
         call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*M
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, N
            do 20 i = j, N
               ij = i+(j-1)*N
               ji = j+(i-1)*N
               zstk(lV+ij-1) = conjg(zstk(lVT+ji-1))
               zstk(lV+ji-1) = conjg(zstk(lVT+ij-1))
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

      subroutine intzgesvd2(fname)

c     [U,S,V]=svd(A)
c     s = svd(A)

      include '../stack.h'
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

      subroutine intdgelsy(fname)

c     [X,rank]=lsq(A,B,rcond)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision EPS, RCOND
      double precision dlamch
      external dlamch
      intrinsic sqrt
c     
      minrhs=2
      maxrhs=3
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'d', MA, N, lA)) return
      if(.not.getrhsvar(2,'d', MB, NRHS, lB)) return
      if(MA .ne. MB) then
         call error(265)
         return
      endif
      M = MA
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'d', 0, 0, lX)) return
         if(.not.createvar(4,'i', 0, 0, lRANK)) return
         lhsvar(1) = 3
         lhsvar(2) = 4
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(rhs.eq.2) then
         eps = dlamch('eps')
         rcond = sqrt(eps)
         MR = 1
         NR = 1
         if(.not.createvar(3,'d', MR, NR, lRCOND)) return
         stk(lRCOND) = RCOND
      else
         if(.not.getrhsvar(3,'d', MR, NR, lRCOND)) return
      endif
      if(MR.ne.1 .or. NR.ne.1 ) then
         err=3
         call error(204)
         return
      endif
      if(.not.createvar(4,'d', N, NRHS, lX)) return
      if(.not.createvar(5,'d', max(M,N), NRHS, lXB)) return
      if(.not.createvar(6,'i', 1, 1, lRANK)) return
      if(.not.createvar(7,'i', 1, N, lJPVT)) return

      LWORKMIN = max( min(M,N)+3*N+1, 2*min(M,N)+NRHS )
      LWORK=maxvol(8,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(8,'d',1,LWORK,lDWORK)) return

      call DLACPY( 'F', M, NRHS, stk(lB), M, stk(lXB), max(M,N) )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
         istk(lJPVT+i-1) = 0
 10   continue
      call DGELSY( M, N, NRHS, stk(lA), M, stk(lXB), max(M,N),
     $     istk(lJPVT), stk(lRCOND), istk(lRANK), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $     RANK, WORK, LWORK, INFO )
      if(info.ne.0) then
         return
      endif
      
      call DLACPY( 'F', N, NRHS, stk(lXB), max(M,N), stk(lX), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )

      if(lhs.eq.1) then
         lhsvar(1)=4
      else
         lhsvar(1)=4
         lhsvar(2)=6
      endif
c     
      end

      subroutine intzgelsy(fname)

c     [X,rank]=lsq(A,B,rcond)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision EPS, RCOND
      double precision dlamch
      external dlamch
      intrinsic sqrt

      minrhs=2
      maxrhs=3
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', MA, N, lA)) return
      if(.not.getrhsvar(2,'z', MB, NRHS, lB)) return

      if(MA .ne. MB) then
         call error(265)
         return
      endif
      M = MA
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         if(.not.createvar(4,'i', 0, 0, lRANK)) return     
         lhsvar(1) = 3
         lhsvar(2) = 4
         return
      elseif(M.eq.-1 .or.N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(rhs.eq.2) then
         eps = dlamch('eps')
         rcond = sqrt(eps)
         MR = 1
         NR = 1
         if(.not.createvar(3,'d', MR, NR, lRCOND)) return
         stk(lRCOND) = RCOND
      else
         if(.not.getrhsvar(3,'d', MR, NR, lRCOND)) return
      endif
      if(MR.ne.1 .or. NR.ne.1 ) then
         err=3
         call error(204)
         return
      endif
      if(.not.createvar(4,'z', N, NRHS, lX)) return
      if(.not.createvar(5,'z', max(M,N), NRHS, lXB)) return
      if(.not.createvar(6,'i', 1, 1, lRANK)) return
      if(.not.createvar(7,'i', 1, N, lJPVT)) return
      if(.not.createvar(8,'d', 1, 2*N, lRWORK)) return
      LWORKMIN = min(M,N) +  max( 2*min(M,N), N+1, min(M,N)+NRHS )
      LWORK=maxvol(9,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(9,'z',1,LWORK,lDWORK)) return

      call ZLACPY( 'F', M, NRHS, zstk(lB), M, zstk(lXB), max(M,N) )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
         istk(lJPVT+i-1) = 0
 10   continue
      call ZGELSY( M, N, NRHS, zstk(lA), M, zstk(lXB), max(M,N),
     $     istk(lJPVT), stk(lRCOND), istk(lRANK), zstk(lDWORK),
     $     LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $     RANK, WORK, LWORK, RWORK, INFO )
      if(info.ne.0) then
         return
      endif
      call ZLACPY( 'F', N, NRHS, zstk(lXB), max(M,N), zstk(lX), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )

      lhsvar(1)=4
      lhsvar(2)=6
c     
      end

      subroutine intdgeev(fname)

c     [V,D]=spec(A)
c     d = spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBVL, JOBVR
      double precision DZERO
      parameter ( DZERO=0.0D0 )
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )
      INTRINSIC  DCMPLX

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
            if(.not.createvar(2,'z', 0, 0, lD)) return
            lhsvar(1) = 2
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'z', 0, 0, lD)) return
            if(.not.createvar(3,'d', 0, 0, lV)) return
            lhsvar(1) = 2
            lhsvar(2) = 3
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif 
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif  
      
      if(lhs.eq.1) then
         if(.not.createvar(2,'z', N, 1, lD)) return
         k = 3              
      else if(lhs.eq.2) then
         if(.not.createvar(2,'z', N, N, lD)) return
         if(.not.createvar(3,'d', N, N, lVR)) return
         if(.not.createvar(4,'z', N, N, lV)) return
         k = 5
      endif
      if(.not.createvar(k,'d', N, 1, lWR)) return
      if(.not.createvar(k+1,'d', N, 1, lWI)) return
      LWORKMIN = 3*N
      if(lhs.gt.1) LWORKMIN = 4*N
      LWORK=maxvol(k+2,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'d',1,LWORK,lDWORK)) return
      JOBVL = 'N'
      if(lhs.eq.1) then
         JOBVR = 'N'
      else
         JOBVR = 'V'
      endif 
      call DGEEV( JOBVL, JOBVR, N, stk(lA), N, stk(lWR), stk(lWI),
     $     stk(lDWORK), N, stk(lVR), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGEEV( JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR,
c     LDVR, 
c     $     WORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.eq.1) then
         call ZLASET( 'F', N, 1, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 5 i = 1, N
            zstk(lD+i-1) = dcmplx(stk(lWR+i-1),stk(lWI+i-1))
 5       continue     
      else 
         call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, N
            ii = i+(i-1)*N
            zstk(lD+ii-1) = dcmplx(stk(lWR+i-1),stk(lWI+i-1))
 10      continue   
         
         j = 0
 20      j = j+1
         if(stk(lWI+j-1).eq.DZERO) then
            do 30 i = 1, N
               ij = i+(j-1)*N
               zstk(lV+ij-1) = stk(lVR+ij-1)
 30         continue
         else   
            do 40 i = 1, N
               ij = i+(j-1)*N
               ij1 = i+j*N
               zstk(lV+ij-1) = dcmplx(stk(lVR+ij-1),stk(lVR+ij1-1))
               zstk(lV+ij1-1) = dcmplx(stk(lVR+ij-1),-stk(lVR+ij1-1)) 
 40         continue
            j = j+1
         endif
         if(j.lt.N) go to 20
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 2
      else 
         lhsvar(1)=4
         lhsvar(2)=2
      endif
c     
      end

      subroutine intzgeev(fname)

c     [V,D]=spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite
      
      character fname*(*)
      character JOBVL, JOBVR
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )

      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         if(lhs.eq.1) then
            if(.not.createvar(2,'z',0,0,lD)) return
            lhsvar(1) = 2
            return
         else
            if(.not.createvar(2,'z', 0, 0, lD)) return
            if(.not.createvar(3,'d', 0, 0, lV)) return
            lhsvar(1) = 2
            lhsvar(2) = 3
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  
      if(vfinite(2*M*N,zstk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      if(lhs.eq.1) then
         if(.not.createvar(2,'z', N, 1, lD)) return
         k = 3              
      else if(lhs.eq.2) then
         if(.not.createvar(2,'z', N, N, lD)) return
         if(.not.createvar(3,'z', N, N, lVR)) return
         k = 4
      endif
      if(.not.createvar(k,'z', N, 1, lW)) return
      if(.not.createvar(k+1,'d', 2*N, 1, lRWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+2,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'z',1,LWORK,lDWORK)) return

      JOBVL = 'N'
      if(lhs.eq.1) then
         JOBVR = 'N'
      else
         JOBVR = 'V'
      endif 
      call ZGEEV( JOBVL, JOBVR, N, zstk(lA), N, zstk(lW), 
     $     zstk(lDWORK), N, zstk(lVR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), INFO )
c     SUBROUTINE ZGEEV( JOBVL, JOBVR, N, A, LDA, W, VL, LDVL,
c     $     VR, LDVR, WORK, LWORK, RWORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.gt.1) then
         call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, N
            ii = i+(i-1)*N
            zstk(lD+ii-1) = zstk(lW+i-1)
 10      continue 
      else
         do 11 i = 1, N
            zstk(lD+i-1) = zstk(lW+i-1)
 11      continue
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 2
      else
         lhsvar(1)=3
         lhsvar(2)=2
      endif
c     
      end

      subroutine intdgetri(fname)

c     B = inv(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision anorm,rcond
      double precision dlange,dlamch
      external dlange,dlamch

c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if(n.eq.0) then
         lhsvar(1) = 1
         return
      elseif(n.eq.-1) then
         stk(lA)=1.0d0/stk(lA)
         lhsvar(1) = 1
         return
      endif
      if(.not.createvar(2,'i',1,N,lIPIV)) return
      if(.not.createvar(3,'i',1,N,lIWORK)) return
      LWORKMIN = MAX(1,N)
      LWORK=maxvol(4,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(4,'d',1,LWORK,lDWORK)) return



      call DGETRF( N, N, stk(lA), N, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.gt.0) then
         call error(19)
         return
      elseif (info.lt.0) then
         return
      endif
      RCOND = 0.0d0
      ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
      call DGECON( '1', N, stk(lA), N, ANORM, RCOND, stk(lDWORK),
     $     istk(lIWORK), INFO )
      if(RCOND.le.sqrt(dlamch('eps'))) then
         write(buf(1:13),'(1pd13.4)') RCOND
c     .  ill conditionned problem
         call msgs(5,0)
      endif


      call DGETRI( N, stk(lA), N, istk(lIPIV), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end

      subroutine intzgetri(fname)

c     B = inv(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      double precision zlange,dlamch,rcond,anorm
      external zlange,dlamch

      character fname*(*)
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if(n.eq.0) then
         lhsvar(1) = 1
         return

      elseif(n.eq.-1) then
         stk(lA)=1.0d0/stk(lA)
         lhsvar(1) = 1
         return
      endif
      if(.not.createvar(2,'i',1,N,lIPIV)) return
      if(.not.createvar(3,'d',1,2*N,lRWORK)) return
      LWORKMIN = MAX(1,2*N)
      LWORK=maxvol(4,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(4,'z',1,LWORK,lDWORK)) return

      call ZGETRF( N, N, zstk(lA), N, istk(lIPIV), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.gt.0) then
         call error(19)
         return
      elseif (info.lt.0) then
         return
      endif
      RCOND = 0.0d0
      ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )

   
      call ZGECON( '1', N, zstk(lA), N, ANORM, RCOND, zstk(lDWORK),
     $     stk(lRWORK), INFO )
      if(RCOND.le.sqrt(dlamch('eps'))) then
         write(buf(1:13),'(1pd13.4)') RCOND
c     .  ill conditionned problem
         call msgs(5,0)
      endif

      call ZGETRI( N, zstk(lA), N, istk(lIPIV), zstk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE ZGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end


      subroutine intdgecon(fname)

c     rcond

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM
      double precision dlange
      external dlange
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA .ne. NA) then
         err=1
         call error(20)
         return
      endif
      N = MA

      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 2
         return
      elseif(n.eq.-1) then
         if(.not.createvar(2,'d', 1, 1, lRCOND)) return
         stk(lRCOND)=1.0d0
         lhsvar(1) = 1
         return
      endif

      if(.not.createvar(2,'d', 1, 1, lRCOND)) return
      if(.not.createvar(3,'i', 1, N, lIPIV)) return
      if(.not.createvar(4,'i', 1, N, lIWORK)) return
      LWORKMIN =  4*N
      LWORK=maxvol(5,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(5,'d',1,LWORK,lDWORK)) return
      ANORM = dlange( '1', N, N, stk(lA), N, stk(lDWORK) )
c     DOUBLE PRECISION FUNCTION DLANGE( NORM, M, N, A, LDA, WORK )
      call DGETRF( N, N, stk(lA), N, istk(lIPIV), INFO )         
c     SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
      stk(lRCOND) = 0.0d0
      if(INFO.eq.0) then
         call DGECON( '1', N, stk(lA), N, ANORM, stk(lRCOND),
     $        stk(lDWORK), istk(lIWORK), INFO )
c     SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     IWORK, INFO )
      endif

      lhsvar(1) = 2
      return
c     
      end

      subroutine intzgecon(fname)

c     rcond

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM
      double precision zlange
      external zlange
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
      if(MA .ne. NA) then
         err=1
         call error(20)
         return
      endif
      N = MA

      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 2
         return
      elseif(n.eq.-1) then
         if(.not.createvar(2,'d', 1, 1, lRCOND)) return
         stk(lRCOND)=1.0d0
         lhsvar(1) = 1
         return
      endif

      if(.not.createvar(2,'d', 1, 1, lRCOND)) return
      if(.not.createvar(3,'i', 1, N, lIPIV)) return
      if(.not.createvar(4,'d', 1, 2*N, lRWORK)) return
      LWORKMIN =  2*N
      LWORK=maxvol(5,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(5,'z',1,LWORK,lDWORK)) return
      ANORM = zlange( '1', N, N, zstk(lA), N, zstk(lDWORK) )
c     DOUBLE PRECISION FUNCTION ZLANGE( NORM, M, N, A, LDA, WORK )
      call ZGETRF( N, N, zstk(lA), N, istk(lIPIV), INFO )         
c     SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
      stk(lRCOND) = 0.0d0
      if(INFO.eq.0) then
         call ZGECON( '1', N, zstk(lA), N, ANORM, stk(lRCOND),
     $        zstk(lDWORK),  stk(lRWORK), INFO )
c     SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     RWORK, INFO )
      endif

      lhsvar(1) = 2
      return
c     
      end

      subroutine intdpotrf(fname)

c     U = chol(A)

      include '../stack.h'
      logical getrhsvar
      logical checklhs,checkrhs
      character fname*(*)
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         lhsvar(1) = 1
         return
      elseif(N.eq.-1) then
         if(stk(lA).le.0.0d0) then
            call error(29)
            return
         endif
         lhsvar(1) = 1
         stk(lA)=sqrt(stk(lA))
         return
      endif

      call DPOTRF( 'U', N, stk(lA), N, INFO )
c     SUBROUTINE DPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         if(info.gt.0)  call error(29)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               stk(lA+ij-1)=0.0d0    
 10         continue
 20      continue
      endif
      
      lhsvar(1)=1
      
c     
      end


      subroutine intzpotrf(fname)

c     U = chol(A)

      include '../stack.h'
      logical getrhsvar
      logical checklhs,checkrhs
      complex*16 ZERO
      parameter ( ZERO = (0.0d0,0.0d0) )
      character fname*(*)
      
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if (N.eq.0) then
         lhsvar(1)=1
         return
      elseif(N.eq.-1) then
         if(stk(lA).le.0.0d0) then
            call error(29)
            return
         endif
         lhsvar(1)=1
         stk(lA)=sqrt(stk(lA))
         return
      endif

      call ZPOTRF( 'U', N, zstk(lA), N, INFO )
c     SUBROUTINE ZPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         if(info.gt.0)  call error(29)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               zstk(lA+ij-1)=ZERO    
 10         continue
 20      continue
      endif
      
      lhsvar(1)=1
      
c     
      end

      subroutine intdgetrf(fname)

c     [L,U,E] = lu(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO = 0.0D0, ONE = 1.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         lhsvar(1) = 1
         if(.not.createvar(2,'d',0,0,lL)) return
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'d',0,0,lE)) return
            lhsvar(3) = 3
         endif
         return
      elseif (M.eq.-1 .or.N.eq.-1) then
         lhsvar(1) = 1
         if(.not.createvar(2,'d',-1,-1,lL)) return
         stk(lL)=stk(lA)
         stk(lA)=1.0d0
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'d',-1,-1,lE)) return
            stk(lE)=1.0d0
            lhsvar(3) = 3
         endif
         return
      endif
      if(.not.createvar(2,'d',M,min(M,N),lL)) return
      if(.not.createvar(3,'d',min(M,N),N,lU)) return
      if(.not.createvar(4,'i',1,min(M,N),lIPIV)) return
      if(.not.createvar(5,'d',M,M,lE)) return
      if(lhs.eq.2) then
         if(.not.createvar(6,'i',1,M,lIW)) return
         if(.not.createvar(7,'d',M,min(M,N),lW)) return
      endif


      call DGETRF( M, N, stk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      
      do 20 j=1,min(M,N)
         do 10 i=1,M
            ij = i+(j-1)*M
            if(i.eq.j) then
               stk(lL+ij-1)=ONE
            else if(i.gt.j) then
               stk(lL+ij-1)=stk(lA+ij-1)
            else if(i.lt.j) then
               stk(lL+ij-1)=ZERO
            endif
 10      continue
 20   continue     
      do 40 j=1,N
         do 30 i=1,min(M,N)
            ij1 = i+(j-1)*min(M,N)
            ij2 = i+(j-1)*M
            if(i.gt.j) then
               stk(lU+ij1-1)=ZERO
            else if(i.le.j) then
               stk(lU+ij1-1)=stk(lA+ij2-1)
            endif
 30      continue
 40   continue
      if(lhs.eq.2) then
c     form E'*L
         do 41 i = 1, M
            istk(lIW-1+i)=i
 41      continue
         do 42 i = 1, min(M,N)
            ip = istk(lIPIV-1+i)
            if( ip.ne.i ) then
               iw=istk(lIw-1+i)
               istk(lIW-1+i)=istk(lIW-1+ip)
               istk(lIW-1+ip)=iw
            endif
 42      continue
         do 43 i= 1, M
            ip =istk(lIW-1+i)
            call dcopy(N,stk(lL-1+i),M,stk(lW-1+ip),M)
 43      continue
         lhsvar(1)=7
         lhsvar(2)=3
      else
         call DLASET( 'F', M, M, ZERO, ONE, stk(lE), M ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         call DLASWP( M, stk(lE), M, 1, min(M,N), istk(lIPIV), 1 )
c     SUBROUTINE DLASWP( N, A, LDA, K1, K2, IPIV, INCX )   
         lhsvar(1)=2
         lhsvar(2)=3
         lhsvar(3)=5
      endif   


      
c     
      return     
      end


      subroutine intzgetrf(fname)

c     [L,U,E] = lu(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO = 0.0D0, ONE = 1.0D0 )
      complex*16 CZERO, CONE
      parameter ( CZERO = (0.0D0,0.0D0), CONE = (1.0D0,0.0D0) )
c     
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         lhsvar(1) = 1
         if(.not.createvar(2,'d',0,0,lL)) return
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'d',0,0,lE)) return
            lhsvar(3) = 3
         endif
         return
      elseif (M.eq.-1 .or.N.eq.-1) then
         lhsvar(1) = 1
         if(.not.createvar(2,'z',-1,-1,lL)) return
         zstk(lL)=zstk(lA)
         zstk(lA)=CONE
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'z',-1,-1,lE)) return
            zstk(lE)=CONE
            lhsvar(3) = 3
         endif
         return
      endif
      if(.not.createvar(2,'z',M,min(M,N),lL)) return
      if(.not.createvar(3,'z',min(M,N),N,lU)) return
      if(.not.createvar(4,'i',1,min(M,N),lIPIV)) return
      if(.not.createvar(5,'d',M,M,lE)) return
      if(lhs.eq.2) then
         if(.not.createvar(6,'i',1,M,lIW)) return
         if(.not.createvar(7,'z',M,min(M,N),lW)) return
      endif

      call ZGETRF( M, N, zstk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      
      do 20 j=1,min(M,N)
         do 10 i=1,M
            ij = i+(j-1)*M
            if(i.eq.j) then
               zstk(lL+ij-1) = CONE
            else if(i.gt.j) then
               zstk(lL+ij-1) = zstk(lA+ij-1)
            else if(i.lt.j) then
               zstk(lL+ij-1) = CZERO
            endif
 10      continue
 20   continue     
      do 40 j=1,N
         do 30 i=1,min(M,N)
            ij1 = i+(j-1)*min(M,N)
            ij2 = i+(j-1)*M
            if(i.gt.j) then
               zstk(lU+ij1-1) = CZERO
            else if(i.le.j) then
               zstk(lU+ij1-1) = zstk(lA+ij2-1)
            endif
 30      continue
 40   continue

      if(lhs.eq.2) then
c     form E'*L
         do 41 i = 1, M
            istk(lIW-1+i)=i
 41      continue
         do 42 i = 1, min(M,N)
            ip = istk(lIPIV-1+i)
            if( ip.ne.i ) then
               iw=istk(lIw-1+i)
               istk(lIW-1+i)=istk(lIW-1+ip)
               istk(lIW-1+ip)=iw
            endif
 42      continue
         do 43 i= 1, M
            ip =istk(lIW-1+i)
            call zcopy(N,zstk(lL-1+i),M,zstk(lW-1+ip),M)
 43      continue
         lhsvar(1)=7
         lhsvar(2)=3
      else
         call DLASET( 'F', M, M, ZERO, ONE, stk(lE), M ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         call DLASWP( M, stk(lE), M, 1, min(M,N), istk(lIPIV), 1 )
c     SUBROUTINE DLASWP( N, A, LDA, K1, K2, IPIV, INCX )   
         lhsvar(1)=2
         lhsvar(2)=3
         lhsvar(3)=5
      endif   



c     
      return     
      end

      subroutine intddet(fname)

c     det(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE, TEN
      double precision D, E
      parameter ( ZERO = 0.0D0, ONE = 1.0D0, TEN = 10.0D0 )
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
         lhsvar(1) = 2
         if(.not.createvar(2,'d',1,1,lL)) return
         stk(lL)=1.0d0
         if (lhs.eq.2) then
            if(.not.createvar(3,'d',1,1,lL)) return
            stk(lL)=0.0d0
            lhsvar(2) = 3
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif

      if(.not.createvar(2,'i',1,M,lIPIV)) return


      call DGETRF( M, N, stk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      if (lhs.eq.1) then
         D=one
         do 10 i=0,N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D=D*stk(lA+i*(M+1))
 10      continue
         if(.not.createvar(1,'d',1,1,lD)) return
         stk(lD)=D
         lhsvar(1)=1
      else 
c     .  [e,m]=det(A)
         D = ONE
         E = ZERO
         do 40 i = 0, N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D = D*stk(lA+i*(M+1))
            if (D .eq. ZERO) go to 50
 15         if (abs(D) .ge. ONE) go to 20
            D = ten*D
            E = E - ONE
            go to 15
 20         continue
 25         if (abs(D) .lt. ten) go to 30
            D = D/ten
            E = E + ONE
            go to 25
 30         continue
 40      continue
 50      continue
         if(.not.createvar(1,'d',1,1,le)) return
         stk(le) = E
         if(.not.createvar(2,'d',1,1,lm)) return
         stk(lm) = D
         lhsvar(1) = 1
         lhsvar(2) = 2
      endif
c     
      return     
      end


      subroutine intzdet(fname)

c     det(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE, TEN, E
      complex*16 D
      parameter ( ZERO = 0.0D0, ONE = 1.0D0, TEN = 10.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         lhsvar(1) = 1
         if (lhs.eq.2) then
            if(.not.createvar(2,'d',0,0,lL)) return
            lhsvar(2) = 2
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif

      if(.not.createvar(2,'i',1,M,lIPIV)) return


      call ZGETRF( M, N, zstk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      if (lhs.eq.1) then
         D=one
         do 10 i=0,N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D=D*zstk(lA+i*(M+1))
 10      continue
         if(.not.createvar(1,'z',1,1,lD)) return
         zstk(lD)=D
         lhsvar(1)=1
      else 
c     .  [e,m]=det(A)
         D = ONE
         E = ZERO
         do 40 i=0,N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D = zstk(lA+i*(M+1))*D
            if (abs(D) .eq. ZERO) go to 50
 15         if (abs(D) .ge. ONE) go to 20
            D = ten*D
            E = E - ONE
            go to 15
 20         continue
 25         if (abs(D) .lt. ten) go to 30
            D = D/ten
            E = E + ONE
            go to 25
 30         continue
 40      continue
 50      continue
         if(.not.createvar(1,'d',1,1,le)) return
         stk(le) = E
         if(.not.createvar(2,'z',1,1,lm)) return
         zstk(lm) = D
         lhsvar(1) = 1
         lhsvar(2) = 2
      endif
c     
      return     
      end


      subroutine intzgesv3(fname)

c     a\b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      external dlamch, zlange
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
               write(buf(1:13),'(1pd13.4)') RCOND
               call msgs(5,0)
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


      subroutine intzgesv4(fname)

c     a/b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      external dlamch, zlange
      intrinsic conjg, sqrt
c     
      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      

      if(.not.getrhsvar(1,'z', MB, NB, lB)) return
      if(.not.getrhsvar(2,'z', MA, NA, lA)) return
      if(MB .eq. 0) then
c     .  for backwar compatibility
         lhsvar(1) = 1
         return
      endif
      if(NA .ne. NB) then
         call error(266)
         return
      endif
      M = MA
      N = NA
      K = MB
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         lhsvar(1) = 3
         return
      endif

      if(.not.createvar(3,'z', N, M, lAF)) return
      if(.not.createvar(4,'z', K, M, lX)) return
      if(.not.createvar(5,'z', N, M, lAT)) return
      if(.not.createvar(6,'z', max(M,N), K, lBT)) return
      if(.not.createvar(7,'i', 1, 1, lRANK)) return
      if(.not.createvar(8,'i', 1, N, lIPIV)) return
      if(.not.createvar(9,'i', 1, M, lJPVT)) return
      if(.not.createvar(10,'d',1,2*M,lRWORK)) return    
      LWORKMIN = max( 2*N, min(M,N)+max( 2*min(M,N), M+1,
     $     min(M,N)+K ))
      LWORK=maxvol(11,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(11,'z',1,LWORK,lDWORK)) return
      EPS = dlamch('eps')
      ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )
c     
c     Transpose A and B
c     
      do 20 j = 1, M
         do 10 i = 1, N
            ij = i+(j-1)*N
            ji = j+(i-1)*M
            zstk(lAT+ij-1) = conjg( zstk(lA+ji-1) )
 10      continue
 20   continue 
      do 40 j = 1, K
         do 30 i = 1, N
            ij = i+(j-1)*max(M,N)
            ji = j+(i-1)*K
            zstk(lBT+ij-1) = conjg( zstk(lB+ji-1) )
 30      continue
 40   continue 
c     
      if(M.eq.N) then
c     
c     M = N
c     
         call ZLACPY( 'F', N, N, zstk(lAT), N, zstk(lAF), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZGETRF( N, N, zstk(lAF), N, istk(lIPIV), INFO )         
c     SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
            call ZGECON( '1', N, zstk(lAF), N, ANORM, RCOND,
     $           zstk(lDWORK), stk(lRWORK), INFO )
c     SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        RWORK, INFO )
            if(RCOND.gt.sqrt(EPS)) then
               call ZGETRS( 'N', N, K, zstk(lAF), N, istk(lIPIV),
     $              zstk(lBT), N, INFO ) 
c     SUBROUTINE ZGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c     B, LDB, INFO )
c     
c     Back transpose X
c     
               do 60 j = 1, N
                  do 50 i = 1, K
                     ij = i+(j-1)*K
                     ji = j+(i-1)*N
                     zstk(lX+ij-1) = conjg( zstk(lBT+ji-1) )
 50               continue
 60            continue
               lhsvar(1) = 4
               return
            endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
c     .  ill conditionned problem
         call msgs(5,0)
      endif
c     
c     M.ne.N or A singular
c     
      RCOND = sqrt(eps)
      do 70 i = 1, M
         istk(lJPVT+i-1) = 0
 70   continue
      call ZGELSY1( N, M, K, zstk(lAT), N, zstk(lBT), max(M,N),
     $     istk(lJPVT), RCOND, istk(lRANK), zstk(lDWORK),
     $     LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGELSY1( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                     RANK, WORK, LWORK, RWORK, INFO )
      if(info.ne.0) then
         return
      endif
      if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call msgs(9,istk(lRANK))
      endif
c     
c     Back transpose X
c     
      do 90 j = 1, M
         do 80 i = 1, K
            ij = i+(j-1)*K
            ji = j+(i-1)*max(M,N)
            zstk(lX+ij-1) = conjg( zstk(lBT+ji-1) )
 80      continue
 90   continue 

      lhsvar(1)=4
      return
c     
      end



      subroutine intdgesv4(fname)

c     a/b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, dlange
      external dlamch, dlange
      intrinsic sqrt
c     
      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'d', MB, NB, lB)) return
      if(.not.getrhsvar(2,'d', MA, NA, lA)) return
      if(MB .eq. 0) then
         lhsvar(1) = 1
         return
      endif
      if(NA .ne. NB) then
         call error(266)
         return
      endif
      M = MA
      N = NA
      K = MB
      if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'d', 0, 0, lX)) return
         lhsvar(1) = 3
         return
      endif

      if(.not.createvar(3,'d', N, M, lAF)) return
      if(.not.createvar(4,'d', K, M, lX)) return
      if(.not.createvar(5,'d', N, M, lAT)) return
      if(.not.createvar(6,'d', max(M,N), K, lBT)) return
      if(.not.createvar(7,'i', 1, 1, lRANK)) return
      if(.not.createvar(8,'i', 1, N, lIPIV)) return
      if(.not.createvar(9,'i', 1, M, lJPVT)) return
      if(.not.createvar(10,'i', 1, N, lIWORK)) return
      LWORKMIN = max( 4*N, min(M,N)+3*M+1, 2*min(M,N)+K )
      LWORK=maxvol(11,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(11,'d',1,LWORK,lDWORK)) return
      EPS = dlamch('eps')
      ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
c     
c     Transpose A and B
c     
      do 20 j = 1, M
         do 10 i = 1, N
            ij = i+(j-1)*N
            ji = j+(i-1)*M
            stk(lAT+ij-1) = stk(lA+ji-1)
 10      continue
 20   continue 
      do 40 j = 1, K
         do 30 i = 1, N
            ij = i+(j-1)*max(M,N)
            ji = j+(i-1)*K
            stk(lBT+ij-1) = stk(lB+ji-1)
 30      continue
 40   continue 
c     
      if(M.eq.N) then
c     
c     M = N
c     
         call DLACPY( 'F', N, N, stk(lAT), N, stk(lAF), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
         call DGETRF( N, N, stk(lAF), N, istk(lIPIV), INFO )         
c     SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
            call DGECON( '1', N, stk(lAF), N, ANORM, RCOND, stk(lDWORK),
     $           istk(lIWORK), INFO )
c     SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        IWORK, INFO )
            if(RCOND.gt.sqrt(EPS)) then
               call DGETRS( 'N', N, K, stk(lAF), N, istk(lIPIV),
     $              stk(lBT), N, INFO ) 
c     SUBROUTINE DGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c     B, LDB, INFO )
c     
c     Back transpose X
c     
               do 60 j = 1, N
                  do 50 i = 1, K
                     ij = i+(j-1)*K
                     ji = j+(i-1)*N
                     stk(lX+ij-1) = stk(lBT+ji-1)
 50               continue
 60            continue
               lhsvar(1) = 4
               return
            endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
         call msgs(5,0)
      endif
c     
c     M.ne.N or A singular
c     
      RCOND = sqrt(eps)
      do 70 i = 1, M
         istk(lJPVT+i-1) = 0
 70   continue
      call DGELSY1( N, M, K, stk(lAT), N, stk(lBT), max(M,N),
     $     istk(lJPVT), RCOND, istk(lRANK), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                     RANK, WORK, LWORK, INFO )
      if(info.ne.0) then
         return
      endif
      if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call msgs(9,istk(lRANK))
      endif
c     
c     Back transpose X
c     
      do 90 j = 1, M
         do 80 i = 1, K
            ij = i+(j-1)*K
            ji = j+(i-1)*max(M,N)
            stk(lX+ij-1) = stk(lBT+ji-1)
 80      continue
 90   continue 

      lhsvar(1)=4
      return
c     
      end


      subroutine intdgesv3(fname)

c     a\b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, dlange
      external dlamch, dlange
      intrinsic sqrt
c     
      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(.not.getrhsvar(2,'d', MB, NRHS, lB)) return
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
         if(.not.createvar(3,'d', 0, 0, lX)) return
         lhsvar(1) = 3
         return
      endif

      if(.not.createvar(3,'d', M, N, lAF)) return
      if(.not.createvar(4,'d', N, NRHS, lX)) return
      if(.not.createvar(5,'d', max(M,N), NRHS, lXB)) return
      if(.not.createvar(6,'i', 1, 1, lRANK)) return
      if(.not.createvar(7,'i', 1, N, lIPIV)) return
      if(.not.createvar(8,'i', 1, N, lJPVT)) return
      if(.not.createvar(9,'i', 1, N, lIWORK)) return       
      LWORKMIN = max( 4*N, min(M,N)+3*N+1, 2*min(M,N)+NRHS )
      LWORK=maxvol(10,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(10,'d',1,LWORK,lDWORK)) return
      
      EPS = dlamch('eps')
      ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
c     
      if(M.eq.N) then
c     
c     M = N
c     
         call DLACPY( 'F', N, N, stk(lA), N, stk(lAF), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
         call DGETRF( N, N, stk(lAF), N, istk(lIPIV), INFO )         
c     SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
            call DGECON( '1', N, stk(lAF), N, ANORM, RCOND, stk(lDWORK),
     $           istk(lIWORK), INFO )
c     SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        IWORK, INFO )
            if(RCOND.gt.sqrt(EPS)) then
               call DGETRS( 'N', N, NRHS, stk(lAF), N, istk(lIPIV),
     $              stk(lB), N, INFO ) 
c     SUBROUTINE DGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c     B, LDB, INFO )
               call DLACPY( 'F', N, NRHS, stk(lB), N, stk(lX), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
               lhsvar(1) = 4
               return
            endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
         call msgs(5,0)
      endif
c     
c     M.ne.N or A singular
c     
      RCOND = sqrt(EPS)
      call DLACPY( 'F', M, NRHS, stk(lB), M, stk(lXB), max(M,N) )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
         istk(lJPVT+i-1) = 0
 10   continue
      call DGELSY1( M, N, NRHS, stk(lA), M, stk(lXB), max(M,N),
     $     istk(lJPVT), RCOND, istk(lRANK), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGELSY1( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                    RANK, WORK, LWORK, INFO )
      if(info.ne.0) then
         return
      endif
      if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call msgs(9,istk(lRANK))
      endif
      call DLACPY( 'F', N, NRHS, stk(lXB), max(M,N), stk(lX), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )

      lhsvar(1)=4
      return
c     
      end


      subroutine intdgees0(fname)

c     [VS,T]=schur(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs 

      character fname*(*)
      character*4 JOBVS, SORT
      integer SDIM
      logical voiddummy
      external voiddummy

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

      if(.not.createvar(2,'d', N, 1, lWR)) return
      if(.not.createvar(3,'d', N, 1, lWI)) return
      k = 4
      if(lhs.gt.1) then
         if(.not.createvar(4,'d', N, N, lVS)) return
         k = 5
      endif
      if(.not.createvar(k,'i', N, 1, lBWORK)) return
      LWORKMIN = 3*N
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return


      if(lhs.eq.1) then
         JOBVS = 'N'
      else
         JOBVS = 'V'
      endif 
      SORT = 'N'

      call DGEES( JOBVS, SORT, voiddummy, N, stk(lA), N, SDIM,
     $     stk(lWR), stk(lWI), stk(lVS), N, stk(lDWORK), LWORK,
     $     istk(lBWORK), INFO )
c     SUBROUTINE DGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, WR, WI,
c     $    VS, LDVS, WORK, LWORK, INFO )
      if(info.gt.0) then
         call msgs(2,info)
      endif

      if(lhs.eq.1) then
         lhsvar(1) = 1
      else
         lhsvar(1)=4
         lhsvar(2)=1
      endif
c     
      end



      subroutine intdgees1(fname)

c     [VS,T]=schur(A,'type')
c     type is r or c

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs 

      character fname*(*)
      
      character TYPE, JOBVS, SORT
      integer SDIM
      logical  voiddummy, WANTD, WANTZ
      double precision ZERO
      parameter (ZERO = 0.0D0)
      external voiddummy
      INTRINSIC  DCMPLX

      minrhs=2
      maxrhs=2
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
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', 0, 0, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(2,'c', M1, N1, lTYPE)) return
      TYPE = cstk(lTYPE:lTYPE)
      WANTD = (TYPE.eq.'r' .or. TYPE.eq.'R')
      WANTZ = (TYPE.eq.'c' .or. TYPE.eq.'C')

      if(.not.WANTD .and. .not.WANTZ) then
         buf='schur'//': type must be real or complex'
         call error(998)
         return
      endif
      if(WANTD) then
         if(.not.createvar(3,'d', N, 1, lWR)) return
         if(.not.createvar(4,'d', N, 1, lWI)) return
         k = 5
         if(lhs.gt.1) then
            if(.not.createvar(5,'d', N, N, lVS)) return
            k = 6
         endif
         if(.not.createvar(k,'i', N, 1, lBWORK)) return
         LWORKMIN = 3*N
         LWORK=maxvol(k+1,'d')
         if(LWORK.le.LWORKMIN) then
            err=(LWORK-LWORKMIN)
            call error(17)
            return
         endif
         if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      else
         if(.not.createvar(3,'z', N, N, lT)) return
         if(.not.createvar(4,'z', N, 1, lW)) return
         k = 5
         if(lhs.gt.1) then
            if(.not.createvar(5,'z', N, N, lVS)) return
            k = 6
         endif
         if(.not.createvar(k,'d', N, 1, lRWORK)) return
         if(.not.createvar(k+1,'i', N, 1, lBWORK)) return
         LWORKMIN = 2*N
         LWORK=maxvol(k+2,'z')
         if(LWORK.le.LWORKMIN) then
            err=2*(LWORK-LWORKMIN)
            call error(17)
            return
         endif
         if(.not.createvar(k+2,'z',1,LWORK,lDWORK)) return     
      endif

      if(lhs.eq.1) then
         JOBVS = 'N'
      else
         JOBVS = 'V'
      endif 
      SORT = 'N'

      if(WANTD) then
         call DGEES( JOBVS, SORT, voiddummy, N, stk(lA), N, SDIM, 
     $        stk(lWR),
     $        stk(lWI), stk(lVS), N, stk(lDWORK), LWORK, istk(lBWORK),
     $        INFO )
c     SUBROUTINE DGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, WR, WI,
c     $    VS, LDVS, WORK, LWORK, INFO )
         if(info.gt.0) then
            call msgs(2,info)
         endif
      else
         do 20 i = 1, N
            do 10 j = 1, N
               ij = i+(j-1)*N
               zstk(lT+ij-1) = dcmplx(stk(lA+ij-1),ZERO)
 10         continue
 20      continue     
         call ZGEES( JOBVS, SORT, voiddummy, N, zstk(lT), N, SDIM,
     $        zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $        istk(lBWORK), INFO )
c     SUBROUTINE ZGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, W,
c     $    VS, LDVS, WORK, LWORK, RWORK, BWORK, INFO )
         if(info.gt.0) then
            call msgs(2,info)
         endif
      endif

      if(WANTD) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
         else
            lhsvar(1)=5
            lhsvar(2)=1
         endif
      else
         if(lhs.eq.1) then
            lhsvar(1) = 3
         else
            lhsvar(1)=5
            lhsvar(2)=3
         endif
      endif
c     
      end

      
      subroutine intoschur(fname)

c     [VS,dim]=schur(A,function)
c     [VS,dim,T]=oschur(A,function)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4  JOBVS, SORT
      logical SCISCHUR,scichk
      external SCISCHUR,scichk
      common /scisch/ lf, nx, nf

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
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
            if(.not.createvar(2,'d',1,1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(1)=1
            lhsvar(2)=2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return 
            stk(lSDIM)=0.0d0
            if(.not.createvar(3,'d', N, N, lVS)) return
            lhsvar(1)=1
            lhsvar(2)=2
            lhsvar(3)=3
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(2,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.1) then
         err=2
         call error(80)
         return
      endif
      if(.not.createvar(3,'d', N, 1, lWR)) return
      if(.not.createvar(4,'d', N, 1, lWI)) return
      k = 5
      if(lhs.gt.1) then
         if(.not.createvar(5,'d', N, N, lVS)) return
         k = 6
      endif
      if(.not.createvar(k,'i', 1, 1, lSDIM)) return 
      if(.not.createvar(k+1,'i', N, 1, lBWORK)) return
      LWORKMIN = 3*N
      LWORK = LWORKMIN
      if(.not.createvar(k+2,'d',1,LWORKMIN,lDWORK)) return
      nx=k+3

      if(lhs.eq.1) then
         JOBVS = 'N'
         lVS = lDWORK
      else
         JOBVS = 'V'
      endif 
      SORT = 'S'
      if(.not.scichk()) return
      call DGEES( JOBVS, SORT, SCISCHUR, N, stk(lA), N, istk(lSDIM),
     $     stk(lWR), stk(lWI), stk(lVS), N, stk(lDWORK), LWORK,
     $     istk(lBWORK), INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(2,info)
         elseif(info.eq.N+1) then
            buf='eigenvalues could not be reordered (the problem '//
     $              'is very ill-conditioned'
            call error(1002)
         elseif(info.eq.N+2) then   
            call msgs(103,0) 
         endif
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 1
      else if(lhs.eq.2) then
         lhsvar(1)=5
         lhsvar(2)=6
      else if(lhs.eq.3) then 
         lhsvar(1)=5
         lhsvar(2)=6
         lhsvar(3)=1
      endif
c     
      end


      subroutine intdgebal(fname)

c     [V,D]=balanc(A)
c     d = balanc(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
c      integer vfinite

      character fname*(*)
      character  JOB
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )

      minrhs=1
      maxrhs=1
      minlhs=2
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
         if(.not.createvar(2,'d', 0, 0, lD)) return
         lhsvar(1) = 1
         lhsvar(2) = 2
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif 

      if(.not.createvar(2,'d', N, N, lZ)) return
      if(.not.createvar(3,'d', 1, N, lSCALE)) return
      JOB='B'
c     SUBROUTINE DGEBAL( JOB, N, A, LDA, ILO, IHI, SCALE, INFO )
      call  DGEBAL(JOB, N, stk(lA), N, ILO, IHI,stk(lSCALE),INF0)

      call DLASET( 'F', N, N, ZERO, ONE, stk(lZ), N ) 
c     SUBROUTINE DGEBAK( JOB, SIDE, N, ILO, IHI, SCALE, M, V, LDV, INFO )
      call DGEBAK(JOB, 'R', N, ILO, IHI, stk(lSCALE), N,
     $     stk(lZ), N, INFO )
 
      lhsvar(1)=1
      lhsvar(2)=2
c     
      end

      subroutine intzgebal(fname)

c     [V,D]=balanc(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character JOB
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lZ)) return
         lhsvar(1) = 1
         lhsvar(2) = 2
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  

      if(.not.createvar(2,'d', N, N, lZ)) return
      if(.not.createvar(3,'d', 1, N, lSCALE)) return
      JOB='B'
      call ZGEBAL( JOB, N, zstk(lA), N, ILO, IHI,stk(lSCALE),INF0)


      call DLASET( 'F', N, N, ZERO, ONE, stk(lZ), N ) 
c     SUBROUTINE DGEBAK( JOB, SIDE, N, ILO, IHI, SCALE, M, V, LDV, INFO )
      call DGEBAK( JOB, 'R', N, ILO, IHI, stk(lSCALE), N,
     $     stk(lZ), N, INFO )

      lhsvar(1)=1
      lhsvar(2)=2
c     
      end


      logical function scischur(re,im)
      INCLUDE '../stack.h'
      logical scifunction, createcvar
      common /scisch/ lf, nx, nf
      integer iadr
      double precision re, im
c     
      iadr(l) = l+l-1
c     
      scischur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=re
      stk(lx+1)=im
      if(.not.scifunction(nx,lf,1,1)) return
c     stk(lx)=fct([re,im])  evaluated by scilab fct pointed to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scischur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scischur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scichk()
c     checks fct passed to schur
      INCLUDE '../stack.h'
      logical scifunction, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scisch/ lf, nx, nf
      iadr(l) = l+l-1
      scichk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.scifunction(nx,lf,1,1)) then
c     error into fct passed to schur (schur(A,tst))
         return
      endif
c     check return value of fct
      ilx=iadr(lx-2)
      if(istk(ilx).ne.1 .and. istk(ilx).ne.4) then
         err=nf
         call error(268)
         return
      endif
      if(istk(ilx+1)*istk(ilx+2).ne.1) then
         err=nf
         call error(268)
         return
      endif
      scichk=.true.
      return
      end


      subroutine intzgees0(fname)

c     [VS,T]=schur(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character  JOBVS, SORT
      integer SDIM
      logical voiddummy
      external voiddummy

      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
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
            if(.not.createvar(2, 'z', N, N, lVS)) return
            lhsvar(1) = 2
            lhsvar(2) = 1
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  
      if(.not.createvar(2,'z', N, 1, lW)) return
      k = 3
      if(lhs.gt.1) then
         if(.not.createvar(3,'z', N, N, lVS)) return
         k = 4
      endif
      if(.not.createvar(k,'d', N, 1, lRWORK)) return
      if(.not.createvar(k+1,'i', N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+2,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'z',1,LWORK,lDWORK)) return     
      if(lhs.eq.1) then
         JOBVS = 'N'
      else
         JOBVS = 'V'
      endif 
      SORT = 'N'

      call ZGEES( JOBVS, SORT, voiddummy, N, zstk(lA), N, SDIM,
     $     zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $     istk(lBWORK), INFO )
c     SUBROUTINE ZGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, W,
c     $  VS, LDVS, WORK, LWORK, RWORK, BWORK, INFO )
      if(info.gt.0) then
         call msgs(2,info)
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 1
      else
         lhsvar(1)=3
         lhsvar(2)=1
      endif
c     
      end


      subroutine intzgees1(fname)

c     [VS,T]=schur(A,'type')
c     type is complex

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character  TYPE, JOBVS, SORT
      integer SDIM
      logical voiddummy, WANTZ
      external voiddummy

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
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
            if(.not.createvar(2, 'z', N, N, lVS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  

      if(.not.getrhsvar(2,'c', M1, N1, lTYPE)) return
      TYPE = cstk(lTYPE:lTYPE)
      WANTZ = (TYPE.eq.'c' .or. TYPE.eq.'C')
      if(.not.WANTZ) then
         buf='schur'//': type must be complex'
         call error(998)
         return
      endif

      if(.not.createvar(3,'z', N, 1, lW)) return
      k = 4
      if(lhs.gt.1) then
         if(.not.createvar(4,'z', N, N, lVS)) return
         k = 5
      endif
      if(.not.createvar(k,'d', N, 1, lRWORK)) return
      if(.not.createvar(k+1,'i', N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+2,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'z',1,LWORK,lDWORK)) return     
      if(lhs.eq.1) then
         JOBVS = 'N'
      else
         JOBVS = 'V'
      endif 
      SORT = 'N'

      call ZGEES( JOBVS, SORT, voiddummy, N, zstk(lA), N, SDIM,
     $     zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $     istk(lBWORK), INFO )
c     SUBROUTINE ZGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, W,
c     $  VS, LDVS, WORK, LWORK, RWORK, BWORK, INFO )
      if(info.gt.0) then
         call msgs(2,info)
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 1
      else
         lhsvar(1)=4
         lhsvar(2)=1
      endif
c     
      end







      LOGICAL FUNCTION SELECTC( EIG )
C     
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C     
C     PURPOSE
C     
C     To select the stable eigenvalues in ordering the Schur form
C     of a matrix.
C     
C     ARGUMENTS
C     
C     Input/Output Parameters
C     
C     EIG    (input) COMPLEX*16
C     The  current eigenvalue considered.
C     
C     METHOD
C     
C     The function value SELECTC is set to .TRUE. for a stable
c     eigenvalue
C     and to .FALSE., otherwise.
C     
C     REFERENCES
C     
C     None.
C     
C     NUMERICAL ASPECTS
C     
C     None.
C     
C     CONTRIBUTOR
C     
C     
C     
C     REVISIONS
C     
C     -
C     
C     KEYWORDS
C     
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C     
C     ******************************************************************
C     
C     .. Parameters ..
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC         DREAL
C     .. Executable Statements ..
C     
      SELECTC = DREAL(EIG).LT.ZERO
C     
      RETURN
C     *** Last line of SELECTC ***
      END


      LOGICAL FUNCTION SELECTD( EIG )
C     
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C     
C     PURPOSE
C     
C     To select the eigenvalues inside the unit circle in ordering
C     the Schur form of a matrix.
C     
C     ARGUMENTS
C     
C     Input/Output Parameters
C     
C     EIG    (input) COMPLEX*16
C     The  current eigenvalue considered.
C     
C     METHOD
C     
C     The function value SELECTC is set to .TRUE. for an eigenvalue
c     which
C     is inside the unit circle and to .FALSE., otherwise.
C     
C     REFERENCES
C     
C     None.
C     
C     NUMERICAL ASPECTS
C     
C     None.
C     
C     CONTRIBUTOR
C     
C     
C     
C     REVISIONS
C     
C     -
C     
C     KEYWORDS
C     
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C     
C     ******************************************************************
C     
C     .. Parameters ..
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     .. Executable Statements ..
C     
      SELECTD = ABS(EIG).LT.ONE
C     
      RETURN
C     *** Last line of SELECTD ***
      END


      subroutine intzschur(fname)

c     [VS,dim]=zschur(A,function)
c     [VS,dim,T]=zschur(A,function)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      
      character fname*(*)
      character*4  JOBVS, SORT
      logical SCIZSCHUR,scizchk
      external SCIZSCHUR,scizchk
      common /scisch/ lf, nx, nf

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', M, N, lA)) return
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
            if(.not.createvar(2,'z', 0, 0, lSDIM)) return
            lhsvar(1)=1
            lhsvar(2)=2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', 0, 0, lSDIM)) return 
            stk(lSDIM)=0.0d0
            if(.not.createvar(3,'z', N, N, lVS)) return
            lhsvar(1)=1
            lhsvar(2)=2
            lhsvar(3)=3
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif

      if(.not.getrhsvar(2,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.1) then
         err=2
         call error(80)
         return
      endif
      if(.not.createvar(3,'z', N, 1, lW)) return
      k = 4
      if(lhs.gt.1) then
         if(.not.createvar(4,'z', N, N, lVS)) return
         k = 5
      endif
      if(.not.createvar(k,'i', 1, 1, lSDIM)) return 
      if(.not.createvar(k+1,'i',N, 1, lBWORK)) return
      if(.not.createvar(k+2,'d', N, 1, lRWORK)) return
      LWORKMIN = 3*N
      LWORK = LWORKMIN
      if(.not.createvar(k+3,'z',1,LWORKMIN,lDWORK)) return
      nx=k+4

      if(lhs.eq.1) then
         JOBVS = 'N'
         lVS = lDWORK
      else
         JOBVS = 'V'
      endif 
      SORT = 'S'

      if(.not.scizchk()) return
      call ZGEES( JOBVS, SORT, SCIZSCHUR, N, zstk(lA), N, istk(lSDIM),
     $     zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $     istk(lBWORK), INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(2,info)
         elseif(info.eq.N+1) then
            buf='eigenvalues could not be reordered (the problem '//
     $              'is very ill-conditioned'
            call error(1002)
         elseif(info.eq.N+2) then   
           call msgs(103,0) 
        endif
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 1
      else if(lhs.eq.2) then
         lhsvar(1)=4
         lhsvar(2)=5
      else if(lhs.eq.3) then 
         lhsvar(1)=4
         lhsvar(2)=5
         lhsvar(3)=1
      endif
c     
      end
      
      logical function scizschur(w)
      INCLUDE '../stack.h'
      logical scifunction, createcvar
      common /scisch/ lf, nx, nf
      integer iadr
      complex*16 w
      intrinsic dreal, dimag
c     
      iadr(l) = l+l-1
c     
      scizschur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=dreal(w)
      stk(lx+1)=dimag(w)
      if(.not.scifunction(nx,lf,1,1)) return
c     stk(lx)=fct([re,im])  evaluated by scilab fct pointed to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scizschur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scizschur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scizchk()
c     checks fct passed to schur
      INCLUDE '../stack.h'
      logical scifunction, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scisch/ lf, nx, nf
      iadr(l) = l+l-1
      scizchk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.scifunction(nx,lf,1,1)) then
c     error into fct passed to schur (schur(A,tst))
         return
      endif
c     check return value of fct
      ilx=iadr(lx-2)
      if(istk(ilx).ne.1 .and. istk(ilx).ne.4) then
         err=nf
         call error(268)
         return
      endif
      if(istk(ilx+1)*istk(ilx+2).ne.1) then
         err=nf
         call error(268)
         return
      endif
      scizchk=.true.
      return
      end

      logical function voiddummy(ar,ai)

      voiddummy = .false.
      return
      end


      subroutine intgschur(fname)

c     [As,Bs,VSR,dim]=gschur(A,B,"function")
c     [VSR,dim]=gschur(A,B,"function")

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBVSL, JOBVSR, SORT
      integer gshsel
      external gshsel
      
      minrhs=3
      maxrhs=3
      minlhs=1
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 
      
      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif

      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         if(lhs.eq.2) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
         elseif(lhs.eq.3) then
            if(.not.createvar(3,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
         else
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(3,'c', mr, mc, lc)) return
      call setgshsel(mr*mc, cstk(lc:lc+mr*mc),irep)
      if ( irep.eq.1) then 
         buf = cstk(lc:lc+mr*mc)
         call error(50)
         return
      endif

      if(.not.createvar(4,'i', 1, 1, lSDIM)) return
      if(.not.createvar(5,'d', N, 1, lALPHAR)) return
      if(.not.createvar(6,'d', N, 1, lALPHAI)) return
      if(.not.createvar(7,'d', N, 1, lBETA)) return              
      if(.not.createvar(8,'d', N, N, lVSL)) return
      if(.not.createvar(9,'d', N, N, lVSR)) return
      if(.not.createvar(10,'i', 2*N, 1, lBWORK)) return

      LWORKMIN = 8*N+16
      LWORK=maxvol(11,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(11,'d',1,LWORK,lDWORK)) return

      JOBVSL = 'V'
      JOBVSR = 'V'
      SORT = 'S'
c     
      call DGGES( JOBVSL, JOBVSR, SORT, gshsel, N, stk(lA), N, stk(lB),
     $     N, istk(lSDIM), stk(lALPHAR), stk(lALPHAI), stk(lBETA),
     $     stk(lVSL), N, stk(lVSR), N, stk(lDWORK), LWORK, istk(lBWORK),
     $     INFO )
c     SUBROUTINE DGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $     SDIM, ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR, WORK,
c     $     LWORK, BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         elseif(info.eq.N+2) then 
            call msgs(103,0)
         elseif(info.eq.N+3) then 
            buf='reordering failed'
            call error(1002)
            return
         endif
      endif

      if(lhs.eq.1) then
         lhsvar(1)=4
      elseif(lhs.eq.2) then
         lhsvar(1)=9
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=8
         lhsvar(2)=9
         lhsvar(3)=4
      else if(lhs.eq.4) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=9
         lhsvar(4)=4
      endif

      end

      subroutine intzgschur(fname)


c     [As,Bs,VSR,dim]=gschur(A,B,'function')
c     [VSR,dim]=gschur(A,B,'function')

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      

      character fname*(*)
      character  JOBVSL, JOBVSR, SORT
      integer gzhsel
      external gzhsel

      minrhs=3
      maxrhs=3
      minlhs=1
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 


      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'z', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.NB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         if(lhs.eq.2) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
         elseif(lhs.eq.3) then
            if(.not.createvar(3,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
         else
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(3,'c', mr, mc, lc)) return
      call setgzhsel(mr*mc, cstk(lc:lc+mr*mc),irep)
      if ( irep.eq.1) then 
         buf = cstk(lc:lc+mr*mc)
         call error(50)
         return
      endif
      if(.not.createvar(4,'i', 1, 1, lSDIM)) return
      if(.not.createvar(5,'z', N, 1, lALPHA)) return
      if(.not.createvar(6,'z', N, 1, lBETA)) return
      if(.not.createvar(7,'z', N, N, lVSL)) return
      if(.not.createvar(8,'z', N, N, lVSR)) return
      if(.not.createvar(9,'d', 8*N, 1, lRWORK)) return
      if(.not.createvar(10,'i', 2*N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(11,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(11,'z',1,LWORK,lDWORK)) return

      JOBVSL = 'V'
      JOBVSR = 'V' 
      SORT = 'S'

      call ZGGES( JOBVSL, JOBVSR, SORT, gzhsel, N, zstk(lA), N,
     $     zstk(lB), N, istk(lSDIM), zstk(lALPHA), zstk(lBETA),
     $     zstk(lVSL), N, zstk(lVSR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), istk(lBWORK), INFO )
c     SUBROUTINE ZGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $    SDIM, ALPHA, BETA, VSL, LDVSL, VSR, LDVSR, WORK, LWORK, RWORK
c     ,
c     $    BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         elseif(info.eq.N+2) then 
            call msgs(103,0)
         elseif(info.eq.N+3) then 
            buf='reordering failed'
            call error(1002)
            return
         endif
      endif

      if(lhs.eq.1) then
         lhsvar(1)=4
      elseif(lhs.eq.2) then
         lhsvar(1)=8
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=7
         lhsvar(2)=8
         lhsvar(3)=4
      else if(lhs.eq.4) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=8
         lhsvar(4)=4
      endif

      end



      
      logical function scigshur(alphar,alphai,beta)
      INCLUDE '../stack.h'
      logical scifunction, createvar, createcvar
      common /scigsch/ lf, nx, nf
      integer iadr
      double precision alphar, alphai, beta
c     
      iadr(l) = l+l-1
c     
      scigshur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=alphar
      stk(lx+1)=alphai
      if(.not.createvar(nx+1,'d',1,1,lb)) return
      stk(lb)=beta
      if(.not.scifunction(nx,lf,1,2)) return
c     stk(lx)=fct([alphar,alphai,beta])  evaluated by scilab fct pointed
c     to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scigshur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scigshur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scigchk()
c     checks fct passed to gschur
      INCLUDE '../stack.h'
      logical scifunction, createvar, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scigsch/ lf, nx, nf
      iadr(l) = l+l-1

      scigchk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.createvar(nx+1,'d',1,1,lb)) return
      stk(lb)=1.0d0
      if(.not.scifunction(nx,lf,1,2)) then
c     error into fct passed to gschur (gschur(A,B,tst))
         return
      endif
c     check return value of fct
      ilx=iadr(lx-2)
      if(istk(ilx).ne.1 .and. istk(ilx).ne.4) then
         err=nf
         call error(268)
         return
      endif
      if(istk(ilx+1)*istk(ilx+2).ne.1) then
         err=nf
         call error(268)
         return
      endif
      scigchk=.true.
      return
      end

      subroutine intdoldsvd(tol,fname)

c     [U,S,V,rk]=svd(A,tol)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      double precision tol,eps,dlamch
      parameter ( ZERO=0.0D0 )

      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', M, N, lA)) return
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
         else if(lhs.eq.4) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            if(.not.createvar(4,'d', 1, 1, lrk)) return
            stk(lrk)=0
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            lhsvar(4) = 4
            return
         endif
      elseif(N.eq.-1.or.M.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'d', N, N, lV)) return
         if(.not.createvar(6,'d', N, N, lVT)) return
         if(.not.createvar(7,'d', 1, 1, lrk)) return
         k = 8
      endif
      LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
         JOBU = 'A'
         JOBVT = 'A'
      endif
      if(lhs.eq.1) then
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $        M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
c     next lines to patch an error of DGESVD
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $        M, stk(lVT), N, stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      endif  
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

c     calculating rk = # of sv's > tol 

      eps=dlamch('eps')
      if(tol.eq.0.0d0) tol=dble(max(M,N))*eps*stk(lSV)
      irang=0
      do 51 i = 1, min(M,N)
         if(stk(lSV+i-1).gt.tol) irang=i
 51   continue
      stk(lrk)=dble(irang)
      if(lhs.gt.1) then
         call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*M
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, N
            do 20 i = j, N
               ij = i+(j-1)*N
               ji = j+(i-1)*N
               stk(lV+ij-1) = stk(lVT+ji-1)
               stk(lV+ji-1) = stk(lVT+ij-1)
 20         continue
 30      continue     
      endif

      if(lhs.eq.1) then
         lhsvar(1) = 2
      else 
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
         lhsvar(4)=7
      endif
c     
      end

      subroutine intzoldsvd(tol,fname)

c     [U,S,V,rk]=svd(A,tol)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      
      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      double precision tol,eps,dlamch
      parameter ( ZERO=0.0D0 )
      intrinsic conjg

      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=4
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
         else if(lhs.eq.4) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            if(.not.createvar(4,'d', 1, 1, lrk)) return
            stk(lrk)=0
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            lhsvar(4) = 4
            return
         endif
      elseif(N.eq.-1.or.M.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
      k = 3              
      if(lhs.gt.1) then
         if(.not.createvar(3,'z', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'z', N, N, lV)) return
         if(.not.createvar(6,'z', N, N, lVT)) return
         if(.not.createvar(7,'d', 1, 1, lrk)) return
         k = 8
      endif
      LRWRK = max(3*min(M,N),5*min(M,N)-4)
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
         JOBU = 'A'
         JOBVT = 'A'
      endif
      if(lhs.eq.1) then
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), 
     $        zstk(lA), M, zstk(lA), N, zstk(lDWORK), LWORK, 
     $        stk(lRWORK), INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
         do 01 ii = 0,min(M,N)-1
            stk(lSV+ii)=abs(stk(lSV+ii))
 01      continue
         call DLASRT('D', min(M,N), stk(lSV), INFO )
      else
         call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), 
     $        zstk(lU), M, zstk(lVT), N, zstk(lDWORK), LWORK,
     $        stk(lRWORK), INFO )
c     SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      endif 
      if(info.ne.0) then
         if(info.gt.0) call error(24)
         return
      endif

c     calculating rk = # of sv's > tol 

      eps=dlamch('eps')
      if(tol.eq.0.0d0) tol=dble(max(M,N))*eps*stk(lSV)
      irang=0
      do 51 i = 1, min(M,N)
         if(stk(lSV+i-1).gt.tol) irang=i
 51   continue
      stk(lrk)=dble(irang)

      if(lhs.gt.1) then
         call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, min(M,N)
            ii = i+(i-1)*M
            stk(lS+ii-1) = stk(lSV+i-1)
 10      continue        
         do 30 j = 1, N
            do 20 i = j, N
               ij = i+(j-1)*N
               ji = j+(i-1)*N
               zstk(lV+ij-1) = conjg(zstk(lVT+ji-1))
               zstk(lV+ji-1) = conjg(zstk(lVT+ij-1))
 20         continue
 30      continue     
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 2
      else 
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
         lhsvar(4)=7
      endif
c     
      end

      SUBROUTINE DGELSY1( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND, RANK,
     $     WORK, LWORK, INFO )
*     
*     -- LAPACK driver routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 30, 1999
*     
*     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDB, LWORK, M, N, NRHS, RANK
      DOUBLE PRECISION   RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), WORK( * )
*     ..
*     
*     Purpose
*     =======
*     
*     DGELSY1 computes a solution, with at least N-RANK zeros to a real 
*     linear least squares problem:
*     minimize || A * X - B ||
*     using a complete orthogonal factorization of A.  A is an M-by-N
*     matrix which may be rank-deficient.
*     
*     Several right hand side vectors b and solution vectors x can be
*     handled in a single call; they are stored as the columns of the
*     M-by-NRHS right hand side matrix B and the N-by-NRHS solution
*     matrix X.
*     
*     The routine first computes a QR factorization with column
c     pivoting:
*     A * P = Q * [ R11 R12 ]
*                 [  0  R22 ]
*     with R11 defined as the largest leading submatrix whose estimated
*     condition number is less than 1/RCOND.  The order of R11, RANK,
*     is the effective rank of A.
*     
*     Then, R22 is considered to be negligible, 
*     The  solution return is then
*     X = P * [ inv(R11)*Q1'*B ]
*             [        0       ]
*     where Q1 consists of the first RANK columns of Q.
*     
*     This routine is basically identical to the original xGELSX except
*     three differences:
*     o The call to the subroutine xGEQPF has been substituted by the
*     the call to the subroutine xGEQP3. This subroutine is a Blas-3
*     version of the QR factorization with column pivoting.
*     o Matrix B (the right hand side) is updated with Blas-3.
*     o The permutation of matrix B (the right hand side) is faster and
*     more simple.
*     
*     Arguments
*     =========
*     
*     M       (input) INTEGER
*     The number of rows of the matrix A.  M >= 0.
*     
*     N       (input) INTEGER
*     The number of columns of the matrix A.  N >= 0.
*     
*     NRHS    (input) INTEGER
*     The number of right hand sides, i.e., the number of
*     columns of matrices B and X. NRHS >= 0.
*     
*     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
*     On entry, the M-by-N matrix A.
*     On exit, A has been overwritten by details of its
*     complete orthogonal factorization.
*     
*     LDA     (input) INTEGER
*     The leading dimension of the array A.  LDA >= max(1,M).
*     
*     B       (input/output) DOUBLE PRECISION array, dimension (LDB,NRHS
c     )
*     On entry, the M-by-NRHS right hand side matrix B.
*     On exit, the N-by-NRHS solution matrix X.
*     
*     LDB     (input) INTEGER
*     The leading dimension of the array B. LDB >= max(1,M,N).
*     
*     JPVT    (input/output) INTEGER array, dimension (N)
*     On entry, if JPVT(i) .ne. 0, the i-th column of A is permuted
*     to the front of AP, otherwise column i is a free column.
*     On exit, if JPVT(i) = k, then the i-th column of AP
*     was the k-th column of A.
*     
*     RCOND   (input) DOUBLE PRECISION
*     RCOND is used to determine the effective rank of A, which
*     is defined as the order of the largest leading triangular
*     submatrix R11 in the QR factorization with pivoting of A,
*     whose estimated condition number < 1/RCOND.
*     
*     RANK    (output) INTEGER
*     The effective rank of A, i.e., the order of the submatrix
*     R11.  This is the same as the order of the submatrix T11
*     in the complete orthogonal factorization of A.
*     
*     WORK    (workspace/output) DOUBLE PRECISION array, dimension
c     (LWORK)
*     On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
*     
*     LWORK   (input) INTEGER
*     The dimension of the array WORK.
*     The unblocked strategy requires that:
*     LWORK >= MAX( MN+3*N+1, 2*MN+NRHS ),
*     where MN = min( M, N ).
*     The block algorithm requires that:
*     LWORK >= MAX( MN+2*N+NB*(N+1), 2*MN+NB*NRHS ),
*     where NB is an upper bound on the blocksize returned
*     by ILAENV for the routines DGEQP3, DTZRZF, STZRQF, DORMQR,
*     and DORMRZ.
*     
*     If LWORK = -1, then a workspace query is assumed; the routine
*     only calculates the optimal size of the WORK array, returns
*     this value as the first entry of the WORK array, and no error
*     message related to LWORK is issued by XERBLA.
*     
*     INFO    (output) INTEGER
*     = 0: successful exit
*     < 0: If INFO = -i, the i-th argument had an illegal value.
*     
*     Further Details
*     ===============
*     
*     Based on contributions by
*     A. Petitet, Computer Science Dept., Univ. of Tenn., Knoxville, USA
*     E. Quintana-Orti, Depto. de Informatica, Universidad Jaime I,
c     Spain
*     G. Quintana-Orti, Depto. de Informatica, Universidad Jaime I,
c     Spain
*     
*     ==================================================================
c     ===
*     
*     .. Parameters ..
      INTEGER            IMAX, IMIN
      PARAMETER          ( IMAX = 1, IMIN = 2 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LQUERY
      INTEGER            I, IASCL, IBSCL, ISMAX, ISMIN, J, LWKOPT, MN,
     $     NB, NB1, NB2, NB3, NB4
      DOUBLE PRECISION   ANRM, BIGNUM, BNRM, C1, C2, S1, S2, SMAX,
     $     SMAXPR, SMIN, SMINPR, SMLNUM, WSIZE
*     ..
*     .. External Functions ..
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH, DLANGE
      EXTERNAL           ILAENV, DLAMCH, DLANGE
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEQP3, DLABAD, DLAIC1, DLASCL, DLASET,
     $     DORMQR, DORMRZ, DTRSM, DTZRZF, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX, MIN
*     ..
*     .. Executable Statements ..
*     
      MN = MIN( M, N )
      ISMIN = MN + 1
      ISMAX = 2*MN + 1
*     
*     Test the input arguments.
*     
      INFO = 0
      NB1 = ILAENV( 1, 'DGEQRF', ' ', M, N, -1, -1 )
      NB2 = ILAENV( 1, 'DGERQF', ' ', M, N, -1, -1 )
      NB3 = ILAENV( 1, 'DORMQR', ' ', M, N, NRHS, -1 )
      NB4 = ILAENV( 1, 'DORMRQ', ' ', M, N, NRHS, -1 )
      NB = MAX( NB1, NB2, NB3, NB4 )
      LWKOPT = MAX( 1, MN+2*N+NB*( N+1 ), 2*MN+NB*NRHS )
      WORK( 1 ) = DBLE( LWKOPT )
      LQUERY = ( LWORK.EQ.-1 )
      IF( M.LT.0 ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( NRHS.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, M, N ) ) THEN
         INFO = -7
      ELSE IF( LWORK.LT.MAX( 1, MN+3*N+1, 2*MN+NRHS ) .AND. .NOT.
     $        LQUERY ) THEN
         INFO = -12
      END IF
*     
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DGELSY', -INFO )
         RETURN
      ELSE IF( LQUERY ) THEN
         RETURN
      END IF
*     
*     Quick return if possible
*     
      IF( MIN( M, N, NRHS ).EQ.0 ) THEN
         RANK = 0
         RETURN
      END IF
*     
*     Get machine parameters
*     
      SMLNUM = DLAMCH( 'S' ) / DLAMCH( 'P' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
*     
*     Scale A, B if max entries outside range [SMLNUM,BIGNUM]
*     
      ANRM = DLANGE( 'M', M, N, A, LDA, WORK )
      IASCL = 0
      IF( ANRM.GT.ZERO .AND. ANRM.LT.SMLNUM ) THEN
*     
*     Scale matrix norm up to SMLNUM
*     
         CALL DLASCL( 'G', 0, 0, ANRM, SMLNUM, M, N, A, LDA, INFO )
         IASCL = 1
      ELSE IF( ANRM.GT.BIGNUM ) THEN
*     
*     Scale matrix norm down to BIGNUM
*     
         CALL DLASCL( 'G', 0, 0, ANRM, BIGNUM, M, N, A, LDA, INFO )
         IASCL = 2
      ELSE IF( ANRM.EQ.ZERO ) THEN
*     
*     Matrix all zero. Return zero solution.
*     
         CALL DLASET( 'F', MAX( M, N ), NRHS, ZERO, ZERO, B, LDB )
         RANK = 0
         GO TO 70
      END IF
*     
      BNRM = DLANGE( 'M', M, NRHS, B, LDB, WORK )
      IBSCL = 0
      IF( BNRM.GT.ZERO .AND. BNRM.LT.SMLNUM ) THEN
*     
*     Scale matrix norm up to SMLNUM
*     
         CALL DLASCL( 'G', 0, 0, BNRM, SMLNUM, M, NRHS, B, LDB, INFO )
         IBSCL = 1
      ELSE IF( BNRM.GT.BIGNUM ) THEN
*     
*     Scale matrix norm down to BIGNUM
*     
         CALL DLASCL( 'G', 0, 0, BNRM, BIGNUM, M, NRHS, B, LDB, INFO )
         IBSCL = 2
      END IF
*     
*     Compute QR factorization with column pivoting of A:
*     A * P = Q * R
*     
      CALL DGEQP3( M, N, A, LDA, JPVT, WORK( 1 ), WORK( MN+1 ),
     $     LWORK-MN, INFO )
      WSIZE = MN + WORK( MN+1 )
*     
*     workspace: MN+2*N+NB*(N+1).
*     Details of Householder rotations stored in WORK(1:MN).
*     
*     Determine RANK using incremental condition estimation
*     
      WORK( ISMIN ) = ONE
      WORK( ISMAX ) = ONE
      SMAX = ABS( A( 1, 1 ) )
      SMIN = SMAX
      IF( ABS( A( 1, 1 ) ).EQ.ZERO ) THEN
         RANK = 0
         CALL DLASET( 'F', MAX( M, N ), NRHS, ZERO, ZERO, B, LDB )
         GO TO 70
      ELSE
         RANK = 1
      END IF
*     
 10   CONTINUE
      IF( RANK.LT.MN ) THEN
         I = RANK + 1
         CALL DLAIC1( IMIN, RANK, WORK( ISMIN ), SMIN, A( 1, I ),
     $        A( I, I ), SMINPR, S1, C1 )
         CALL DLAIC1( IMAX, RANK, WORK( ISMAX ), SMAX, A( 1, I ),
     $        A( I, I ), SMAXPR, S2, C2 )
*     
         IF( SMAXPR*RCOND.LE.SMINPR ) THEN
            DO 20 I = 1, RANK
               WORK( ISMIN+I-1 ) = S1*WORK( ISMIN+I-1 )
               WORK( ISMAX+I-1 ) = S2*WORK( ISMAX+I-1 )
 20         CONTINUE
            WORK( ISMIN+RANK ) = C1
            WORK( ISMAX+RANK ) = C2
            SMIN = SMINPR
            SMAX = SMAXPR
            RANK = RANK + 1
            GO TO 10
         END IF
      END IF
*     
*     workspace: 3*MN.
*     
*     Logically partition R = [ R11 R12 ]
*     [  0  R22 ]
*     where R11 = R(1:RANK,1:RANK)
*     
c     *     [R11,R12] = [ T11, 0 ] * Y
c     *
c     IF( RANK.LT.N )
c     $   CALL DTZRZF( RANK, N, A, LDA, WORK( MN+1 ), WORK( 2*MN+1 ),
c     $                LWORK-2*MN, INFO )
c     *
c     *     workspace: 2*MN.
*     Details of Householder rotations stored in WORK(MN+1:2*MN)
*     
*     B(1:M,1:NRHS) := Q' * B(1:M,1:NRHS)
*     
      CALL DORMQR( 'Left', 'Transpose', M, NRHS, MN, A, LDA, WORK( 1 ),
     $     B, LDB, WORK( 2*MN+1 ), LWORK-2*MN, INFO )
      WSIZE = MAX( WSIZE, 2*MN+WORK( 2*MN+1 ) )
*     
*     workspace: 2*MN+NB*NRHS.
*     
*     B(1:RANK,1:NRHS) := inv(T11) * B(1:RANK,1:NRHS)
*     
      CALL DTRSM( 'Left', 'Upper', 'No transpose', 'Non-unit', RANK,
     $     NRHS, ONE, A, LDA, B, LDB )
*     
      DO 40 J = 1, NRHS
         DO 30 I = RANK + 1, N
            B( I, J ) = ZERO
 30      CONTINUE
 40   CONTINUE
c     *
c     *     B(1:N,1:NRHS) := Y' * B(1:N,1:NRHS)
c     *
c     IF( RANK.LT.N ) THEN
c     CALL DORMRZ( 'Left', 'Transpose', N, NRHS, RANK, N-RANK, A,
c     $                LDA, WORK( MN+1 ), B, LDB, WORK( 2*MN+1 ),
c     $                LWORK-2*MN, INFO )
c     END IF
c     *
c     *     workspace: 2*MN+NRHS.
*     
*     B(1:N,1:NRHS) := P * B(1:N,1:NRHS)
*     
      DO 60 J = 1, NRHS
         DO 50 I = 1, N
            WORK( JPVT( I ) ) = B( I, J )
 50      CONTINUE
         CALL DCOPY( N, WORK( 1 ), 1, B( 1, J ), 1 )
 60   CONTINUE
*     
*     workspace: N.
*     
*     Undo scaling
*     
      IF( IASCL.EQ.1 ) THEN
         CALL DLASCL( 'G', 0, 0, ANRM, SMLNUM, N, NRHS, B, LDB, INFO )
         CALL DLASCL( 'U', 0, 0, SMLNUM, ANRM, RANK, RANK, A, LDA,
     $        INFO )
      ELSE IF( IASCL.EQ.2 ) THEN
         CALL DLASCL( 'G', 0, 0, ANRM, BIGNUM, N, NRHS, B, LDB, INFO )
         CALL DLASCL( 'U', 0, 0, BIGNUM, ANRM, RANK, RANK, A, LDA,
     $        INFO )
      END IF
      IF( IBSCL.EQ.1 ) THEN
         CALL DLASCL( 'G', 0, 0, SMLNUM, BNRM, N, NRHS, B, LDB, INFO )
      ELSE IF( IBSCL.EQ.2 ) THEN
         CALL DLASCL( 'G', 0, 0, BIGNUM, BNRM, N, NRHS, B, LDB, INFO )
      END IF
*     
 70   CONTINUE
      WORK( 1 ) = DBLE( LWKOPT )
*     
      RETURN
*     
*     End of DGELSY
*     
      END
      SUBROUTINE ZGELSY1( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND, RANK,
     $                   WORK, LWORK, RWORK, INFO )
*
*  -- LAPACK driver routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 30, 1999
*
*     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDB, LWORK, M, N, NRHS, RANK
      DOUBLE PRECISION   RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   RWORK( * )
      COMPLEX*16         A( LDA, * ), B( LDB, * ), WORK( * )
*     ..
*
*  Purpose
*  =======
*
*  ZGELSY1 computes a solution with at least N-RANK zeros to a complex 
*  linear leastsquares problem:
*      minimize || A * X - B ||
*  using a complete orthogonal factorization of A.  A is an M-by-N
*  matrix which may be rank-deficient.
*
*  Several right hand side vectors b and solution vectors x can be
*  handled in a single call; they are stored as the columns of the
*  M-by-NRHS right hand side matrix B and the N-by-NRHS solution
*  matrix X.
*
*  The routine first computes a QR factorization with column pivoting:
*      A * P = Q * [ R11 R12 ]
*                  [  0  R22 ]
*  with R11 defined as the largest leading submatrix whose estimated
*  condition number is less than 1/RCOND.  The order of R11, RANK,
*  is the effective rank of A.
*
*  Then, R22 is considered to be negligible
*  The returned  solution is then
*     X = P *  [ inv(T11)*Q1'*B ]
*              [        0       ]
*  where Q1 consists of the first RANK columns of Q.
*
*  This routine is basically identical to the original xGELSX except
*  three differences:
*    o The permutation of matrix B (the right hand side) is faster and
*      more simple.
*    o The call to the subroutine xGEQPF has been substituted by the
*      the call to the subroutine xGEQP3. This subroutine is a Blas-3
*      version of the QR factorization with column pivoting.
*    o Matrix B (the right hand side) is updated with Blas-3.
*
*  Arguments
*  =========
*
*  M       (input) INTEGER
*          The number of rows of the matrix A.  M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A.  N >= 0.
*
*  NRHS    (input) INTEGER
*          The number of right hand sides, i.e., the number of
*          columns of matrices B and X. NRHS >= 0.
*
*  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
*          On entry, the M-by-N matrix A.
*          On exit, A has been overwritten by details of its
*          complete orthogonal factorization.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1,M).
*
*  B       (input/output) COMPLEX*16 array, dimension (LDB,NRHS)
*          On entry, the M-by-NRHS right hand side matrix B.
*          On exit, the N-by-NRHS solution matrix X.
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B. LDB >= max(1,M,N).
*
*  JPVT    (input/output) INTEGER array, dimension (N)
*          On entry, if JPVT(i) .ne. 0, the i-th column of A is permuted
*          to the front of AP, otherwise column i is a free column.
*          On exit, if JPVT(i) = k, then the i-th column of A*P
*          was the k-th column of A.
*
*  RCOND   (input) DOUBLE PRECISION
*          RCOND is used to determine the effective rank of A, which
*          is defined as the order of the largest leading triangular
*          submatrix R11 in the QR factorization with pivoting of A,
*          whose estimated condition number < 1/RCOND.
*
*  RANK    (output) INTEGER
*          The effective rank of A, i.e., the order of the submatrix
*          R11.  This is the same as the order of the submatrix T11
*          in the complete orthogonal factorization of A.
*
*  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          The unblocked strategy requires that:
*            LWORK >= MN + MAX( 2*MN, N+1, MN+NRHS )
*          where MN = min(M,N).
*          The block algorithm requires that:
*            LWORK >= MN + MAX( 2*MN, NB*(N+1), MN+MN*NB, MN+NB*NRHS )
*          where NB is an upper bound on the blocksize returned
*          by ILAENV for the routines ZGEQP3, ZTZRZF, CTZRQF, ZUNMQR,
*          and ZUNMRZ.
*
*          If LWORK = -1, then a workspace query is assumed; the routine
*          only calculates the optimal size of the WORK array, returns
*          this value as the first entry of the WORK array, and no error
*          message related to LWORK is issued by XERBLA.
*
*  RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
*  Further Details
*  ===============
*
*  Based on contributions by
*    A. Petitet, Computer Science Dept., Univ. of Tenn., Knoxville, USA
*    E. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain
*    G. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain
*
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            IMAX, IMIN
      PARAMETER          ( IMAX = 1, IMIN = 2 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      COMPLEX*16         CZERO, CONE
      PARAMETER          ( CZERO = ( 0.0D+0, 0.0D+0 ),
     $                   CONE = ( 1.0D+0, 0.0D+0 ) )
*     ..
*     .. Local Scalars ..
      LOGICAL            LQUERY
      INTEGER            I, IASCL, IBSCL, ISMAX, ISMIN, J, LWKOPT, MN,
     $                   NB, NB1, NB2, NB3, NB4
      DOUBLE PRECISION   ANRM, BIGNUM, BNRM, SMAX, SMAXPR, SMIN, SMINPR,
     $                   SMLNUM, WSIZE
      COMPLEX*16         C1, C2, S1, S2
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLABAD, XERBLA, ZCOPY, ZGEQP3, ZLAIC1, ZLASCL,
     $                   ZLASET, ZTRSM, ZTZRZF, ZUNMQR, ZUNMRZ
*     ..
*     .. External Functions ..
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH, ZLANGE
      EXTERNAL           ILAENV, DLAMCH, ZLANGE
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, DCMPLX, MAX, MIN
*     ..
*     .. Executable Statements ..
*
      MN = MIN( M, N )
      ISMIN = MN + 1
      ISMAX = 2*MN + 1
*
*     Test the input arguments.
*
      INFO = 0
      NB1 = ILAENV( 1, 'ZGEQRF', ' ', M, N, -1, -1 )
      NB2 = ILAENV( 1, 'ZGERQF', ' ', M, N, -1, -1 )
      NB3 = ILAENV( 1, 'ZUNMQR', ' ', M, N, NRHS, -1 )
      NB4 = ILAENV( 1, 'ZUNMRQ', ' ', M, N, NRHS, -1 )
      NB = MAX( NB1, NB2, NB3, NB4 )
      LWKOPT = MAX( 1, MN+2*N+NB*( N+1 ), 2*MN+NB*NRHS )
      WORK( 1 ) = DCMPLX( LWKOPT )
      LQUERY = ( LWORK.EQ.-1 )
      IF( M.LT.0 ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( NRHS.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, M, N ) ) THEN
         INFO = -7
      ELSE IF( LWORK.LT.( MN+MAX( 2*MN, N+1, MN+NRHS ) ) .AND. .NOT.
     $         LQUERY ) THEN
         INFO = -12
      END IF
*
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'ZGELSY', -INFO )
         RETURN
      ELSE IF( LQUERY ) THEN
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( MIN( M, N, NRHS ).EQ.0 ) THEN
         RANK = 0
         RETURN
      END IF
*
*     Get machine parameters
*
      SMLNUM = DLAMCH( 'S' ) / DLAMCH( 'P' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
*
*     Scale A, B if max entries outside range [SMLNUM,BIGNUM]
*
      ANRM = ZLANGE( 'M', M, N, A, LDA, RWORK )
      IASCL = 0
      IF( ANRM.GT.ZERO .AND. ANRM.LT.SMLNUM ) THEN
*
*        Scale matrix norm up to SMLNUM
*
         CALL ZLASCL( 'G', 0, 0, ANRM, SMLNUM, M, N, A, LDA, INFO )
         IASCL = 1
      ELSE IF( ANRM.GT.BIGNUM ) THEN
*
*        Scale matrix norm down to BIGNUM
*
         CALL ZLASCL( 'G', 0, 0, ANRM, BIGNUM, M, N, A, LDA, INFO )
         IASCL = 2
      ELSE IF( ANRM.EQ.ZERO ) THEN
*
*        Matrix all zero. Return zero solution.
*
         CALL ZLASET( 'F', MAX( M, N ), NRHS, CZERO, CZERO, B, LDB )
         RANK = 0
         GO TO 70
      END IF
*
      BNRM = ZLANGE( 'M', M, NRHS, B, LDB, RWORK )
      IBSCL = 0
      IF( BNRM.GT.ZERO .AND. BNRM.LT.SMLNUM ) THEN
*
*        Scale matrix norm up to SMLNUM
*
         CALL ZLASCL( 'G', 0, 0, BNRM, SMLNUM, M, NRHS, B, LDB, INFO )
         IBSCL = 1
      ELSE IF( BNRM.GT.BIGNUM ) THEN
*
*        Scale matrix norm down to BIGNUM
*
         CALL ZLASCL( 'G', 0, 0, BNRM, BIGNUM, M, NRHS, B, LDB, INFO )
         IBSCL = 2
      END IF
*
*     Compute QR factorization with column pivoting of A:
*        A * P = Q * R
*
      CALL ZGEQP3( M, N, A, LDA, JPVT, WORK( 1 ), WORK( MN+1 ),
     $             LWORK-MN, RWORK, INFO )
      WSIZE = MN + DBLE( WORK( MN+1 ) )
*
*     complex workspace: MN+NB*(N+1). real workspace 2*N.
*     Details of Householder rotations stored in WORK(1:MN).
*
*     Determine RANK using incremental condition estimation
*
      WORK( ISMIN ) = CONE
      WORK( ISMAX ) = CONE
      SMAX = ABS( A( 1, 1 ) )
      SMIN = SMAX
      IF( ABS( A( 1, 1 ) ).EQ.ZERO ) THEN
         RANK = 0
         CALL ZLASET( 'F', MAX( M, N ), NRHS, CZERO, CZERO, B, LDB )
         GO TO 70
      ELSE
         RANK = 1
      END IF
*
   10 CONTINUE
      IF( RANK.LT.MN ) THEN
         I = RANK + 1
         CALL ZLAIC1( IMIN, RANK, WORK( ISMIN ), SMIN, A( 1, I ),
     $                A( I, I ), SMINPR, S1, C1 )
         CALL ZLAIC1( IMAX, RANK, WORK( ISMAX ), SMAX, A( 1, I ),
     $                A( I, I ), SMAXPR, S2, C2 )
*
         IF( SMAXPR*RCOND.LE.SMINPR ) THEN
            DO 20 I = 1, RANK
               WORK( ISMIN+I-1 ) = S1*WORK( ISMIN+I-1 )
               WORK( ISMAX+I-1 ) = S2*WORK( ISMAX+I-1 )
   20       CONTINUE
            WORK( ISMIN+RANK ) = C1
            WORK( ISMAX+RANK ) = C2
            SMIN = SMINPR
            SMAX = SMAXPR
            RANK = RANK + 1
            GO TO 10
         END IF
      END IF
*
*     complex workspace: 3*MN.
*
*     Logically partition R = [ R11 R12 ]
*                             [  0  R22 ]
*     where R11 = R(1:RANK,1:RANK)
*
c*     [R11,R12] = [ T11, 0 ] * Y
c*
c      IF( RANK.LT.N )
c     $   CALL ZTZRZF( RANK, N, A, LDA, WORK( MN+1 ), WORK( 2*MN+1 ),
c     $                LWORK-2*MN, INFO )
c*
*     complex workspace: 2*MN.
*     Details of Householder rotations stored in WORK(MN+1:2*MN)
*
*     B(1:M,1:NRHS) := Q' * B(1:M,1:NRHS)
*
      CALL ZUNMQR( 'Left', 'Conjugate transpose', M, NRHS, MN, A, LDA,
     $             WORK( 1 ), B, LDB, WORK( 2*MN+1 ), LWORK-2*MN, INFO )
      WSIZE = MAX( WSIZE, 2*MN+DBLE( WORK( 2*MN+1 ) ) )
*
*     complex workspace: 2*MN+NB*NRHS.
*
*     B(1:RANK,1:NRHS) := inv(T11) * B(1:RANK,1:NRHS)
*
      CALL ZTRSM( 'Left', 'Upper', 'No transpose', 'Non-unit', RANK,
     $            NRHS, CONE, A, LDA, B, LDB )
*
      DO 40 J = 1, NRHS
         DO 30 I = RANK + 1, N
            B( I, J ) = CZERO
   30    CONTINUE
   40 CONTINUE
c*
c*     B(1:N,1:NRHS) := Y' * B(1:N,1:NRHS)
c*
c      IF( RANK.LT.N ) THEN
c         CALL ZUNMRZ( 'Left', 'Conjugate transpose', N, NRHS, RANK,
c     $                N-RANK, A, LDA, WORK( MN+1 ), B, LDB,
c     $                WORK( 2*MN+1 ), LWORK-2*MN, INFO )
c      END IF
*
*     complex workspace: 2*MN+NRHS.
*
*     B(1:N,1:NRHS) := P * B(1:N,1:NRHS)
*
      DO 60 J = 1, NRHS
         DO 50 I = 1, N
            WORK( JPVT( I ) ) = B( I, J )
   50    CONTINUE
         CALL ZCOPY( N, WORK( 1 ), 1, B( 1, J ), 1 )
   60 CONTINUE
*
*     complex workspace: N.
*
*     Undo scaling
*
      IF( IASCL.EQ.1 ) THEN
         CALL ZLASCL( 'G', 0, 0, ANRM, SMLNUM, N, NRHS, B, LDB, INFO )
         CALL ZLASCL( 'U', 0, 0, SMLNUM, ANRM, RANK, RANK, A, LDA,
     $                INFO )
      ELSE IF( IASCL.EQ.2 ) THEN
         CALL ZLASCL( 'G', 0, 0, ANRM, BIGNUM, N, NRHS, B, LDB, INFO )
         CALL ZLASCL( 'U', 0, 0, BIGNUM, ANRM, RANK, RANK, A, LDA,
     $                INFO )
      END IF
      IF( IBSCL.EQ.1 ) THEN
         CALL ZLASCL( 'G', 0, 0, SMLNUM, BNRM, N, NRHS, B, LDB, INFO )
      ELSE IF( IBSCL.EQ.2 ) THEN
         CALL ZLASCL( 'G', 0, 0, BIGNUM, BNRM, N, NRHS, B, LDB, INFO )
      END IF
*
   70 CONTINUE
      WORK( 1 ) = DCMPLX( LWKOPT )
*
      RETURN
*
*     End of ZGELSY
*
      END
