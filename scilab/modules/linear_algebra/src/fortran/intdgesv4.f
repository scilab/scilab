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
	   subroutine intdgesv4(fname)

c     a/b

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision RCONDthresh
      double precision dlamch, dlange
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
c     Check if A and B matrices contains Inf or NaN's
      if (vfinite(M*N, stk(lA)).eq.0) then
         call error(229)
         return
      endif
      if (vfinite(MB*NB, stk(lB)).eq.0) then
         call error(229)
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
      RCOND_thresh=EPS*10
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
            if(RCOND.gt.RCOND_thresh) then
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
         call writebufdgesv4(buf,RCOND)
         call msgs(5,1)
      endif
c     
c     M.ne.N or A singular
c     
      RCOND = RCOND_thresh
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

