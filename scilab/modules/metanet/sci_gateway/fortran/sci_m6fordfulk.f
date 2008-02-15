
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6fordfulk, fin = 13
       subroutine intsm6fordfulk(fname)
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
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable ma (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable source (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable sink (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable u (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable tail (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable head (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n5,n6)) return
       if(.not.checkval(fname,n5,n7)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr2)),1,lw6,loc6)) return
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr2)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,1,1,lw15,loc15)) return
       if(.not.cremat(fname,top+9,0,1,1,lw16,loc16)) return
       if(.not.cremat(fname,top+10,0,istk(iadr(lr1)),1,lw17,loc17)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr1)),1,lw18,loc18)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,1,1,lw19,loc19)) return
       call fordfulk(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),ist
     $ k(iadr(lr4)),istk(iadr(lr5)),stk(lw6),istk(iadr(lr6)),istk(iadr(l
     $ r7)),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),s
     $ tk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+12
c     
       if(lhs .ge. 1) then
c       --------------output variable: maxflow
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw16)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: f
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: ierr
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw19)),-1,stk(lrs),-1)
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
       top=topk+lhs
       return
       end
c
