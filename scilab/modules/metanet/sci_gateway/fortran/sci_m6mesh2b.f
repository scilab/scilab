
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6mesh2b, fin = 46
       subroutine intsm6mesh2b(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,getvectrow,cr
     $ emat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable nbs (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n6 (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable n4 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable lfront (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable cr (number 5)
c       
       if(.not.getmat(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
       if(.not.checkval(fname,m5,2)) return
c       checking variable tri (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable front (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,m5,istk(iadr(lr1)),lw6,lwc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw7,loc7)) return
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+3,0,1,1,lw10,loc10)) return
       if(.not.cremat(fname,top+4,0,1,1,lw11,loc11)) return
       call mesh2b(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr4)),stk(lr5),stk(lw6),stk(lw7),istk(iadr(lr6)),istk(iadr(l
     $ r7)),stk(lw10),stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: nbt
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: nu
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: c
        top=topl+3
        if(.not.cremat(fname,top,0,m5,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(m5*istk(iadr(lr1)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: err
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw11)),-1,stk(lrs),-1)
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
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       top=topk+lhs
       return
       end
c
