
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6transc, fin = 18
       subroutine intsm6transc(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ls (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+2,0,1,1,lw4,loc4)) return
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call transc(istk(iadr(lr1)),stk(lw2),istk(iadr(lr2)),stk(lw4),n2,
     $ ne8,ne6,istk(iadr(lr3)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: lpft
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne6,lrs,lcs)) return
        call cintf(1*ne6,stk(lw2),stk(lrs))
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: lsft
        top=topl+2
        if(.not.cremat(fname,top,0,1,ne8,lrs,lcs)) return
        call cintf(1*ne8,stk(lw4),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
