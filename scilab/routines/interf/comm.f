c fin = 1 
c SCILAB function : GetMsg
c     Copyright INRIA
      subroutine intsGetMsg
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 0) then
          call error(39)
          return
        endif
        if (lhs .gt. 3) then
          call error(41)
          return
        endif
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lw1=lw
        lw=lw+1
        lw3=lw
        lw=lw+1
        lw5=lw
        lw=lw+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call getmsg(stk(lw1),ne1,stk(lw3),ne3,stk(lw5),ne5)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c     
        if(lhs .ge. 1) then
c     
c       output variable: type
c     
        top=top+1
        ilw=iadr(lw)
        err=sadr(ilw+6+ne3)-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        istk(ilw)=10
        istk(ilw+1)=1
        istk(ilw+2)=1
        istk(ilw+3)=0
        istk(ilw+4)=1
        istk(ilw+5)=ne3+1
        call cchar(ne3,stk(lw3),istk(ilw+6))
        lw=sadr(ilw+6+ne3)
        lstk(top+1)=lw-mv
        endif
c     
        if(lhs .ge. 2) then
c     
c       output variable: msg
c     
        top=top+1
        ilw=iadr(lw)
        err=sadr(ilw+6+ne5)-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        istk(ilw)=10
        istk(ilw+1)=1
        istk(ilw+2)=1
        istk(ilw+3)=0
        istk(ilw+4)=1
        istk(ilw+5)=ne5+1
        call cchar(ne5,stk(lw5),istk(ilw+6))
        lw=sadr(ilw+6+ne5)
        lstk(top+1)=lw-mv
        endif
c     
        if(lhs .ge. 3) then
c     
c       output variable: appli
c     
        top=top+1
        ilw=iadr(lw)
        err=sadr(ilw+6+ne1)-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        istk(ilw)=10
        istk(ilw+1)=1
        istk(ilw+2)=1
        istk(ilw+3)=0
        istk(ilw+4)=1
        istk(ilw+5)=ne1+1
        call cchar(ne1,stk(lw1),istk(ilw+6))
        lw=sadr(ilw+6+ne1)
        lstk(top+1)=lw-mv
        endif
c     
c       putting in order the stack
c     
        call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
        return
      end
c
c fin = 2 
c SCILAB function : SendMsg
      subroutine intsSendMsg
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 2) then
          call error(39)
          return
        endif
        if (lhs .ne. 1) then
          call error(41)
          return
        endif
c       checking variable type (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 10) then
          err = 1
          call error(55)
          return
        endif
        if (istk(il1+1)*istk(il1+2) .ne. 1) then
          err = 1
          call error(89)
          return
        endif
        n1 = istk(il1+5)-1
        l1 = il1+6
c       checking variable msg (number 2)
c       
        il2 = iadr(lstk(top-rhs+2))
        if (istk(il2) .ne. 10) then
          err = 2
          call error(55)
          return
        endif
        if (istk(il2+1)*istk(il2+2) .ne. 1) then
          err = 2
          call error(89)
          return
        endif
        n2 = istk(il2+5)-1
        l2 = il2+6
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lbuf1 = lbuf
        call cvstr(n1,istk(l1),buf(lbuf1:lbuf1+n1-1),1)
        lbuf = lbuf+n1+1
        lbuf3 = lbuf
        call cvstr(n2,istk(l2),buf(lbuf3:lbuf3+n2-1),1)
        lbuf = lbuf+n2+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call sendmsg(buf(lbuf1:lbuf1+n1-1),n1,buf(lbuf3:lbuf3+n2-1),n2)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c       no output variable
        top=top+1
        il=iadr(l0)
        istk(il)=0
        lstk(top+1)=l0+1
        return
      end
c
c fin = 3 
c SCILAB function : ExecAppli
      subroutine intsExecAppli
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 3) then
          call error(39)
          return
        endif
        if (lhs .ne. 1) then
          call error(41)
          return
        endif
c       checking variable command (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 10) then
          err = 1
          call error(55)
          return
        endif
        if (istk(il1+1)*istk(il1+2) .ne. 1) then
          err = 1
          call error(89)
          return
        endif
        n1 = istk(il1+5)-1
        l1 = il1+6
c       checking variable host (number 2)
c       
        il2 = iadr(lstk(top-rhs+2))
        if (istk(il2) .ne. 10) then
          err = 2
          call error(55)
          return
        endif
        if (istk(il2+1)*istk(il2+2) .ne. 1) then
          err = 2
          call error(89)
          return
        endif
        n2 = istk(il2+5)-1
        l2 = il2+6
c       checking variable appli (number 3)
c       
        il3 = iadr(lstk(top-rhs+3))
        if (istk(il3) .ne. 10) then
          err = 3
          call error(55)
          return
        endif
        if (istk(il3+1)*istk(il3+2) .ne. 1) then
          err = 3
          call error(89)
          return
        endif
        n3 = istk(il3+5)-1
        l3 = il3+6
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lbuf1 = lbuf
        call cvstr(n1,istk(l1),buf(lbuf1:lbuf1+n1-1),1)
        lbuf = lbuf+n1+1
        lbuf3 = lbuf
        call cvstr(n2,istk(l2),buf(lbuf3:lbuf3+n2-1),1)
        lbuf = lbuf+n2+1
        lbuf5 = lbuf
        call cvstr(n3,istk(l3),buf(lbuf5:lbuf5+n3-1),1)
        lbuf = lbuf+n3+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call execappli(buf(lbuf1:lbuf1+n1-1),n1,buf(lbuf3:lbuf3+n2-1),n2
     & ,buf(lbuf5:lbuf5+n3-1),n3)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c       no output variable
        top=top+1
        il=iadr(l0)
        istk(il)=0
        lstk(top+1)=l0+1
        return
      end
