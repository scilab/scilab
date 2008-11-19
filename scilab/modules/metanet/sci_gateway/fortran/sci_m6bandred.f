
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6bandred, fin = 48
       subroutine intsm6bandred(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nz (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable liwork (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable iwork (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable lrwork (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable rwork (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable optpro (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+1,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       call bandred(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk
     $ (iadr(lr4)),istk(iadr(lr5)),stk(lr6),istk(iadr(lr7)),stk(lw8),stk
     $ (lw9),stk(lw10),stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: iperm
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: mrepi
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: iband
        top=topl+3
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: ierr
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
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
