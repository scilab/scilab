
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6floqua, fin = 27
       subroutine intsm6floqua(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,10,10)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable eps (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable mincap (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable maxcap (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable he (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ta (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable la (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable lp (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable n (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable qorig (number 9)
c       
       if(.not.getvectrow(fname,top,top-rhs+9,it9,m9,n9,lr9,lc9)) return
c       checking variable qweight (number 10)
c       
       if(.not.getvectrow(fname,top,top-rhs+10,it10,m10,n10,lr10,lc10)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n3)) return
       if(.not.checkval(fname,n2,n4)) return
       if(.not.checkval(fname,n2,n5)) return
       if(.not.checkval(fname,n2,n9)) return
       if(.not.checkval(fname,n2,n10)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,n2,1,lw2,loc2)) return
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+2,0,n2,1,lw4,loc4)) return
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+3,0,n2,1,lw14,loc14)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr8)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr8)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr8)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr8)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+8,0,n2,1,lw19,loc19)) return
       if(.not.cremat(fname,top+9,0,1,1,lw21,loc21)) return
       call floqua(istk(iadr(lr2)),stk(lw2),istk(iadr(lr3)),stk(lw4),stk
     $ (lr1),istk(iadr(lr4)),istk(iadr(lr6)),istk(iadr(lr7)),n2,n6,istk(
     $ iadr(lr8)),istk(iadr(lr5)),stk(lr9),stk(lw14),stk(lw15),stk(lw16)
     $ ,stk(lw17),stk(lw18),stk(lw19),stk(lr10),stk(lw21))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+9
c     
       if(lhs .ge. 1) then
c       --------------output variable: phi
        top=topl+1
        if(.not.cremat(fname,top,0,1,n2,lrs,lcs)) return
        call dcopy(1*n2,stk(lw14),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: flag
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw21)),-1,stk(lrs),-1)
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
