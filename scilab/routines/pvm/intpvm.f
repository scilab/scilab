c     Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved 
c SCILAB function : pvmjoingroup, fin = 1
       subroutine intspvmjoingroup(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl,res
       logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call scipvmjoingroup(buf(lbufi1:lbuff1),nlr1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: inum
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmlvgroup, fin = 2
       subroutine intspvmlvgroup(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call scipvmlvgroup(buf(lbufi1:lbuff1),nlr1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: size
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmgsize, fin = 3
       subroutine intspvmgsize(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call scipvmgsize(buf(lbufi1:lbuff1),nlr1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: size
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmgettid, fin = 4
       subroutine intspvmgettid(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getscalar,bufstore
       logical cremat
       integer inum
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable inum (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       inum = stk(lr2)
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
c       call scipvmgettid(buf(lbufi1:lbuff1),nlr1,istk(iadr(lr2)),stk(l
c     $ w4))
       call scipvmgettid(buf(lbufi1:lbuff1),nlr1,inum,stk(lw4))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: tid
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmgetinst, fin = 5
       subroutine intspvmgetinst(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getscalar
       logical bufstore,cremat
       integer tid
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable tid (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       tid = stk(lr2)
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
c       call scipvmgetinst(buf(lbufi1:lbuff1),nlr1,istk(iadr(lr2)),stk(
c     $ lw4))
       call scipvmgetinst(buf(lbufi1:lbuff1),nlr1,tid,stk(lw4))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: inum
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmbarrier, fin = 6
       subroutine intspvmbarrier(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getscalar
       logical bufstore,cremat
       integer count
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable count (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       count = stk(lr2)
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
c       call scipvmbarrier(buf(lbufi1:lbuff1),nlr1,istk(iadr(lr2)),stk(
c     $ lw4))
       call scipvmbarrier(buf(lbufi1:lbuff1),nlr1,count,stk(lw4))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmbcast, fin = 7
       subroutine intspvmbcast(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr, tag, maxsize, n
       integer topk,rhsk,topl, address
       logical checkrhs,checklhs,getsmat,checkval,getscalar
       logical bufstore,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable group (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable buff (number 2)
c       
c       if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable msgtag (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
       tag=stk(lr3)
c     construct the pack vector for the variable number 2
c     set adress where to put the pack vector and its max
c     allowable size
       ilpack=iadr(lstk(top))
       maxsize=iadr(lstk(bot)) - ilpack
       address = top-rhs+2
       call varpak(address,istk(ilpack),n,maxsize,ierr)
       if(ierr .gt. 0) then 
          buf = fname // ' Unknow type or not yet implemented' 
          call error(999)
          return
       endif
c
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c
c     set correct size for the pack vect
c      lstk(top+1)=lstk(top)+n
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c

       call scipvmbcast(buf(lbufi1:lbuff1),nlr1,
     $      istk(ilpack),n,stk(lstk(address)),tag,res)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        stk(lrs)=res
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmtasks, fin = 8
       subroutine intspvmtasks(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat,getscalar,crepointer,listcremat
       logical lcrestringmatfromC
       integer where
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable where (number 1)
c       
       if(rhs .le. 0) then
        top = top+1
        rhs = rhs+1
        if(.not.cremat(fname,top,0,1,1,lr1,lc1)) return
        stk(lr1)= 0
       endif
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
c       call entier(1,stk(lr1),istk(iadr(lr1)))
       where = stk(lr1)
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
       if(.not.cremat(fname,top+2,0,1,1,lw3,loc3)) return
       if(.not.cremat(fname,top+3,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+4,0,1,1,lw5,loc5)) return
       if(.not.cremat(fname,top+5,0,1,1,lw6,loc6)) return
       mm7=1
       if(.not.crepointer(fname,top+6,lw7)) return
       if(.not.cremat(fname,top+7,0,1,1,lw9,loc9)) return
c       call scipvmtasks(istk(iadr(lr1)),stk(lw2),stk(lw3),stk(lw4),stk
c     $ (lw5),stk(lw6),stk(lw7),ne3,stk(lw9))
       call scipvmtasks(where,stk(lw2),stk(lw3),stk(lw4),stk
     $ (lw5),stk(lw6),stk(lw7),ne3,stk(lw9))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+7
c       Creation of output list
       top=topl+1
       call crelist(top,7,lw)
c     
c       Element 1: tid
       if(.not.listcremat(fname,top,1,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw3),stk(lrs))
c     
c       Element 2: ptid
       if(.not.listcremat(fname,top,2,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw4),stk(lrs))
c     
c       Element 3: dtid
       if(.not.listcremat(fname,top,3,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw5),stk(lrs))
c     
c       Element 4: flag
       if(.not.listcremat(fname,top,4,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw6),stk(lrs))
c     
c       Element 5: name
       if(.not.lcrestringmatfromC(fname,top,5,lw,lw7,mm7,ne3)) return
c     
c       Element 6: ntask
       if(.not.listcremat(fname,top,6,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
c     
c       Element 7: info
       if(.not.listcremat(fname,top,7,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw9)),-1,stk(lrs),-1)
c     
c     Putting in order the stack
       call copyobj(fname,topl+1,topk+1)
       top=topk+1
       return
       end
c
c SCILAB function : pvmconfig, fin = 9
       subroutine intspvmconfig(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat,crepointer,listcremat
       logical lcrestringmatfromC
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       if(.not.cremat(fname,top+2,0,1,1,lw2,loc2)) return
       if(.not.cremat(fname,top+3,0,1,1,lw3,loc3)) return
       mm4=1
       if(.not.crepointer(fname,top+4,lw4)) return
       if(.not.crepointer(fname,top+5,lw5)) return
       if(.not.cremat(fname,top+6,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+7,0,1,1,lw8,loc8)) return
       call scipvmconfig(stk(lw1),stk(lw2),stk(lw3),stk(lw4),stk(lw5),
     $ stk(lw6),ne3,stk(lw8))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+7
c       Creation of output list
       top=topl+1
       call crelist(top,7,lw)
c     
c       Element 1: nhost
       if(.not.listcremat(fname,top,1,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
c     
c       Element 2: narch
       if(.not.listcremat(fname,top,2,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
c     
c       Element 3: dtid
       if(.not.listcremat(fname,top,3,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw3),stk(lrs))
c     
c       Element 4: name
       if(.not.lcrestringmatfromC(fname,top,4,lw,lw4,mm4,ne3)) return
c     
c       Element 5: arch
       if(.not.lcrestringmatfromC(fname,top,5,lw,lw5,mm4,ne3)) return
c     
c       Element 6: speed
       if(.not.listcremat(fname,top,6,lw,0,1,ne3,lrs,lcs)) return
       call cintf(1*ne3,stk(lw6),stk(lrs))
c     
c       Element 7: info
       if(.not.listcremat(fname,top,7,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
c     
c     Putting in order the stack
       call copyobj(fname,topl+1,topk+1)
       top=topk+1
       return
       end
c
c SCILAB function : pvmaddhosts, fin = 10
       subroutine intspvmaddhosts(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,crestringv,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable hosts (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1,1)) return
c     
c       cross variable size checking
c     
       if(.not.crestringv(fname,top+1,lr1-5-m1*n1,lw1)) return
       if(.not.cremat(fname,top+2,0,n1,1,lw3,loc3)) return
       call scipvmaddhosts(stk(lw1),n1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: infos
        top=topl+1
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmdelhosts, fin = 11
       subroutine intspvmdelhosts(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,crestringv,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable hosts (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1,1)) return
c     
c       cross variable size checking
c     
       if(.not.crestringv(fname,top+1,lr1-5-m1*n1,lw1)) return
       if(.not.cremat(fname,top+2,0,n1,1,lw3,loc3)) return
       call scipvmdelhosts(stk(lw1),n1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: infos
        top=topl+1
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmparent, fin = 12
       subroutine intspvmparent(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmparent(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmtidtohost, fin = 13
       subroutine intspvmtidtohost(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       integer tid
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tid (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
c       call entier(1,stk(lr1),istk(iadr(lr1)))
       tid = stk(lr1)
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
c       call scipvmtidtohost(istk(iadr(lr1)),stk(lw2))
       call scipvmtidtohost(tid,stk(lw2))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmsettimer, fin = 14
       subroutine intspvmsettimer(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmsettimer(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmgettimer, fin = 15
       subroutine intspvmgettimer(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmgettimer(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call unsfdcopy(1*1,stk(lw1),1,stk(lrs),1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmmytid, fin = 16
       subroutine intspvmmytid(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmmytid(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmexit, fin = 17
       subroutine intspvmexit(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmexit(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmkill, fin = 18
       subroutine intspvmkill(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tids (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     
c       cross variable size checking
c     
c       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,n1,1,lw3,loc3)) return
       call entier(n1,stk(lr1),istk(iadr(lstk(top+2))))
c       call scipvmkill(istk(iadr(lr1)),n1,stk(lw3))
       call scipvmkill(istk(iadr(lstk(top+2))),n1,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmspawn, fin = 19
       subroutine intspvmspawn(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getscalar,cresmat2
       logical bufstore,cremat
       integer ntask
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable task (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable ntask (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable win (number 3)
c       
       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        nlr3 = 1
        if(.not.cresmat2(fname,top,nlr3,lr3)) return
        call cvstr(nlr3,istk(lr3),'w',0)
       endif
       if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
       if(.not.checkval(fname,m3*n3,1)) return
c       checking variable where (number 4)
c       
       if(rhs .le. 3) then
        top = top+1
        rhs = rhs+1
        nlr4 = 4
        if(.not.cresmat2(fname,top,nlr4,lr4)) return
        call cvstr(nlr4,istk(lr4),'null',0)
       endif
       if(.not.getsmat(fname,top,top-rhs+4,m4,n4,1,1,lr4,nlr4)) return
       if(.not.checkval(fname,m4*n4,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
       if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr3,nlr3)) return
       if(.not.bufstore(fname,lbuf,lbufi5,lbuff5,lr4,nlr4)) return
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       ntask = stk(lr2)
c       if(.not.cremat(fname,top+1,0,istk(iadr(lr2)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+1,0,ntask,1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,1,1,lw9,loc9)) return
c       call scipvmspawn(buf(lbufi1:lbuff1),nlr1,buf(lbufi3:lbuff3),nlr
c     $ 3,buf(lbufi5:lbuff5),nlr4,istk(iadr(lr2)),stk(lw8),stk(lw9))
       call scipvmspawn(buf(lbufi1:lbuff1),nlr1,buf(lbufi3:lbuff3),nlr
     $ 3,buf(lbufi5:lbuff5),nlr4,ntask,stk(lw8),stk(lw9))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: tids
        top=topl+1
c        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        if(.not.cremat(fname,top,0,1,ntask,lrs,lcs)) return
c        call int2db(1*istk(iadr(lr2)),istk(iadr(lw8)),-1,stk(lrs),-1)
        call int2db(1*ntask,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw9)),-1,stk(lrs),-1)
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
c SCILAB function : pvmspawnindependent, fin = 20
       subroutine intspvmspawnindependent(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getscalar,cresmat2
       logical bufstore,cremat
       integer ntask
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable task (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable ntask (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable where (number 3)
c       
       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        nlr3 = 4
        if(.not.cresmat2(fname,top,nlr3,lr3)) return
        call cvstr(nlr3,istk(lr3),'null',0)
       endif
       if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
       if(.not.checkval(fname,m3*n3,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       ntask = stk(lr2)
       if(.not.bufstore(fname,lbuf,lbufi4,lbuff4,lr3,nlr3)) return
c     if(.not.cremat(fname,top+1,0,istk(iadr(lr2)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+1,0,ntask,1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,1,1,lw7,loc7)) return
c       call scipvmspawnindependent(buf(lbufi1:lbuff1),nlr1,istk(iadr(
c     $ lr2)),buf(lbufi4:lbuff4),nlr3,stk(lw6),stk(lw7))
       call scipvmspawnindependent(buf(lbufi1:lbuff1),nlr1,ntask,
     $ buf(lbufi4:lbuff4),nlr3,stk(lw6),stk(lw7))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: tids
        top=topl+1
c        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        if(.not.cremat(fname,top,0,1,ntask,lrs,lcs)) return
c     call int2db(1*istk(iadr(lr2)),istk(iadr(lw6)),-1,stk(lrs),-1)
        call int2db(1*ntask,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
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
c
c SCILAB function : pvmrecv, fin = 21
      subroutine intspvmrecv(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer beginvar, tag, tid, iis
      integer topk, rhsk
      logical checkrhs, checklhs, getscalar, cremat
c
      rhs = max(0,rhs)
c     
      topk = top 
      rhsk = rhs 
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,4)) return
c     
c     checking variable tid (number 1)
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      tid=stk(lr1)
c     
c     checking variable tag (number 2)
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      tag=stk(lr2)
c
c     free input variable at the top of the stack
      top=top-rhs+1
c
c     set adress where to put the received variable and its max
c     allowable size
      beginvar=lstk(top)
      maxsize=lstk(bot)-beginvar
c
c     receive the variable
      call scipvmrecv(stk(beginvar),maxsize,n,tid,tag,iis)
c     ATTENTION scipvmrecv ne doit pas ecrire plus de maxsize double
c     dans stk(beginvar)
      if(err .gt. 0) then 
         buf = fname // ' Internal Error' 
         call error(999)
         return
      endif
c
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c
c     set correct size for the received variable
      lstk(top+1)=lstk(top)+n

c     MA MODIF: j incremente top ici et non pas apres la creation de
c      la variable de retour..
c     
      if(lhs .ge. 2) then
      top=top+1
c     create variable status at the top of the returned variables
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
c     set value of the status variable
         stk(lrs)=iis
c     top=top+1
      endif
      if(lhs .ge. 3) then
c     --------------output variable: tid
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=tid
      endif
      if(lhs .ge. 4) then
c       --------------output variable: tag
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=tag
      endif
c     
c     clear status variable if lhs is one
c      if(lhs.eq.1) top=top-1
c      return
      end

c
c SCILAB function : pvmsend, fin = 22
       subroutine intspvmsend(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, address, tag, n
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c     checking variable tids (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     checking variable buff (number 2)
c       
c     if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c     checking variable tag (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
       tag=stk(lr3)
c     construct the pack vector for the variable number 2
c     set adress where to put the pack vector and its max
c     allowable size
       ilpack=iadr(lstk(top)+n1)
       maxsize=iadr(lstk(bot)) - ilpack
       address = top-rhs+2
c       write(*,*) 'call varpack'
       call varpak(address,istk(ilpack),n,maxsize,ierr)
c       write(*,*) 'callback varpack'
       if(ierr .gt. 0) then 
          buf = fname // ' Unknow type or not yet implemented' 
          call error(999)
          return
       endif
c
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c
c     set correct size for the pack vect
c      lstk(top+1)=lstk(top)+n

c     
c     cross variable size checking
c     
c       call entier(n1,stk(lr1),istk(iadr(lr1)))
      call entier(n1,stk(lr1),istk(iadr(lstk(top))))
c
c     Gestion du passage par @ du buffer a envoyer.
       iiladdress =  lstk(address)
       iladdress=iadr(lstk(address))
       if(istk(iladdress).lt.0) then 
          iiladdress=istk(iladdress+1)
       endif
       
c       call scipvmsend(istk(iadr(lr1)),n1,
       call scipvmsend(istk(iadr(lstk(top))),n1,
     $      istk(ilpack),n,
     $      stk(iiladdress),
     $      tag,is)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        stk(lrs)=is
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmrecvvar, fin = 23
       subroutine intspvmrecvvar(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getsmat,checkval,bufstore
       logical cremat
       integer tid, tag
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tid (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable tag (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable buff (number 3)
c       
       if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
       if(.not.checkval(fname,m3*n3,1)) return
c     
c       cross variable size checking
c     
c       call entier(1,stk(lr1),istk(iadr(lr1)))
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       tid = stk(lr1)
       tag = stk(lr2)
       if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr3,nlr3)) return
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
c       call scipvmrecvvar(istk(iadr(lr1)),istk(iadr(lr2)),buf(lbufi3:
c     $ lbuff3),stk(lw4))
       call scipvmrecvvar(tid,tag,buf(lbufi3:lbuff3),stk(lw4))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmsendvar, fin = 24
       subroutine intspvmsendvar(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getsmat,checkval,getscalar
       logical bufstore,cremat
       integer tag
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tids (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable buff (number 2)
c       
       if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2)) return
       if(.not.checkval(fname,m2*n2,1)) return
c       checking variable tag (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
c       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr2,nlr2)) return
c       call entier(1,stk(lr3),istk(iadr(lr3)))
       tag = stk(lr3)
       if(.not.cremat(fname,top+1,0,1,1,lw5,loc5)) return
       call entier(n1,stk(lr1),istk(iadr(lstk(top+2))))
c       call scipvmsendvar(istk(iadr(lr1)),n1,buf(lbufi3:lbuff3),istk(
c     $ iadr(lr3)),stk(lw5))
c       call scipvmsendvar(istk(iadr(lr1)),n1,buf(lbufi3:lbuff3),tag,
       call scipvmsendvar(istk(iadr(lstk(top+2))),n1,buf(lbufi3:lbuff3),tag,
     $ stk(lw5))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvmreduce, fin = 25
       subroutine intspvmreduce(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,getmat,getscalar
       logical bufstore,cremat
       integer msgtag, rootginst
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable func (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c       checking variable buff (number 2)
c       
       if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable msgtag (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable group (number 4)
c       
       if(.not.getsmat(fname,top,top-rhs+4,m4,n4,1,1,lr4,nlr4)) return
       if(.not.checkval(fname,m4*n4,1)) return
c       checking variable rootginst (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c       call entier(1,stk(lr3),istk(iadr(lr3)))
       msgtag = stk(lr3)
       if(.not.bufstore(fname,lbuf,lbufi7,lbuff7,lr4,nlr4)) return
c       call entier(1,stk(lr5),istk(iadr(lr5)))
       rootginst = stk(lr5)
       if(.not.cremat(fname,top+1,0,1,1,lw10,loc10)) return
       call scipvmreduce(buf(lbufi1:lbuff1),nlr1,stk(lr2),m2,n2,msgtag,
     $ buf(lbufi7:lbuff7),nlr4,rootginst,stk(lw10))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: buff
        top=topl+1
        if(.not.cremat(fname,top,it2,m2,n2,lrs,lcs)) return
c     PAS TRES PROPRE.
c     A remplacer par un copy(type,size... ou par un dxcopy...
        call unsfdcopy(n2*m2*(it2+1),stk(lr2),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw10)),-1,stk(lrs),-1)
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
c SCILAB function : pvm_start, fin = 26
       subroutine intspvmstart(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cresmat2,getsmat,checkval,cremat,bufsto
     $ re
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable hostfile (number 1)
c       
       if(rhs .le. 0) then
        top = top+1
        rhs = rhs+1
        nlr1 = 4
        if(.not.cresmat2(fname,top,nlr1,lr1)) return
        call cvstr(nlr1,istk(lr1),'null',0)
       endif
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr1,nlr1)) return
       call scipvmstart(stk(lw1),buf(lbufi2:lbuff2),nlr1)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvm_halt, fin = 27
       subroutine intspvmhalt(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,0)) return
       if(.not.checklhs(fname,1,1)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call scipvmhalt(stk(lw1))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end

c
c SCILAB function : pvm_error, fin = 28
       subroutine intspvmerror(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,crepointer,crestringmatfromC
       integer num
c
       iadr(l)=l+l-1
c
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable err (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
       num = stk(lr1)
c       call entier(1,stk(lr1),istk(iadr(lr1)))
       mm2=1
       if(.not.crepointer(fname,top+1,lw2)) return
c       call scipvmerror(istk(iadr(lr1)),stk(lw2))
       call scipvmerror(num,stk(lw2))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.crestringmatfromC(fname,top,lw2,mm2,mm2)) return
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvm_sci2f77, fin = 29
       subroutine intspvmsci2f77(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr, iladdress, address
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable var (number 1)
c       
c       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
       address = top-rhs+1
       iladdress=iadr(lstk(address))
c
       call scipvmscitof77(top - rhs + 1)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+0
c     
c       if(lhs .ge. 1) then
c       --------------output variable: var
c        top=topl+1
c        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
c        call unsfdcopy(1*1,stk(lr1),1,stk(lrs),1)
c       endif
c     Putting in order the stack
c     if the variable is passed by value copy it on the result
       if(lhs .ge. 1.and.istk(iladdress).ge.0) then
        call copyobj(fname,topl,topk+1)
        top=topk+lhs
c     else we return the null variable
       else
          top=topk+lhs
          il = iadr(lstk(top))
          istk(il) = 0
          lstk(top+1) = lstk(top) + 1
c          top=topk-lhs
       endif
c       top=topk+lhs
       return
       end
c
c SCILAB function : pvm_f772sci, fin = 30
       subroutine intspvmf772sci(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr, iladdress, address
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable var (number 1)
c       
c       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
       address = top-rhs+1
       iladdress=iadr(lstk(address))
c
       call scipvmf77tosci(top - rhs + 1)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+0
c     
c       if(lhs .ge. 1) then
c       --------------output variable: var
c        top=topl+1
c        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
c        call unsfdcopy(1*1,stk(lr1),1,stk(lrs),1)
c       endif
c     Putting in order the stack
c     if the variable is passed by value copy it on the result
       if(lhs .ge. 1.and.istk(iladdress).ge.0) then
        call copyobj(fname,topl,topk+1)
        top=topk+lhs
c     else we return the null variable
       else
          top=topk+lhs
          il = iadr(lstk(top))
          istk(il) = 0
          lstk(top+1) = lstk(top) + 1
c          top=topk-lhs
       endif
c      top=topk+lhs
       return
       end
c
c SCILAB function : pvm_probe, fin = 31
       subroutine intspvm_probe(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       integer tid, msgtag
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tid (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable msgtag (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c     
c       cross variable size checking
c     
c       call entier(1,stk(lr1),istk(iadr(lr1)))
c       call entier(1,stk(lr2),istk(iadr(lr2)))
       tid = stk(lr1)
       msgtag = stk(lr2)
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
c       call scipvmprobe(istk(iadr(lr1)),istk(iadr(lr2)),stk(lw3))
       call scipvmprobe(tid,msgtag,stk(lw3))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : pvm_bufinfo, fin = 32
       subroutine intspvm_bufinfo(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       integer bufinfo
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable bufid (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c     
c       cross variable size checking
c     
c       call entier(1,stk(lr1),istk(iadr(lr1)))
       bufinfo = stk(lr1)
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
       if(.not.cremat(fname,top+2,0,1,1,lw3,loc3)) return
       if(.not.cremat(fname,top+3,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+4,0,1,1,lw5,loc5)) return
c       call scipvmbufinfo(istk(iadr(lr1)),stk(lw2),stk(lw3),stk(lw4),stk(lw
c     $ 5))
       call scipvmbufinfo(bufinfo,stk(lw2),stk(lw3),stk(lw4),stk(lw5))
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: bytes
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: msgtag
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: tid
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: res
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw5)),-1,stk(lrs),-1)
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
       top=topk+lhs
       return
       end
c
c  interface function 
c   ********************
       subroutine intpvm
       include '../stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22
     $      ,23
     $      ,24
     $      ,25
     $      ,26,27,28,29,30,31,32) fin
       return
1      call intspvmjoingroup('pvm_joingroup')
       return
2      call intspvmlvgroup('pvm_lvgroup')
       return
3      call intspvmgsize('pvm_gsize')
       return
4      call intspvmgettid('pvm_gettid')
       return
5      call intspvmgetinst('pvm_getinst')
       return
6      call intspvmbarrier('pvm_barrier')
       return
7      call intspvmbcast('pvm_bcast')
       return
8      call intspvmtasks('pvm_tasks')
       return
9      call intspvmconfig('pvm_config')
       return
10      call intspvmaddhosts('pvm_addhosts')
       return
11      call intspvmdelhosts('pvm_delhosts')
       return
12      call intspvmparent('pvm_parent')
       return
13      call intspvmtidtohost('pvm_tidtohost')
       return
14      call intspvmsettimer('pvmset_timer')
       return
15      call intspvmgettimer('pvm_get_timer')
       return
16      call intspvmmytid('pvm_mytid')
       return
17      call intspvmexit('pvm_exit')
       return
18      call intspvmkill('pvm_kill')
       return
19      call intspvmspawn('pvm_spawn')
       return
20      call intspvmspawnindependent('pvm_spawn_independent')
       return
21      call intspvmrecv('pvm_recv')
       return
22      call intspvmsend('pvm_send')
       return
23      call intspvmrecvvar('pvm_recv_var')
       return
24      call intspvmsendvar('pvm_send_var')
       return
25      call intspvmreduce('pvm_reduce')
        return
26      call intspvmstart('pvm_start')
       return
27      call intspvmhalt('pvm_halt')
       return
28      call intspvmerror('pvm_error')
       return
29      call intspvmsci2f77('pvm_sci2f77')
       return
30      call intspvmf772sci('pvm_f772sci')
       return
31      call intspvm_probe('pvm_probe')
       return
32      call intspvm_bufinfo('pvm_bufinfo')
       return
       end
