      subroutine intdgebak(fname)
c***************************************************************************
c      V=DGEBAK(JOB, SIDE, ILO, IHI, SCALE, V)
c************************************************************************

      include 'stack.h'
      logical getrhsvar
      logical checklhs,checkrhs
      character fname*(*)
      character job*(1), side*(1)
c
       nbvars=0

       minrhs=6
       maxrhs=6
       minlhs=1
       maxlhs=1
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

c      job
       if(.not.getrhsvar(1,'c', m1, n1, ljob)) return
       job=cstk(ljob:ljob+1)
       if(.not.getrhsvar(2,'c', m1, n1, lside)) return
       side=cstk(lside:lside+1)
       if(.not.getrhsvar(3,'i', m1, n1, lilo)) return
       if(.not.getrhsvar(4,'i', m1, n1, lihi)) return
c          
       if(.not.getrhsvar(5,'d', mS, nS, lscale)) return
       if(.not.getrhsvar(6,'d', mV, nV, lV)) return

       call dgebak(job, side, mV , istk(lilo), istk(lihi), 
     $      stk(lscale), nV, stk(lV), mV, info)
c      SUBROUTINE DGEBAK( JOB, SIDE, N, ILO, IHI, SCALE, M, V, LDV,
c     $                   INFO )
       if(info.ne.0) then
         call errorinfo("dgebak",info)
         return
       endif

       lhsvar(1)=6
c
       return
       end



