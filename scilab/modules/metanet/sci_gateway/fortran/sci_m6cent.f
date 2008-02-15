
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6cent, fin = 36
       subroutine intsm6cent(fname)
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
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable fe (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable che (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable head (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lngt (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n1 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n3,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+4,0,n1,1,lw9,loc9)) return
       nn10= int(stk(lr5))
       if(.not.cremat(fname,top+5,0,nn10,1,lw10,loc10)) return
       if(.not.cremat(fname,top+6,0,n1,1,lw11,loc11)) return
       call cent(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(ia
     $ dr(lr4)),n1,stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw1
     $ 1))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+6
c     
       if(lhs .ge. 1) then
c       --------------output variable: r
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: idmax
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw7)),-1,stk(lrs),-1)
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
