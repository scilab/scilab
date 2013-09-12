c SCILAB function : mpi_comm_rank, fin = 7
       subroutine intsmpi_comm_rank(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cremat,getscalar
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,1)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable comm (number 1)
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
       call entier(1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
       if(.not.cremat(fname,top+2,0,1,1,lw3,loc3)) return
       call MPI_COMM_RANK(istk(iadr(lr1)),stk(lw2),stk(lw3))
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
c       --------------output variable: myid
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw3)),-1,stk(lrs),-1)
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
