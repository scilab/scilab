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
      subroutine inthinf(fname)

c     [Ak,Bk,Ck,Dk,RCOND]=hinf(A,B,C,D,ncon,nmeas,gamma)

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
         buf='hinf'//': A must be a square matrix'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(MA .ne. MB) then
         buf='hinf'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(NA .ne. NC) then
         buf='hinf'//': A and C must have equal number of columns'
         call error(998)
         return
       endif
       if(.not.getrhsvar(4,'d', MD, ND, lD)) return
       if(NB .ne. ND) then
         buf='hinf'//': B and D must have equal number of columns'
         call error(998)
         return
       endif
       if(MC .ne. MD) then
         buf='hinf'//': C and D must have equal number of rows'
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
         buf='hinf'//': NCON must be a scalar'
         call error(998)
         return
       endif
       NCON = istk(lNCON)

       if(.not.getrhsvar(6,'i', M2, N2, lNMEAS)) return
       if(M2.ne.1 .or. N2.ne.1 ) then
         buf='hinf'//': NMEAS must be a scalar'
         call error(998)
         return
       endif
       NMEAS = istk(lNMEAS)

       if(.not.getrhsvar(7,'d', M3, N3, lGAMMA)) return
       if(M3.ne.1 .or. N3.ne.1 ) then
         buf='hinf'//': GAMMA must be a scalar'
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
       if(.not.createvar(12,'d', 4, 1, lRCOND)) return
       LINTWORK = max(2*max(N,M-NCON,R-NMEAS,NCON),N*N)

       if(.not.createvar(13,'i', LINTWORK, 1, lIWORK)) return
       if(.not.createvar(14,'i', 2*N, 1, lBWORK)) return

       Q = MAX( M - NCON, NCON, R - NMEAS, NMEAS )
       LWORKMIN = 2*Q*( 3*Q + 2*N ) + 
     $          MAX( 1, ( N + Q )*( N + Q + 6 ), 
     $               Q*( Q + MAX( N, Q, 5 ) + 1 ), 2*N*( N + 2*Q ) +
     $               MAX( 1, 4*Q*Q +
     $                    MAX( 2*Q, 3*N*N + 
     $                         MAX( 2*N*Q, 10*N*N + 12*N + 5 ) ),
     $                         Q*( 3*N + 3*Q + 
     $                             MAX( 2*N, 4*Q + max( N, Q ) ) ) ) )
       LWORK=maxvol(15,'d')
       if(LWORK.le.LWORKMIN) then
         buf='hinf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(15,'d',1,LWORK,lDWORK)) return
      
      CALL SB10FD( N, M, R, NCON, NMEAS, GAMMA, stk(lA), N,
     $             stk(lB), N, stk(lC), R, stk(lD), R, stk(lAK),
     $             N, stk(lBK), N, stk(lCK), NCON, stk(lDK),
     $             NCON, stk(lRCOND), TOL, istk(lIWORK),
     $             stk(lDWORK), LDWORK, istk(lBWORK), INFO )
      if(info.ne.0) then
        call errorinfo("hinf",info)
        return
      endif      

      lhsvar(1) = 8
      lhsvar(2) = 9
      lhsvar(3) = 10
      lhsvar(4) = 11
      if(lhs.eq.5) lhsvar(5) = 12

c
       end
c     =======================================================
      