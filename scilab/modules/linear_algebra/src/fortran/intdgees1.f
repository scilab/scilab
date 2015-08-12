c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      
      subroutine intdgees1(fname)

c     [VS,T]=schur(A,'type')
c     type is r orc$

      include 'stack.h'
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

      
