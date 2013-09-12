c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine intzgetri(fname)

c     B = inv(A)

      include 'stack.h'
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

      ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )

      call ZGETRF( N, N, zstk(lA), N, istk(lIPIV), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.gt.0) then
         call error(19)
         return
      elseif (info.lt.0) then
         return
      endif
      RCOND = 0.0d0
   
      call ZGECON( '1', N, zstk(lA), N, ANORM, RCOND, zstk(lDWORK),
     $     stk(lRWORK), INFO )
      if(RCOND.le.sqrt(dlamch('eps'))) then
         call writebufzgetri(buf,RCOND)
c     .  ill conditionned problem
         call msgs(5,0)
      endif

      call ZGETRI( N, zstk(lA), N, istk(lIPIV), zstk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE ZGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end

