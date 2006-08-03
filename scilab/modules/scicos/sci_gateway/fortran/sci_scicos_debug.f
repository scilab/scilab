c     ********************
c     Copyright INRIA
c     ********************
      subroutine scicosdebug(fname)
c
c
      include 'stack.h'
      common /cosdebug/ cosd
      logical getrhsvar, createvar,checklhs,checkrhs
      integer cosd
      character*(*) fname
c
      rhs = max(0,rhs)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,-1,1)) return
      if (rhs.eq.1) then
         if(.not.getrhsvar(1,'i',n,m,i)) return
         cosd=istk(i)
         lhsvar(1)=0
      else
         if(.not.createvar(1,'d',1,1,ipr1)) return
         stk(ipr1)=cosd
         lhsvar(1)=1 
      endif

      end
c     ********************

