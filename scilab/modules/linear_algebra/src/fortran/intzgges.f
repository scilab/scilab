c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine intzgges(fname)

c     [AS,BS,VSL,VSR]=gschur(A,B)
c     [AS,BS]=gschur(A,B)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      

      character fname*(*)
      character JOBVSL, JOBVSR, SORT
      logical voiddummy
      external voiddummy

      minrhs=2
      maxrhs=2
      minlhs=2
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'z', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.NB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         lhsvar(2)=2
         if(lhs.eq.4) then
            if(.not.createvar(3,'d', N, N, lVSL)) return
            if(.not.createvar(4,'d', N, N, lVSR)) return
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(3,'i', 1, 1, lSDIM)) return
      if(.not.createvar(4,'z', N, 1, lALPHA)) return
      if(.not.createvar(5,'z', N, 1, lBETA)) return
      K = 6
      if(lhs.eq.4) then
         if(.not.createvar(6,'z', N, N, lVSL)) return
         if(.not.createvar(7,'z', N, N, lVSR)) return
         k = 8
      endif
      if(.not.createvar(k,'d', 8*N, 1, lRWORK)) return
      k=k+1
      if(.not.createvar(k,'i', 2*N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+1,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return


      if(lhs.eq.2) then
         JOBVSL = 'N'
         JOBVSR = 'N'
         lVSL = lDWORK
         lVSR = lDWORK
      else
         JOBVSL = 'V'
         JOBVSR = 'V'
      endif
      SORT = 'N'
      call ZGGES( JOBVSL, JOBVSR, SORT, voiddummy, N, 
     $     zstk(lA), N,
     $     zstk(lB), N, istk(lSDIM), zstk(lALPHA), zstk(lBETA),
     $     zstk(lVSL), N, zstk(lVSR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), istk(lBWORK), INFO )
c     SUBROUTINE ZGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $    SDIM, ALPHA, BETA, VSL, LDVSL, VSR, LDVSR, WORK, LWORK, RWORK
c     ,
c     $    BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         endif
      endif

      if(lhs.eq.2) then
         lhsvar(1)=1
         lhsvar(2)=2
      else
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=6
         lhsvar(4)=7
      endif
c     
      end


