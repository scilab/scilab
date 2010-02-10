c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) DIGITEO 2010 - Yann COLLETTE
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

       subroutine scita2lpd(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable tail (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable head (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       nn5= int(stk(lr3))
       if(.not.cremat(fname,top+1,0,nn5,1,lw5,loc5)) return
       if(.not.cremat(fname,top+2,0,n1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,n1,1,lw7,loc7)) return
       call ta2lpd(istk(iadr(lr1)),istk(iadr(lr2)),n1,istk(iadr(lr4)),st
     $ k(lw5),stk(lw6),stk(lw7))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: lp
        top=topl+1
        if(.not.cremat(fname,top,0,1,nn5,lrs,lcs)) return
        call int2db(1*nn5,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: la
        top=topl+2
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: ls
        top=topl+3
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
