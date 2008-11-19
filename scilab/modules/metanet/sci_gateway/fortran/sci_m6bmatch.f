
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6bmatch, fin = 42
       subroutine intsm6bmatch(fname)
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
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,14,14)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable nd2 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable n8t8 (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable nd8 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable np1 (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable nt2 (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable mt2 (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable mt3 (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable mt4 (number 10)
c       
       if(.not.getscalar(fname,top,top-rhs+10,lr10)) return
c       checking variable ni (number 11)
c       
       if(.not.getscalar(fname,top,top-rhs+11,lr11)) return
c       checking variable mi (number 12)
c       
       if(.not.getscalar(fname,top,top-rhs+12,lr12)) return
c       checking variable tail (number 13)
c       
       if(.not.getvectrow(fname,top,top-rhs+13,it13,m13,n13,lr13,lc13)) 
     $ return
c       checking variable head (number 14)
c       
       if(.not.getvectrow(fname,top,top-rhs+14,it14,m14,n14,lr14,lc14)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n13,n14)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr11),istk(iadr(lr11)))
       call entier(1,stk(lr12),istk(iadr(lr12)))
       call entier(n13,stk(lr13),istk(iadr(lr13)))
       call entier(n14,stk(lr14),istk(iadr(lr14)))
       if(.not.cremat(fname,top+1,0,1,1,lw12,loc12)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       nn15= int(stk(lr10))
       if(.not.cremat(fname,top+4,0,nn15,1,lw15,loc15)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr6)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr8)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr1)),1,lw19,loc19)) retur
     $ n
       if(.not.cremat(fname,top+9,0,istk(iadr(lr1)),1,lw20,loc20)) retur
     $ n
       if(.not.cremat(fname,top+10,0,istk(iadr(lr1)),1,lw21,loc21)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr1)),1,lw22,loc22)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr1)),1,lw23,loc23)) retu
     $ rn
       nn24= int(stk(lr7))
       if(.not.cremat(fname,top+13,0,nn24,1,lw24,loc24)) return
       if(.not.cremat(fname,top+14,0,istk(iadr(lr1)),1,lw25,loc25)) retu
     $ rn
       if(.not.cremat(fname,top+15,0,istk(iadr(lr3)),1,lw26,loc26)) retu
     $ rn
       if(.not.cremat(fname,top+16,0,istk(iadr(lr3)),1,lw27,loc27)) retu
     $ rn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr3)),1,lw28,loc28)) retu
     $ rn
       if(.not.cremat(fname,top+18,0,istk(iadr(lr3)),1,lw29,loc29)) retu
     $ rn
       if(.not.cremat(fname,top+19,0,istk(iadr(lr1)),1,lw30,loc30)) retu
     $ rn
       if(.not.cremat(fname,top+20,0,istk(iadr(lr1)),1,lw31,loc31)) retu
     $ rn
       nn32= int(stk(lr9))
       if(.not.cremat(fname,top+21,0,nn32,1,lw32,loc32)) return
       if(.not.cremat(fname,top+22,0,istk(iadr(lr3)),1,lw33,loc33)) retu
     $ rn
       if(.not.cremat(fname,top+23,0,istk(iadr(lr8)),1,lw34,loc34)) retu
     $ rn
       if(.not.cremat(fname,top+24,0,istk(iadr(lr1)),1,lw35,loc35)) retu
     $ rn
       if(.not.cremat(fname,top+25,0,istk(iadr(lr8)),1,lw36,loc36)) retu
     $ rn
       if(.not.cremat(fname,top+26,0,istk(iadr(lr1)),1,lw37,loc37)) retu
     $ rn
       if(.not.cremat(fname,top+27,0,istk(iadr(lr1)),1,lw38,loc38)) retu
     $ rn
       if(.not.cremat(fname,top+28,0,istk(iadr(lr8)),1,lw39,loc39)) retu
     $ rn
       if(.not.cremat(fname,top+29,0,istk(iadr(lr1)),1,lw40,loc40)) retu
     $ rn
       if(.not.cremat(fname,top+30,0,istk(iadr(lr1)),1,lw41,loc41)) retu
     $ rn
       if(.not.cremat(fname,top+31,0,istk(iadr(lr1)),1,lw42,loc42)) retu
     $ rn
       if(.not.cremat(fname,top+32,0,istk(iadr(lr1)),1,lw43,loc43)) retu
     $ rn
       if(.not.cremat(fname,top+33,0,istk(iadr(lr1)),1,lw44,loc44)) retu
     $ rn
       if(.not.cremat(fname,top+34,0,istk(iadr(lr4)),1,lw45,loc45)) retu
     $ rn
       call bmatch(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr6)),istk(
     $ iadr(lr8)),istk(iadr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)),istk(i
     $ adr(lr11)),istk(iadr(lr12)),istk(iadr(lr13)),istk(iadr(lr14)),stk
     $ (lw12),stk(lw13),stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18
     $ ),stk(lw19),stk(lw20),stk(lw21),stk(lw22),stk(lw23),stk(lw24),stk
     $ (lw25),stk(lw26),stk(lw27),stk(lw28),stk(lw29),stk(lw30),stk(lw31
     $ ),stk(lw32),stk(lw33),stk(lw34),stk(lw35),stk(lw36),stk(lw37),stk
     $ (lw38),stk(lw39),stk(lw40),stk(lw41),stk(lw42),stk(lw43),stk(lw44
     $ ),stk(lw45))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+34
c     
       if(lhs .ge. 1) then
c       --------------output variable: card
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw12)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: match
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw13)),-1,stk(lrs),-1)
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
