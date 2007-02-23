      subroutine intdgels(fname)
c
c     [B,A]=dgels(TRANS, A, B)
      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      character TRANS*(1)
c
       nbvars=0

       minrhs=3
       maxrhs=3
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'c', m1,n1, ltrans))return
       TRANS=cstk(ltrans:ltrans+1)
       if(.not.getrhsvar(2,'d', M, N, lA)) return
       if(.not.getrhsvar(3,'d', mB, NRHS, lB)) return
c      check that B is M-by-NRHS if TRANS = 'N', or N-by-NRHS
c          if TRANS = 'T'.
       LWORKMIN = min(M,N) + MAX(1,M,N,NRHS)
       LWORK=maxvol(4,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgels'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(4,'d',1,LWORK,lDWORK)) return
c      SUBROUTINE DGELS( TRANS, M, N, NRHS, A, LDA, B, LDB, WORK, LWORK,
c     $                  INFO )

       call dgels(TRANS ,M ,N ,NRHS, stk(lA), M,
     $      stk(lB), mB, stk(lDWORK), LWORK , INFO)
       if(info.ne.0) then
         call errorinfo("dgels",info)
         return
       endif
       lhsvar(1)=3
       lhsvar(2)=2
c
       end



