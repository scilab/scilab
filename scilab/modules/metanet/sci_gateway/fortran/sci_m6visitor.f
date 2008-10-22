
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6visitor, fin = 41
       subroutine intsm6visitor(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable a (number 2)
c       
       if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
       if(.not.checkval(fname,m2,n2)) return
c       checking variable nstac (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,m2,n2)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2*m2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr3)),1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw6,loc6)) return
       call visitor(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(
     $ lw4),stk(lw5),stk(lw6))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: iroad
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
