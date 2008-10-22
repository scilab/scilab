
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6flomax, fin = 24
       subroutine intsm6flomax(fname)
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
       if(.not.checkrhs(fname,10,10)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable is (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable it (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable la (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lp (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable he (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable ta (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable mincap (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable maxcap (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c       checking variable n (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable phi (number 10)
c       
       if(.not.getvectrow(fname,top,top-rhs+10,it10,m10,n10,lr10,lc10)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n5,n6)) return
       if(.not.checkval(fname,n5,n7)) return
       if(.not.checkval(fname,n5,n8)) return
       if(.not.checkval(fname,n5,n10)) return
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(n8,stk(lr8),istk(iadr(lr8)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       nn9= int(stk(lr9))
       if(.not.cremat(fname,top+1,0,nn9,1,lw9,loc9)) return
       call entier(1,stk(lr9),istk(iadr(lr9)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n10,stk(lr10),istk(iadr(lr10)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr9)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+3,0,1,1,lw15,loc15)) return
       call flomax(istk(iadr(lr7)),istk(iadr(lr8)),istk(iadr(lr5)),istk(
     $ iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(iadr(lr4)),n5,stk
     $ (lw9),n3,istk(iadr(lr9)),istk(iadr(lr6)),istk(iadr(lr10)),stk(lw1
     $ 4),stk(lw15))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: vflow
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw15)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: phi
        top=topl+2
        if(.not.cremat(fname,top,0,1,n5,lrs,lcs)) return
        call int2db(n10*m10,istk(iadr(lr10)),-1,stk(lrs),-1)
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
