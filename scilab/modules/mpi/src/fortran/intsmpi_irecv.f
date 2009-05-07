c SCILAB function : mpi_irecv, fin = 4
       subroutine intsmpi_irecv(fname)
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
       if(.not.checkrhs(fname,2,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable source (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable tag (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable comm (number 3)
c       
       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        if(.not.cremat(fname,top,0,1,1,lr3,lc3)) return
        stk(lr3)= MPI_COMM_WORLD
       endif
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,1,1,lw5,loc5)) return
       call scimpiirecv(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),
     $ stk(lw4),stk(lw5))
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
c       --------------output variable: request
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
        top=topl+2
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
       top=topk+lhs
       return
       end
