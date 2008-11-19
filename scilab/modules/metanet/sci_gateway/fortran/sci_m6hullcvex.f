
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6hullcvex, fin = 37
       subroutine intsm6hullcvex(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nn (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable xx (number 3)
c       
       if(.not.getmat(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
       if(.not.checkval(fname,m3,2)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw8,loc8)) return
       call hullcvex(istk(iadr(lr1)),istk(iadr(lr2)),stk(lr3),stk(lw4),s
     $ tk(lw5),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+5
c     
       if(lhs .ge. 1) then
c       --------------output variable: nhull
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: iwork
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw5)),-1,stk(lrs),-1)
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
