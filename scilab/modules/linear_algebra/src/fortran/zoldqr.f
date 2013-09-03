c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine zoldqr(tol,fname)

c     [Q,R,rk,E]=qr(A,tol)
c     [Q,R,rk,E]=qr(A)

      include 'stack.h'
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
C      if(N.ge.2) then
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
C      endif
      
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
      
