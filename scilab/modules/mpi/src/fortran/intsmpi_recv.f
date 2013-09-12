c SCILAB function : mpi_recv, fin = 2
       subroutine intsmpi_recv(fname)
c
       character*(*) fname
       include 'stack.h'
c      
       integer iadr, sadr
       integer beginvar, maxsize, n
       integer source, tag, comm, status(2), result
       integer topk,rhsk
       logical checkrhs,checklhs,getscalar,cremat
c     
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,3)) return
       if(.not.checklhs(fname,1,3)) return
c     checking variable source (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
       source = stk(lr1)       
c     checking variable tag (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
       tag = stk(lr2)
c     checking variable comm (number 3)
c       
       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        if(.not.cremat(fname,top,0,1,1,lr3,lc3)) return
        stk(lr3)= MPI_COMM_WORLD
       endif
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
       comm = stk(lr3)
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
       call scimpirecv(stk(beginvar), maxsize, n,
     $      source, tag, comm, status, result)
c     ATTENTION scimpirecv ne doit pas ecrire plus de maxsize double
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
c     MA MODIF: j'incremente top ici et non pas apres la creation de
c     la variable de retour...
       top=top+1
c
c     
c     create variable status at the top of the returned variables
      if(.not.cremat(fname,top,0,1,7,lrs,lcs)) return
c     set value of the status variable
      call int2db(2,status,-1,stk(lrs),-1)
      top=top+1
c     create variable status at the top of the returned variables
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
c     set value of the status variable
      stk(lrs)=result
c     clear res & status variable if lhs result 1
      if(lhs.eq.1) top=top-2
c     clear res variable if lhs result 2
      if(lhs.eq.2) top=top-1
      return
      end
c
