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

      subroutine intricc2(fname)

c     [X,RCOND,FERR]=ricc(A,C,D,'type','method')
c     [X,RCOND]=ricc(A,C,D,'type','method')
c     X = riccsl(A,C,D,'type','method')
c     'type' is 'continuous' or 'discrete'
c     'method' is 'schr' or 'sign' for continuous-time systems
c             and 'schr' or 'invf' for discrete-tyme systems 

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4 TYPE, METHOD 
      logical WANTC, WANTD, WSCHUR, WSIGN, WINVF
 
      minrhs=4
      maxrhs=5
      minlhs=1
      maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA.ne.NA) then
         buf='ricc'//': the matrix A must be square'
         call error(998)
         return
       endif       
       if(.not.getrhsvar(2,'d', MD, ND, lD)) return
       if(MD.ne.ND) then
         buf='ricc'//': the matrix D must be square'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(MC.ne.NC) then
         buf='ricc'//': the matrix C must be square'
         call error(998)
         return
       endif
       if(MA.ne.MC .or. MC.ne.MD .or. MA.ne.MD) then
         buf='ricc'//
     $     ': the matrices A, C and D must have the same order'
         call error(998)
         return

       endif
       N = MA

       if(.not.getrhsvar(4,'c', M1, N1, lTYPE)) return
       TYPE(1:4) = cstk(lTYPE:lTYPE+4)
       WANTC = (TYPE(1:4).eq.'cont' .or. TYPE(1:4).eq.'CONT')
       WANTD = (TYPE(1:4).eq.'disc' .or. TYPE(1:4).eq.'DISC')
       if(.not.WANTC .and. .not.WANTD) then
         buf='ricc'//': type must be continuous or discrete'
         call error(998)
         return
       endif
       
       k = 5
       WSCHUR = .TRUE.
       if(rhs.eq.5) then
         if(.not.getrhsvar(5,'c', M1, N1, lMETHOD)) return
         METHOD(1:4) = cstk(lMETHOD:lMETHOD+4)
         if(WANTC) then
           WSCHUR = (METHOD(1:4).eq.'schr' .or. METHOD(1:4).eq.'SCHR')
           WSIGN = (METHOD(1:4).eq.'sign' .or. METHOD(1:4).eq.'SIGN')
           if(.not.WSCHUR .and. .not.WSIGN) then
             buf='ricc'//': method must be schur or sign'
             call error(998)
             return
           endif
         else
           WSCHUR = (METHOD(1:4).eq.'schr' .or. METHOD(1:4).eq.'SCHR')
           WINVF = (METHOD(1:4).eq.'invf' .or. METHOD(1:4).eq.'INVF')
           if(.not.WSCHUR .and. .not.WINVF) then
             buf='ricc'//': method must be schur or invf'
             call error(998)
             return
           endif
         endif
         k = 6
       endif
       if(.not.createvar(k,'d', N, N, lX)) return      
       if(.not.createvar(k+1,'d', N, 1, lWR)) return
       if(.not.createvar(k+2,'d', N, 1, lWI)) return
       if(.not.createvar(k+3,'d', 1, 1, lRCOND)) return
       if(.not.createvar(k+4,'d', 1, 1, lFERR)) return 
       if(.not.createvar(k+5,'i', 1, max(2*N,N*N), lIWORK)) return       
       if(.not.createvar(k+6,'i', 1, 2*N, lBWORK)) return
       if(WANTC) then
         if(WSCHUR) then
           LWORKMIN = 9*N*N + 4*N + max(1,6*N)
         else if(WSIGN) then
           LWORKMIN = 9*N*N + 7*N + 1
         endif
       else
         if(WSCHUR) then
           LWORKMIN = 12*N*N + 22*N + max(16,4*N)
         else if(WINVF) then
           LWORKMIN = 28*N*N + 2*N + max(1,2*N) 
         endif
       endif
       LWORK=maxvol(k+7,'d')
       if(LWORK.le.LWORKMIN) then
         buf='ricc'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(k+7,'d',1,LWORK,lDWORK)) return

       if(WANTC) then
         if(WSCHUR) then 
            call RICCSL( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD), 
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND), 
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK),
     $        istk(lBWORK), INFO )
c            SUBROUTINE RICCSL( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
c     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK, INFO )
         else if(WSIGN) then
            call RICCMS( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK), INFO )
c            SUBROUTINE RICCMS( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
c     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
         endif
       else
         if(WSCHUR) then
            call RICDSL( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK),
     $        istk(lBWORK), INFO )
c            SUBROUTINE RICDSL( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
C     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK, INFO )
         else if (WINVF) then
            call RICDMF( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK), INFO )
c            SUBROUTINE RICDMF( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
C     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
         endif
      endif
      if(info.ne.0) then
        call errorinfo("ricc",info)
        return
      endif
   
    
      if(lhs.eq.1) then
        lhsvar(1)=k
      else if(lhs.eq.2) then
        lhsvar(1)=k
        lhsvar(2)=k+3
      else if(lhs.eq.3) then 
          lhsvar(1)=k
          lhsvar(2)=k+3
          lhsvar(3)=k+4
      endif
c
       end

c     =======================================================
      