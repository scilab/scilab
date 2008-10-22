
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6deumesh, fin = 47
       subroutine intsm6deumesh(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,5)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n4 (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable n6 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable x (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable y (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n4,n5)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr3)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,1,1,lw8,loc8)) return
       nn9=3
       if(.not.cremat(fname,top+4,0,nn9,istk(iadr(lr2)),lw9,lwc9)) retur
     $ n
       if(.not.cremat(fname,top+5,0,1,1,lw10,loc10)) return
       call deumesh(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(
     $ lr4),stk(lr5),stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+5
c     
       if(lhs .ge. 1) then
c       --------------output variable: iadj
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: iend
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: nbt
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: nu
        top=topl+4
        if(.not.cremat(fname,top,0,nn9,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(nn9*istk(iadr(lr2)),istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 5) then
c       --------------output variable: iero
        top=topl+5
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw10)),-1,stk(lrs),-1)
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
       if(lhs .ge. 5) then
        call copyobj(fname,topl+5,topk+5)
       endif
       top=topk+lhs
       return
       end
c
