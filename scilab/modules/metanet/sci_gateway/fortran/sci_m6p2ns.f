
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6p2ns, fin = 5
       subroutine intsm6p2ns(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable p (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable direct (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable n (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call p2ns(istk(iadr(lr1)),n1,stk(lw3),ne10,istk(iadr(lr2)),istk(i
     $ adr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)),n2,istk(iadr(lr6)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: ns
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne10,lrs,lcs)) return
        call cintf(1*ne10,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
