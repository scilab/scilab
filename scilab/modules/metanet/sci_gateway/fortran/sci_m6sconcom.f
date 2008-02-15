
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6sconcom, fin = 11
       subroutine intsm6sconcom(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top
       rhsk = rhs
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable icomp (number 1)
c
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nfcomp (number 2)
c
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c
c       cross variable size checking
c
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       call concom(istk(iadr(lr1)),n2,istk(iadr(lr2)),stk(lw4),ne4)
       if(err .gt. 0 .or. err1 .gt. 0) return
c
       topk=top-rhs
       topl=top+1
c
       if(lhs .ge. 1) then
c       --------------output variable: ns
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne4,lrs,lcs)) return
        call cintf(1*ne4,stk(lw4),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
