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
      subroutine intmucomp(fname)

c     [bound,D,G] = mucomp(Z,K,T)
c     [bound,D] = mucomp(Z,K,T)
c     bound = mucomp(Z,K,T)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)

       minrhs=3
       maxrhs=3
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lZ)) return
       if(M.ne.N) then
         buf='mucomp'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           lhsvar(1) = 2
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           if(.not.createvar(3,'d', N, 1, lD)) return
           lhsvar(1)=2
           lhsvar(2)=3
           return
         else if(lhs.eq.3) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           if(.not.createvar(3,'d', N, 1, lD)) return
           if(.not.createvar(4,'d', N, 1, lG)) return 
           lhsvar(1)=2
           lhsvar(2)=3
           lhsvar(3)=4
           return
         endif
       endif
       if(.not.getrhsvar(2,'i', M1, N1, lK)) return
       if(.not.getrhsvar(3,'i', M2, N2, lT)) return
       if(M1*N1.ne.M2*N2) then
         buf='mucomp'//': K and T must have equal lengths'
         call error(998)
         return
       endif
       M = M1*N1
       if(.not.createvar(4,'d', 1, 1, lBOUND)) return
       if(.not.createvar(5,'d', N, 1, lD)) return
       if(.not.createvar(6,'d', N, 1, lG)) return 
       if(.not.createvar(7,'d', 2*N-1, 1, lX)) return
       if(.not.createvar(8,'i', 4*N-2, 1, lIWORK)) return 
       LRWRK = 2*N*N*N + 9*N*N +  44*N - 11
       if(.not.createvar(9,'d', LRWRK, 1, lRWORK)) return
       LZWRKMIN  = 6*N*N*N + 12*N*N + 12*N - 3
       LZWRK=maxvol(10,'z')
       if(LZWRK.le.LZWRKMIN) then
         buf='mucomp'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(10,'z',1,LZWRK,lZWORK)) return 
    
       call AB13MD( 'N', N, zstk(lZ), N, M, istk(lK), istk(lT),
     $    stk(lX), stk(lBOUND), stk(lD), stk(lG), istk(lIWORK),
     $    stk(lRWORK), LRWRK, zstk(lZWORK), LZWRK, INFO )
c       SUBROUTINE AB13MD( FACT, N, Z, N, M, NBLOCK, ITYPE, X,
c     $    BOUND, D, G, IWORK, DWORK, LDWORK, ZWORK, LZWORK,
c     $    INFO )
      if(info.ne.0) then
        call errorinfo("mucomp",info)
        return
      endif
    
      if(lhs.eq.1) then
        lhsvar(1) = 4
      else if(lhs.eq.2) then
        lhsvar(1)=4
        lhsvar(2)=5
      else if(lhs.eq.3) then
        lhsvar(1)=4
        lhsvar(2)=5
        lhsvar(3)=6
      endif
c
       end

c     =======================================================
      