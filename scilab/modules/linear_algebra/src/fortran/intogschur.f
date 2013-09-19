c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$

      subroutine intogschur(fname)
c     [dim]=gschur(A,B,function)
c     [VSR,dim]=gschur(A,B,function)
c     [VSL,VSR,dim]=gschur(A,B,function)
c     [As,Bs,VSR,dim]=gschur(A,B,function)
c     [VSR,dim]=gschur(A,B,function)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character  JOBVSL, JOBVSR, SORT
      logical SCIGSHUR,scigchk
      external SCIGSHUR,scigchk
      common /scigsch/ lf, nfree, nf

      minrhs=3
      maxrhs=3
      minlhs=1
      maxlhs=5
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif

      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         if(lhs.eq.2) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
         elseif(lhs.eq.3) then
            if(.not.createvar(3,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
         elseif(lhs.eq.4) then
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
         else
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', N, N, lVSL)) return
            if(.not.createvar(5,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
            lhsvar(5)=5
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      nf=3
      if(.not.getrhsvar(nf,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.2) then
         err=nf
         call error(80)
         return
      endif

      if(.not.createvar(4,'i', 1, 1, lSDIM)) return
      if(.not.createvar(5,'d', N, 1, lALPHAR)) return
      if(.not.createvar(6,'d', N, 1, lALPHAI)) return
      if(.not.createvar(7,'d', N, 1, lBETA)) return              
      if(.not.createvar(8,'d', N, N, lVSL)) return
      if(.not.createvar(9,'d', N, N, lVSR)) return
      if(.not.createvar(10,'i', 2*N, 1, lBWORK)) return

c To adapt to Lapack 3.1.1
c Old version : LWORKMIN = 7*(N+1)+16
      LWORKMIN = MAX( 8*N, 6*N + 16 )
      LWORK=LWORKMIN
      if(.not.createvar(11,'d',1,LWORK,lDWORK)) return
      nfree = 12

      JOBVSL = 'V'
      JOBVSR = 'V'
      SORT = 'S'
c     
      if(.not.scigchk()) return
      call DGGES( JOBVSL, JOBVSR, SORT, SCIGSHUR, N, stk(lA), N,
     $     stk(lB), N, istk(lSDIM), stk(lALPHAR), stk(lALPHAI),
     $     stk(lBETA), stk(lVSL), N, stk(lVSR), N, stk(lDWORK), LWORK,
     $     istk(lBWORK),INFO )


c     SUBROUTINE DGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $     SDIM, ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR, WORK,
c     $     LWORK, BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         elseif(info.eq.N+2) then 
            call msgs(103,0)
         elseif(info.eq.N+3) then 
            buf='reordering failed'
            call error(1002)
            return
         endif
      endif

      if(lhs.eq.1) then
         lhsvar(1)=4
      elseif(lhs.eq.2) then
         lhsvar(1)=9
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=8
         lhsvar(2)=9
         lhsvar(3)=4
      else if(lhs.eq.4) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=9
         lhsvar(4)=4
      else if(lhs.eq.5) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=8
         lhsvar(4)=9
         lhsvar(5)=4
      endif
      end

