      subroutine matqz
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      double precision t,eps
      logical fail,macro
      integer top0,top2,iadr,sadr,tops
      character*(nlgh+1) namef
      common/cschur/namef
      external fschur,bschur
      integer         iero
      common /ierinv/ iero

c     
c     fin    1       2     3        4
c          gschur   gspec ereduc  fstair
c     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matqz '//buf(1:4))
      endif
c     
      tops=top
      eps=stk(leps)

      if(fin.eq.1) then
c     obsolete 
c     call macro for emulation
         call cvname(ids(1,pt+1),'gschur',0)
         fun=-1
         return
      elseif(fin.eq.2) then
c     osolete
c     call macro for emulation
         call cvname(ids(1,pt+1),'gspec',0)
         fun=-1
         return
      elseif (fin .eq.3) then
         call intereduc('ereduc')
      elseif (fin .eq. 4) then
         call intfstair('fstair')
      endif
      return
c     
      end

      subroutine intereduc(fname)
c     Copyright INRIA
c     SCILAB function : ereduc
c     --------------------------

      INCLUDE '../stack.h'
c     
      character*(*) fname
      double precision t,eps
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

      subroutine intfstair(fname)
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      character*(*) fname
      double precision t,eps
      integer iadr,sadr,tops

     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1

c     
      tops=top
      eps=stk(leps)

c     
c     SCILAB function : fstair
c     --------------------------
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 7) then
         call error(39)
         return
      endif
      if (lhs .gt. 10) then
         call error(41)
         return
      endif
c     checking variable a (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 1) then
         if (istk(il1) .ne. 1) then
            call putfunnam('fstair',top-rhs+1)
            fun=-1
            return
         endif
      endif
      n1 = istk(il1+1)
      m1 = istk(il1+2)
      l1 = sadr(il1+4)
c     checking variable e (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         call putfunnam('fstair',top-rhs+2)
         fun=-1
         return
      endif
      n2 = istk(il2+1)
      m2 = istk(il2+2)
      l2 = sadr(il2+4)
c     checking variable q (number 3)
c     
      il3 = iadr(lstk(top-rhs+3))
      if (istk(il3) .ne. 1) then
         call putfunnam('fstair',top-rhs+3)
         fun=-1
         return
      endif
      if (istk(il3+1) .ne. istk(il3+2)) then
         err = 3
         call error(20)
         return
      endif
      n3 = istk(il3+1)
      m3 = istk(il3+2)
      l3 = sadr(il3+4)
c     checking variable z (number 4)
c     
      il4 = iadr(lstk(top-rhs+4))
      if (istk(il4) .ne. 1) then
         call putfunnam('fstair',top-rhs+4)
         fun=-1
         return
      endif
      if (istk(il4+1) .ne. istk(il4+2)) then
         err = 4
         call error(20)
         return
      endif
      n4 = istk(il4+1)
      m4 = istk(il4+2)
      l4 = sadr(il4+4)
c     checking variable stair (number 5)
c     
      il5 = iadr(lstk(top-rhs+5))
      if (istk(il5) .ne. 1) then
         call putfunnam('fstair',top-rhs+5)
         fun=-1
         return
      endif
      if (istk(il5+1) .ne. 1) then
         err = 5
         call error(89)
         return
      endif
      m5 = istk(il5+2)
      l5 = sadr(il5+4)
c     checking variable rk (number 6)
c     
      il6 = iadr(lstk(top-rhs+6))
      if (istk(il6) .ne. 1) then
         err = 6
         call error(53)
         return
      endif
      if (istk(il6+1)*istk(il6+2) .ne. 1) then
         err = 6
         call error(89)
         return
      endif
      l6 = sadr(il6+4)
c     checking variable tol (number 7)
c     
      il7 = iadr(lstk(top-rhs+7))
      if (istk(il7) .ne. 1) then
         err = 7
         call error(53)
         return
      endif
      if (istk(il7+1)*istk(il7+2) .ne. 1) then
         err = 7
         call error(89)
         return
      endif
      l7 = sadr(il7+4)
c     
c     cross variable size checking
c     
      if (n1 .ne. n2) then
         call error(42)
         return
      endif
      if (n1 .ne. n3) then
         call error(42)
         return
      endif
      if (n1 .ne. m3) then
         call error(42)
         return
      endif
      if (n1 .ne. m5) then
         call error(42)
         return
      endif
      if (m1 .ne. m2) then
         call error(42)
         return
      endif
      if (m1 .ne. n4) then
         call error(42)
         return
      endif
      if (m1 .ne. m4) then
         call error(42)
         return
      endif
c     
c     cross formal parameter checking
c     not implemented yet
c     
c     cross equal output variable checking
c     not implemented yet
      call entier(m5,stk(l5),istk(iadr(l5)))
      call entier(1,stk(l6),istk(iadr(l6)))
      lw10=lw
      lw=lw+1
      lw11=lw
      lw=lw+m1
      nn12=n1+1
      lw12=lw
      lw=lw+nn12
      lw13=lw
      lw=lw+m1
      lw14=lw
      lw=lw+nn12
      nn15=4
      lw15=lw
      lw=lw+nn15
      lw16=lw
      lw=lw+m1
      lw17=lw
      lw=lw+m1

      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
      call fstair(stk(l1),stk(l2),stk(l3),stk(l4),n1,m1
     $     ,istk(iadr(l5)),istk(iadr(l6)),stk(l7),stk(lw10)
     $     ,stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(lw15)
     $     ,stk(lw16),stk(lw17),ierr)
      if (ierr.ne.0) then
         buf='fstair failed !'
         call error(9999)
         return
      endif
c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
      if(lhs .ge. 1) then
c     
c     output variable: a
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
c     output variable: e
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+n1*m1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il2),1,istk(ilw),1)
         lw=sadr(ilw+4)
         call unsfdcopy(n2*m2,stk(l2),1,stk(lw),1)
         lw=lw+n2*m2
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 3) then
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
         call icopy(4,istk(il3),1,istk(ilw),1)
         lw=sadr(ilw+4)
         call unsfdcopy(n3*m3,stk(l3),1,stk(lw),1)
         lw=lw+n3*m3
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 4) then
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
         call icopy(4,istk(il4),1,istk(ilw),1)
         lw=sadr(ilw+4)
         call unsfdcopy(n4*m4,stk(l4),1,stk(lw),1)
         lw=lw+n4*m4
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 5) then
c     
c     output variable: nblcks
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
         call int2db(1,stk(lw10),1,stk(lw),1)
         lw=lw+1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 6) then
c     
c     output variable: muk
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+m1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=m1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(m1,stk(lw11),1,stk(lw),1)
         lw=lw+m1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 7) then
c     
c     output variable: nuk
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+nn12-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=nn12
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(nn12,stk(lw12),1,stk(lw),1)
         lw=lw+nn12
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 8) then
c     
c     output variable: muk0
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+m1-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=m1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(m1,stk(lw13),1,stk(lw),1)
         lw=lw+m1
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 9) then
c     
c     output variable: nuk0
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+nn12-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=nn12
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(nn12,stk(lw14),1,stk(lw),1)
         lw=lw+nn12
         lstk(top+1)=lw-mv
      endif
c     
      if(lhs .ge. 10) then
c     
c     output variable: mnei
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+nn15-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=nn15
         istk(ilw+2)=1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call int2db(nn15,stk(lw15),1,stk(lw),1)
         lw=lw+nn15
         lstk(top+1)=lw-mv
      endif
c     
c     putting in order the stack
c     
      call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return
c     
      end


