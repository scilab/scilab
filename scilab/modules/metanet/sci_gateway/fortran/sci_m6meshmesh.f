
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6meshmesh, fin = 49
       subroutine intsm6meshmesh(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,19,19)) return
       if(.not.checklhs(fname,1,8)) return
c       checking variable inseed (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable nsorc (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable nsink (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable mincst (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable maxcst (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable itsup (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable otsup (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable bhicst (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable bcap (number 10)
c       
       if(.not.getscalar(fname,top,top-rhs+10,lr10)) return
c       checking variable mincap (number 11)
c       
       if(.not.getscalar(fname,top,top-rhs+11,lr11)) return
c       checking variable maxcap (number 12)
c       
       if(.not.getscalar(fname,top,top-rhs+12,lr12)) return
c       checking variable ns (number 13)
c       
       if(.not.getscalar(fname,top,top-rhs+13,lr13)) return
c       checking variable ns2 (number 14)
c       
       if(.not.getscalar(fname,top,top-rhs+14,lr14)) return
c       checking variable ns4 (number 15)
c       
       if(.not.getscalar(fname,top,top-rhs+15,lr15)) return
c       checking variable ns10 (number 16)
c       
       if(.not.getscalar(fname,top,top-rhs+16,lr16)) return
c       checking variable ns12 (number 17)
c       
       if(.not.getscalar(fname,top,top-rhs+17,lr17)) return
c       checking variable ns20 (number 18)
c       
       if(.not.getscalar(fname,top,top-rhs+18,lr18)) return
c       checking variable ns30 (number 19)
c       
       if(.not.getscalar(fname,top,top-rhs+19,lr19)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call simple(1,stk(lr9),stk(lr9))
       call simple(1,stk(lr10),stk(lr10))
       call entier(1,stk(lr11),istk(iadr(lr11)))
       call entier(1,stk(lr12),istk(iadr(lr12)))
       if(.not.cremat(fname,top+1,0,1,1,lw13,loc13)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw14,loc14)) retur
     $ n
       nn15= int(stk(lr16))
       if(.not.cremat(fname,top+3,0,nn15,1,lw15,loc15)) return
       if(.not.cremat(fname,top+4,0,nn15,1,lw16,loc16)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr2)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+7,0,nn15,1,lw19,loc19)) return
       if(.not.cremat(fname,top+8,0,nn15,1,lw20,loc20)) return
       call entier(1,stk(lr13),istk(iadr(lr13)))
       call entier(1,stk(lr15),istk(iadr(lr15)))
       call entier(1,stk(lr17),istk(iadr(lr17)))
       call entier(1,stk(lr16),istk(iadr(lr16)))
       nn25= int(stk(lr14))
       if(.not.cremat(fname,top+9,0,nn25,1,lw25,loc25)) return
       if(.not.cremat(fname,top+10,0,istk(iadr(lr13)),1,lw26,loc26)) ret
     $ urn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr13)),1,lw27,loc27)) ret
     $ urn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr17)),1,lw28,loc28)) ret
     $ urn
       if(.not.cremat(fname,top+13,0,istk(iadr(lr15)),1,lw29,loc29)) ret
     $ urn
       if(.not.cremat(fname,top+14,0,istk(iadr(lr13)),1,lw30,loc30)) ret
     $ urn
       nn31= int(stk(lr19))
       if(.not.cremat(fname,top+15,0,nn31,1,lw31,loc31)) return
       if(.not.cremat(fname,top+16,0,istk(iadr(lr13)),1,lw32,loc32)) ret
     $ urn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr13)),1,lw33,loc33)) ret
     $ urn
       call meshmesh(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),ist
     $ k(iadr(lr4)),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),istk
     $ (iadr(lr8)),stk(lr9),stk(lr10),istk(iadr(lr11)),istk(iadr(lr12)),
     $ stk(lw13),stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(l
     $ w19),stk(lw20),istk(iadr(lr13)),istk(iadr(lr15)),istk(iadr(lr17))
     $ ,istk(iadr(lr16)),stk(lw25),stk(lw26),stk(lw27),stk(lw28),stk(lw2
     $ 9),stk(lw30),stk(lw31),stk(lw32),stk(lw33))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+17
c     
       if(lhs .ge. 1) then
c       --------------output variable: ntype
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw14)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: m
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw13)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: tail
        top=topl+3
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw15)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: head
        top=topl+4
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw16)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 5) then
c       --------------output variable: xnod
        top=topl+5
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw17)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 6) then
c       --------------output variable: ynod
        top=topl+6
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw18)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 7) then
c       --------------output variable: cap
        top=topl+7
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw19)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 8) then
c       --------------output variable: cost
        top=topl+8
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw20)),-1,stk(lrs),-1)
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
       if(lhs .ge. 5) then
        call copyobj(fname,topl+5,topk+5)
       endif
       if(lhs .ge. 6) then
        call copyobj(fname,topl+6,topk+6)
       endif
       if(lhs .ge. 7) then
        call copyobj(fname,topl+7,topk+7)
       endif
       if(lhs .ge. 8) then
        call copyobj(fname,topl+8,topk+8)
       endif
       top=topk+lhs
       return
       end
c
