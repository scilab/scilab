c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$

 
      subroutine intzgelsy(fname)

c     [X,rank]=lsq(A,B,rcond)

      include 'stack.h'
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
