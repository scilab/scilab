       subroutine i_mget(fname)
c
       character*(*) fname
       integer topk,rhsk,topl,fd
       logical checkrhs,checklhs,getscalar,getsmat,checkval
       character*4 ityp
       include '../stack.h'
c
       integer iadr, sadr
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,3)) return
       if(.not.checklhs(fname,1,1)) return
c
c       checking variable n (number 1)
       if(rhs .le. 0) then
          n = 1
       else
          if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
          n=stk(lr1)
       endif
c
c       checking variable type (number 2)
       if(rhs .le. 1) then
          ityp(1:2)='i'//char(0)
          nlr2=1
       else
          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          call cvstr(nlr2,istk(lr2),ityp,1)
          ityp(nlr2+1:nlr2+1)=char(0)
       endif
       if(ityp(1:1).eq.'u') then
          it=10
          ik=2
       else
          it=0
          ik=1
       endif
c       if(nlr2.eq.3) ik=3
       if(ityp(ik:ik).eq.'c') then
          it=it+1
       elseif(ityp(ik:ik).eq.'s') then
          it=it+2
       elseif(ityp(ik:ik).eq.'i') then
          it=it+4
       else
          buf='Incorrect integer type: '//ityp
          call error(9991)
          return
       endif
c
c       checking variable fd (number 3)
       if(rhs .le. 2) then
          fd=-1
       else
          if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
          fd=stk(lr3)
       endif

       top=topk-rhs+1
       il=iadr(lstk(top))
       istk(il)=8
       istk(il+1)=1
       istk(il+2)=n
       istk(il+3)=it
       l=il+4
       err=sadr(l+memused(it,n))-lstk(bot)
       if(err.gt.0) then 
          call error(17)
          return
       endif

       err=0
       call mgetnc(fd,istk(l),n,ityp,err)
       if(err .gt. 0) then 
          buf = 'mgeti: Internal Error' 
          call error(999)
          return
       endif
       if(err.lt.0) n = -err -1
       if(n.eq.0) then
          istk(il)=1
          istk(il+1)=0
          istk(il+2)=0
          istk(il+3)=0
          lstk(top+1)=sadr(il+4)+1
       else
          istk(il+2) = n
          lstk(top+1) = sadr(l+memused(it,n))
       endif
       return
       end
