c SCILAB function : mpi_send, fin = 1
       subroutine intsmpi_send(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       integer address,n,beginvar
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,4)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable buff (number 1)
c       
c       if(.not.getmat(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
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
c       call entier(1,stk(lr2),istk(iadr(lr2)))
c       call entier(1,stk(lr3),istk(iadr(lr3)))
c     WARNING: si on met la ligne en dessous, ce ECRASE la 5ieme case
c       tableau varpack. 
c     The question is WHY?????
c       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw7,loc7)) return
       call scimpisend(stk(beginvar),n,
     $      stk(lstk(address)),
     $      istk(iadr(lr2)),
     $      istk(iadr(lr3)),
     $      istk(iadr(lr4)),
     $      stk(lw7))
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
        call int2db(1*1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
