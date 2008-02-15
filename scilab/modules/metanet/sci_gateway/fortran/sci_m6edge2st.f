
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6edge2st, fin = 6
       subroutine intsm6edge2st(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable alpha (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     
c       cross variable size checking
c     
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call edge2st(n1,istk(iadr(lr1)),stk(lw3),ne3)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: tree
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne3,lrs,lcs)) return
        call cintf(1*ne3,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
