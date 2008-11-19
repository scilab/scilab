
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6clique1, fin = 39
       subroutine intsm6clique1(fname)
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
       if(.not.checklhs(fname,1,1)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable m2 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable np1 (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable nwk (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable kat (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable hat (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr5)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       call clique1(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk
     $ (iadr(lr4)),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),stk(l
     $ w8),stk(lw9),stk(lw10))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: wcl
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
