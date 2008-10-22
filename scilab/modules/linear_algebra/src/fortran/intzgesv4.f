c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
	   subroutine intzgesv4(fname)

c     a/b

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      integer vfinite
      external dlamch, zlange, vfinite
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
c     Check if A and B matrices contains Inf or NaN's
      if (vfinite(M*N*2, zstk(lA)).eq.0) then
         call error(229)
         return
      endif
      if (vfinite(MB*NB*2, zstk(lB)).eq.0) then
         call error(229)
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
         call writebufzgesv4(buf,RCOND)
c     .  ill conditionned problem
         call msgs(5,1)
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


