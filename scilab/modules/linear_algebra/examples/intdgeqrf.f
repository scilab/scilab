      subroutine intdgeqrf(fname)

c     [A,TAU]=dgeqrf(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
c
       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(.not.createvar(2,'d', 1, min(M,N), ltau)) return
       LWORKMIN = MAX(1,N)
       LWORK=maxvol(3,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgeqrf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(3,'d',1,LWORK,lDWORK)) return

      call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dgeqrf",info)
         return
       endif
       lhsvar(1)=1
       lhsvar(2)=2
c
       end



