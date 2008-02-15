
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6kilter, fin = 25
       subroutine intsm6kilter(fname)
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
       if(.not.checkrhs(fname,8,8)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable mincap (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable maxcap (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable he (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ta (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable la (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable lp (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable n (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable cost (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       if(.not.checkval(fname,n1,n3)) return
       if(.not.checkval(fname,n1,n4)) return
       if(.not.checkval(fname,n1,n8)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,n1,1,lw11,loc11)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr7)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+3,0,istk(iadr(lr7)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr7)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr7)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+6,0,n1,1,lw16,loc16)) return
       if(.not.cremat(fname,top+7,0,1,1,lw17,loc17)) return
       call kilter(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(l
     $ r8),istk(iadr(lr5)),istk(iadr(lr6)),n1,n5,istk(iadr(lr7)),istk(ia
     $ dr(lr4)),stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(lw15),stk(lw
     $ 16),stk(lw17))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+7
c     
       if(lhs .ge. 1) then
c       --------------output variable: phi
        top=topl+1
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: flag
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw17)),-1,stk(lrs),-1)
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
