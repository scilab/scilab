      subroutine inttestdgebal(fname)
c***************************************************************************
c   usage: 
c   [SCALE, ILOW, IHI]=dgebal(JOB, A) 
c************************************************************************
      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      character job*(1)
c
       nbvars=0

       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

c      job
       if(.not.getrhsvar(1,'c', m1, n1, ljob)) return
       job=cstk(ljob:ljob+1)
c      A
       if(.not.getrhsvar(2,'d', mA, nA, lA)) return
c          
       if(.not.createvar(3,'d', mA, 1, lscale)) return
       if(.not.createvar(4,'i', 1, 1, ilo)) return
       if(.not.createvar(5,'i', 1, 1, ihi)) return

       call dgebal(job, mA , stk(lA), mA, istk(ilo), istk(ihi),
     $      stk(lscale), info)
c       call DGEBAL( JOB, N, A, LDA, ILO, IHI, SCALE, INFO )

       if(info.ne.0) then
         call errorinfo("dgebal",info)
         return
       endif

c     return SCALE (3), ILOW (4), IHI (5)
       lhsvar(1)=3
       lhsvar(2)=4
       lhsvar(3)=5
c
       return
       end



