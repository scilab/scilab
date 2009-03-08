c     --------------------------

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
      subroutine intereduc(fname)
c     SCILAB function : ereduc
c     --------------------------

      INCLUDE 'stack.h'
c     
      character*(*) fname
      double precision eps
      integer iadr,sadr,tops


      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
      
      tops=top
      eps=stk(leps)
c     
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .gt. 5) then
         call error(41)
         return
      endif
c     checking variable e (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))

      if (istk(il1) .ne. 1) then
         call putfunnam('ereduc',top-rhs+1)
         fun=-1
         return
      endif
      n1 = istk(il1+1)
      m1 = istk(il1+2)
      l1 = sadr(il1+4)
c     checking variable tol (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         err = 2
         call error(53)
         return
      endif
      if (istk(il2+1)*istk(il2+2) .ne. 1) then
         err = 2
         call error(89)
         return
      endif
      l2 = sadr(il2+4)
c     
c     cross variable size checking
c     
c     
c     cross formal parameter checking
c     not implemented yet
c     
c     cross equal output variable checking
c     not implemented yet
      lw4=lw
      lw=lw+n1*n1
      lw5=lw
      lw=lw+m1*m1
      lw6=lw
      lw=lw+n1
      lw7=lw
      lw=lw+1
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
      call ereduc(stk(l1),n1,m1,stk(lw4),stk(lw5),stk(lw6),stk(lw7)
     $     ,stk(l2))
      if (err .gt. 0) return
c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
      if(lhs .ge. 1) then
c     
c     output variable: e
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+n1*m1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il1),1,istk(ilw),1)
         lw=sadr(ilw+4)
         call unsfdcopy(n1*m1,stk(l1),1,stk(lw),1)
         lw=lw+n1*m1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 2) then
c     
c     output variable: q
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+n1*n1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=n1
         istk(ilw+2)=n1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(n1*n1,stk(lw4),1,stk(lw),1)
         lw=lw+n1*n1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 3) then
c     
c     output variable: z
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+m1*m1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=m1
         istk(ilw+2)=m1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(m1*m1,stk(lw5),1,stk(lw),1)
         lw=lw+m1*m1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 4) then
c     
c     output variable: stair
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+n1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=n1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(n1,stk(lw6),1,stk(lw),1)
         lw=lw+n1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 5) then
c     
c     output variable: rk
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+5-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(1,stk(lw7),1,stk(lw),1)
         lw=lw+1
         lstk(top+1)=lw-mv
      endif
c     
c     putting in order the stack
c     
      call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return

      end
c     --------------------------
