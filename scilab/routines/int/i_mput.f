       subroutine i_mput(fname)
c
       character*(*) fname
       integer topk,rhsk,topl,fd
       logical checkrhs,checklhs,getsmat,checkval,getscalar
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
       if(.not.checkrhs(fname,1,3)) return
       if(.not.checklhs(fname,1,1)) return
c
c       checking variable fd (number 3)
c     
       if(rhs .le. 2) then
          fd=-1
       else
          if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
          fd=stk(lr3)
       endif

c       checking variable res (number 1)
c       
       il=iadr(lstk(top-rhs+1))
       n1=istk(il+1)*istk(il+2)
       it=istk(il+3)
       l=il+4

c       checking variable type (number 2)
c 
       if(rhs .le. 1) then
          ityp(1:2)='l'//char(0)
          nlr2=1
       else
          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          call cvstr(nlr2,istk(lr2),ityp,1)
          ityp(nlr2+1:nlr2+1)=char(0)
       endif

       if(ityp(1:1).eq.'u') then
          it1=10
          ik=2
       else
          it1=0
          ik=1
       endif
       if(nlr2.eq.3) ik=3
       if(ityp(ik:ik).eq.'c') then
          it1=it1+1
       elseif(ityp(ik:ik).eq.'s') then
          it1=it1+2
       elseif(ityp(ik:ik).eq.'l') then
          it1=it1+4
       else
          buf='Incorrect integer type: '//ityp
          call error(9991)
          return
       endif
c     converting data if necessary
       if(it1.ne.it) then
          inc=1
          if(mod(it,10).lt.mod(it1,10)) then
             inc=-1
             err=sadr(l+memused(it1,n1))-lstk(bot)
             if(err.gt.0) then
                call error(17)
                return
             endif
             call tpconv(it,it1,n1, istk(l),inc,istk(l),inc)
          endif
       endif


       call mputi(fd,istk(l),n1,ityp,err)
       if(err .gt. 0) then 
        buf = fname // ' Internal Error' 
        call error(999)
        return
       endif
c
       top=topk-rhs+1
       istk(il)=0
       lstk(top+1)=sadr(il+1)
       end
