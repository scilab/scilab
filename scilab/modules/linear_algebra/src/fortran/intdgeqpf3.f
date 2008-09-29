c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
      subroutine intdgeqpf3(fname)

c     [Q,R]=qr(A)
c     [Q,R,E]=qr(A)

      include 'stack.h'
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
c      if(N.ge.2) then
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
c      endif
      
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


