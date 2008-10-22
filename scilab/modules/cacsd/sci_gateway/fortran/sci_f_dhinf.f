c     =======================================================

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     =======================================================
      subroutine intdhinf(fname)

c     [Ak,Bk,Ck,Dk,RCOND]=dhinf(A,B,C,D,ncon,nmeas,gamma)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision GAMMA, TOL, EPS
      integer N, M, R, Q
      double precision dlamch
      external dlamch
c
       minrhs=7
       maxrhs=7
       minlhs=4
       maxlhs=5
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA .ne. NA) then
         buf='dhinf'//': A must be a square matrix'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(MA .ne. MB) then
         buf='dhinf'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(NA .ne. NC) then
         buf='dhinf'//': A and C must have equal number of columns'
         call error(998)
         return
       endif
       if(.not.getrhsvar(4,'d', MD, ND, lD)) return
       if(NB .ne. ND) then
         buf='dhinf'//': B and D must have equal number of columns'
         call error(998)
         return
       endif
       if(MC .ne. MD) then
         buf='dhinf'//': C and D must have equal number of rows'
         call error(998)
         return
       endif
       N = MA
       M = NB
       R = MC
       if(N.eq.0 .or. M.eq.0 .or. R.eq.0) then
         if(.not.createvar(5,'d', 0, 0, lAK)) return
         if(.not.createvar(6,'d', 0, 0, lBK)) return
         if(.not.createvar(7,'d', 0, 0, lCK)) return
         if(.not.createvar(8,'d', 0, 0, lDK)) return
         if(.not.createvar(9,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 5
         lhsvar(2) = 6
         lhsvar(3) = 7
         lhsvar(4) = 8
         lhsvar(5) = 9
         return
       endif
       if(.not.getrhsvar(5,'i', M1, N1, lNCON)) return
       if(M1.ne.1 .or. N1.ne.1 ) then
         buf='dhinf'//': NCON must be a scalar'
         call error(998)
         return
       endif
       NCON = istk(lNCON)

       if(.not.getrhsvar(6,'i', M2, N2, lNMEAS)) return
       if(M2.ne.1 .or. N2.ne.1 ) then
         buf='dhinf'//': NMEAS must be a scalar'
         call error(998)
         return
       endif
       NMEAS = istk(lNMEAS)

       if(.not.getrhsvar(7,'d', M3, N3, lGAMMA)) return
       if(M3.ne.1 .or. N3.ne.1 ) then
         buf='dhinf'//': GAMMA must be a scalar'
         call error(998)
         return
       endif
       GAMMA = stk(lGAMMA)
       EPS = dlamch('eps')
       TOL = sqrt(EPS)
       
       if(.not.createvar(8,'d', N, N, lAK)) return
       if(.not.createvar(9,'d', N, NMEAS, lBK)) return
       if(.not.createvar(10,'d', NCON, N, lCK)) return
       if(.not.createvar(11,'d', NCON, NMEAS, lDK)) return
       if(.not.createvar(12,'d', N, N, lX)) return
       if(.not.createvar(13,'d', N, N, lZ)) return
       if(.not.createvar(14,'d', 8, 1, lRCOND)) return
       LINTWORK = max(2*max(NCON,N),M,NCON+NMEAS,N*N)
       if(.not.createvar(15,'i', LINTWORK, 1, lIWORK)) return
       if(.not.createvar(16,'i', 2*N, 1, lBWORK)) return

       Q = MAX( M - NCON, NCON, R - NMEAS, NMEAS )
       LWORKMIN = max((N+Q)*(N+Q+6),13*N*N + M*M + 2*Q*Q + N*(M+Q) +
     $            max(M*(M+7*N),2*Q*(8*N+M+2*Q)) + 6*N + 
     $            max(14*N+23,16*N,2*N+max(M,2*Q),3*max(M,2*Q)))
       LWORK=maxvol(17,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dhinf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(17,'d',1,LWORK,lDWORK)) return
      
      CALL SB10DD( N, M, R, NCON, NMEAS, GAMMA, stk(lA), N,
     $             stk(lB), N, stk(lC), R, stk(lD), R, stk(lAK),
     $             N, stk(lBK), N, stk(lCK), NCON, stk(lDK),
     $             NCON, stk(lX), N, stk(lZ), N, stk(lRCOND), TOL,
     $             istk(lIWORK), stk(lDWORK), LDWORK, istk(lBWORK),
     $             INFO )
      if(info.ne.0) then
        call errorinfo("dhinf",info)
        return
      endif      

      lhsvar(1) = 8
      lhsvar(2) = 9
      lhsvar(3) = 10
      lhsvar(4) = 11
      if(lhs.eq.5) lhsvar(5) = 14

c
       end
c     =======================================================





