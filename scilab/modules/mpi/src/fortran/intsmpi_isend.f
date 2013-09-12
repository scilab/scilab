c SCILAB function : mpi_isend, fin = 3
       subroutine intsmpi_isend(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       integer address, n
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable dest (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable tag (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable comm (number 4)
c       
       if(rhs .le. 3) then
        top = top+1
        rhs = rhs+1
        if(.not.cremat(fname,top,0,1,1,lr4,lc4)) return
        stk(lr4)= MPI_COMM_WORLD
       endif
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     construct the pack vector for the variable number 1
c     set adress where to put the pack vector and its max
c     allowable size
       beginvar=lstk(top)
       maxsize=lstk(bot) - beginvar
       address = top-rhs+1
c       write(*,*) 'call varpack'
       call varpak(address,stk(beginvar),n,maxsize,ierr)
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
       lstk(top+1)=lstk(top)+n


c     
c       cross variable size checking
c     
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
c     WARNING: si on met la ligne en dessous, ce ECRASE la 5ieme case
c       tableau varpack. 
c     The question is WHY?????
c       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+2,0,1,1,lw8,loc8)) return
       call scimpiisend(stk(beginvar),n,
     $      stk(lstk(address)),
     $      istk(iadr(lr2)),
     $      istk(iadr(lr3)),
     $      istk(iadr(lr4)),
     $      stk(lw7),
     $      stk(lw8))
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
        call int2db(1*1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: res
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
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
