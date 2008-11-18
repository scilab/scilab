
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6prevn2p, fin = 3
       subroutine intsm6prevn2p(fname)
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
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable i (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable j (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable pln (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable la (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable lp (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable ls (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable direct (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n4,n6)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,1,1,lw10,loc10)) return
       call prevn2p(istk(iadr(lr1)),istk(iadr(lr2)),n4,n3,istk(iadr(lr4)
     $ ),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),istk(iadr(lr3))
     $ ,stk(lw10),ne11)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: p
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne11,lrs,lcs)) return
        call cintf(1*ne11,stk(lw10),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
