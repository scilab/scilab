c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) 2013 - Michael Baudin
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
      subroutine intdgesv3(fname)

c     a\b

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, dlange
      double precision RCONDthresh
      integer vfinite
      external dlamch, dlange, vfinite
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
c     Check if A and B matrices contains Inf or NaN's
      if (vfinite(M*N, stk(lA)).eq.0) then
         call error(229)
         return
      endif
      if (vfinite(MB*NRHS, stk(lB)).eq.0) then
         call error(229)
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
      RCOND_thresh=EPS*10
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
            if(RCOND.gt.RCONDthresh) then
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
         call writebufdgesv3(buf,RCOND)
         call msgs(5,1)
      endif
c     
c     M.ne.N or A singular
c     
      RCOND = RCONDthresh
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


