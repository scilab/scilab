c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
      subroutine intdgelsy(fname)

c     [X,rank]=lsq(A,B,rcond)

      include 'stack.h'
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