c
c fin = 4 
c SCILAB function : CreateLink
      subroutine intsCreateLink
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 2) then
          call error(39)
          return
        endif
        if (lhs .ne. 1) then
          call error(41)
          return
        endif
c       checking variable appli1 (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 10) then
          err = 1
          call error(55)
          return
        endif
        if (istk(il1+1)*istk(il1+2) .ne. 1) then
          err = 1
          call error(89)
          return
        endif
        n1 = istk(il1+5)-1
        l1 = il1+6
c       checking variable appli2 (number 2)
c       
        il2 = iadr(lstk(top-rhs+2))
        if (istk(il2) .ne. 10) then
          err = 2
          call error(55)
          return
        endif
        if (istk(il2+1)*istk(il2+2) .ne. 1) then
          err = 2
          call error(89)
          return
        endif
        n2 = istk(il2+5)-1
        l2 = il2+6
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lbuf1 = lbuf
        call cvstr(n1,istk(l1),buf(lbuf1:lbuf1+n1-1),1)
        lbuf = lbuf+n1+1
        lbuf3 = lbuf
        call cvstr(n2,istk(l2),buf(lbuf3:lbuf3+n2-1),1)
        lbuf = lbuf+n2+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call createlink(buf(lbuf1:lbuf1+n1-1),n1,buf(lbuf3:lbuf3+n2-1),n
     & 2)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c       no output variable
        top=top+1
        il=iadr(l0)
        istk(il)=0
        lstk(top+1)=l0+1
        return
      end
c
c fin = 5 
c SCILAB function : DestroyLink
      subroutine intsDestroyLink
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 2) then
          call error(39)
          return
        endif
        if (lhs .ne. 1) then
          call error(41)
          return
        endif
c       checking variable appli1 (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 10) then
          err = 1
          call error(55)
          return
        endif
        if (istk(il1+1)*istk(il1+2) .ne. 1) then
          err = 1
          call error(89)
          return
        endif
        n1 = istk(il1+5)-1
        l1 = il1+6
c       checking variable appli2 (number 2)
c       
        il2 = iadr(lstk(top-rhs+2))
        if (istk(il2) .ne. 10) then
          err = 2
          call error(55)
          return
        endif
        if (istk(il2+1)*istk(il2+2) .ne. 1) then
          err = 2
          call error(89)
          return
        endif
        n2 = istk(il2+5)-1
        l2 = il2+6
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lbuf1 = lbuf
        call cvstr(n1,istk(l1),buf(lbuf1:lbuf1+n1-1),1)
        lbuf = lbuf+n1+1
        lbuf3 = lbuf
        call cvstr(n2,istk(l2),buf(lbuf3:lbuf3+n2-1),1)
        lbuf = lbuf+n2+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call destroylink(buf(lbuf1:lbuf1+n1-1),n1,buf(lbuf3:lbuf3+n2-1),
     & n2)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c       no output variable
        top=top+1
        il=iadr(l0)
        istk(il)=0
        lstk(top+1)=l0+1
        return
      end
c
c fin = 6 
c SCILAB function : WaitMsg
      subroutine intsWaitMsg
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 1) then
          call error(39)
          return
        endif
        if (lhs .gt. 2) then
          call error(41)
          return
        endif
c       checking variable appli (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 10) then
          err = 1
          call error(55)
          return
        endif
        if (istk(il1+1)*istk(il1+2) .ne. 1) then
          err = 1
          call error(89)
          return
        endif
        n1 = istk(il1+5)-1
        l1 = il1+6
c     
c       cross variable size checking
c     
c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        lbuf1 = lbuf
        call cvstr(n1,istk(l1),buf(lbuf1:lbuf1+n1-1),1)
        lbuf = lbuf+n1+1
        lw3=lw
        lw=lw+1
        lw5=lw
        lw=lw+1
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
        call waitmsg(buf(lbuf1:lbuf1+n1-1),n1,stk(lw3),ne3,stk(lw5),ne5)
        if (err .gt. 0) return
c
        top=top-rhs
        lw0=lw
        mv=lw0-l0
c     
        if(lhs .ge. 1) then
c     
c       output variable: type
c     
        top=top+1
        ilw=iadr(lw)
        err=sadr(ilw+6+ne3)-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        istk(ilw)=10
        istk(ilw+1)=1
        istk(ilw+2)=1
        istk(ilw+3)=0
        istk(ilw+4)=1
        istk(ilw+5)=ne3+1
        call cchar(ne3,stk(lw3),istk(ilw+6))
        lw=sadr(ilw+6+ne3)
        lstk(top+1)=lw-mv
        endif
c     
        if(lhs .ge. 2) then
c     
c       output variable: msg
c     
        top=top+1
        ilw=iadr(lw)
        err=sadr(ilw+6+ne5)-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        istk(ilw)=10
        istk(ilw+1)=1
        istk(ilw+2)=1
        istk(ilw+3)=0
        istk(ilw+4)=1
        istk(ilw+5)=ne5+1
        call cchar(ne5,stk(lw5),istk(ilw+6))
        lw=sadr(ilw+6+ne5)
        lstk(top+1)=lw-mv
        endif
c     
c       putting in order the stack
c     
        call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
        return
      end
c

c  interface function 
c   ********************
       subroutine  comm
c
      include '../stack.h'
c
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c
      goto (1,2,3,4,5,6) fin 
       return
1      call intsGetMsg
      return
2      call intsSendMsg
      return
3      call intsExecAppli
      return
4      call intsCreateLink
      return
5      call intsDestroyLink
      return
6      call intsWaitMsg
      return
       end
