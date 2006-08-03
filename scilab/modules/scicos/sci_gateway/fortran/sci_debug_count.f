c     ********************
c     Copyright INRIA
c     ********************
			subroutine scicosdebugcount(fname)
c
c
      include 'stack.h'
      common /cosdebugcounter/ counter
      logical getrhsvar, createvar,checklhs,checkrhs
      integer counter
      character*(*) fname
c
      rhs = max(0,rhs)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,-1,0)) return
      if(.not.createvar(1,'d',1,1,ipr1)) return
      stk(ipr1)=counter
      lhsvar(1)=1 
      end
c     ********************      