c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
			subroutine intzgees0(fname)

c     [VS,T]=schur(A)

      include 'stack.h'
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

