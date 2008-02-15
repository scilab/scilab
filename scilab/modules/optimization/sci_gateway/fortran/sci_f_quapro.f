c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
	subroutine sciquapro(fname)
			
	character*(*) fname
      include 'stack.h'
      integer iadr, sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
			
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 10 .and. rhs .ne. 9) then
         call error(39)
         return
      endif
      if (lhs .gt. 3 .or. lhs .lt. 2) then
         call error(41)
         return
      endif
c     checking variable x0 (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 1) then
         err = 1
         call error(53)
         return
      endif
      if (istk(il1+2) .ne. 1) then
         err = 1
         call error(89)
         return
      endif
      n1 = istk(il1+1)
      l1 = sadr(il1+4)
c     checking variable h (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         err = 2
         call error(53)
         return
      endif
      if (istk(il2+1) .ne. istk(il2+2)) then
         err = 2
         call error(20)
         return
      endif
      n2 = istk(il2+1)
      m2 = istk(il2+2)
      l2 = sadr(il2+4)
c     checking variable p (number 3)
c     
      il3 = iadr(lstk(top-rhs+3))
      if (istk(il3) .ne. 1) then
         err = 3
         call error(53)
         return
      endif
      if (istk(il3+2) .ne. 1) then
         err = 3
         call error(89)
         return
      endif
      n3 = istk(il3+1)
      l3 = sadr(il3+4)
c     checking variable c (number 4)
c     
      il4 = iadr(lstk(top-rhs+4))
      if (istk(il4) .ne. 1) then
         err = 4
         call error(53)
         return
      endif
      n4 = istk(il4+1)
      m4 = istk(il4+2)
      l4 = sadr(il4+4)
c     checking variable d (number 5)
c     
      il5 = iadr(lstk(top-rhs+5))
      if (istk(il5) .ne. 1) then
         err = 5
         call error(53)
         return
      endif
      if (istk(il5+2) .ne. 1.and.istk(il5+2) .ne. 0) then
         err = 5
         call error(89)
         return
      endif
      m5 = istk(il5+1)
      l5 = sadr(il5+4)
c     checking variable ci (number 6)
c     
      il6 = iadr(lstk(top-rhs+6))
      if (istk(il6) .ne. 1) then
         err = 6
         call error(53)
         return
      endif
      if (istk(il6+2) .gt. 1) then
         err = 6
         call error(89)
         return
      endif
      n6 = istk(il6+1)
      l6 = sadr(il6+4)
c     checking variable cs (number 7)
c     
      il7 = iadr(lstk(top-rhs+7))
      if (istk(il7) .ne. 1) then
         err = 7
         call error(53)
         return
      endif
      if (istk(il7+2) .gt. 1) then
         err = 7
         call error(89)
         return
      endif
      n7 = istk(il7+1)
      l7 = sadr(il7+4)
c     checking variable mi (number 8)
c     
      il8 = iadr(lstk(top-rhs+8))
      if (istk(il8) .ne. 1) then
         err = 8
         call error(53)
         return
      endif
      if (istk(il8+1)*istk(il8+2) .ne. 1) then
         err = 8
         call error(89)
         return
      endif
      mi = stk(sadr(il8+4))
c     checking variable modo (number 9)
c     
      il9 = iadr(lstk(top-rhs+9))
      if (istk(il9) .ne. 1) then
         err = 9
         call error(53)
         return
      endif
      if (istk(il9+1)*istk(il9+2) .ne. 1) then
         err = 9
         call error(89)
         return
      endif
      modo = stk(sadr(il9+4))
      if(modo.ne.1.and.modo.ne.2.and.modo.ne.3) then
         err=9
         call error(36)
         return
      endif
      if (rhs .eq. 10) then
c     checking variable imp (number 10)
c     
         il10 = iadr(lstk(top-rhs+10))
         if (istk(il10) .ne. 1) then
            err = 10
            call error(53)
            return
         endif
         if (istk(il10+1)*istk(il10+2) .ne. 1) then
            err = 10
            call error(89)
            return
         endif
         imp = stk(sadr(il10+4))
      else
         imp = 0
      endif
c     
c     cross variable size checking
c     
      if (n3 .ne. n2) then
         call error(42)
         return
      endif
      if (n1 .ne. n3) then
         if(modo.eq.3) then
            call error(42)
            return
         else
            n1=n3
         endif
      endif
      if (n4.ne.0.and.n3 .ne. n4) then
         call error(42)
         return
      endif
      if (n6 .eq. 0) then
         ira=0
      elseif (n1 .ne. n6) then
         call error(42)
         return
      else
         ira=1
      endif
      if(n7 .eq. 0) then
      elseif (n1 .ne. n7) then
         call error(42)
         return
      else
         ira=ira+2
      endif
      if (m4 .ne. m5) then
         call error(42)
         return
      endif
      md=m4-mi
c     
c     x
c      lw9=lw
c     lw=lw+n1
      lw9=l1
c     f
      lw10=lw
      lw=lw+1
c     w
      lw11=lw
      lw=lw+4*n1*(n1+1)+md+max(md,2*n1-2+n1*(n1+1)/2)
c     iv
      lw12=lw
      lw=lw+3*n1+2*md+mi+1
c     lagr
      lw13=lw
      if(ira.gt.0) then
         nl=n1+m4
      else
         nl=m4
      endif
      lw=lw+nl
c     
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif

C     SUBROUTINE PLCBAS(H,P,C,D,CI,CS,IRA,MI,MD,X,F,W,IV,DLAGR,IMP,IO,N,
C    &                  MODO,INFO,ITER)
C
      iter=0
      call plcbas(stk(l2),stk(l3),stk(l4),stk(l5),stk(l6),stk(l7),
     &     ira,mi,md,stk(lw9),stk(lw10),stk(lw11),stk(lw12),
     &     stk(lw13),imp,wte,n1,modo,info,iter)
      if (info .ne. 0) then
         if (info .eq. 1) then
            call msgs(104,0)
         elseif (info .eq. -1) then
            call error(123)
            return
         elseif (info .eq. -2) then
            call msgs(11,0)
         elseif (info .eq. -3) then
            call error(125)
            return
         elseif (info .eq. -4) then
            call error(42)
            return
         elseif (info .eq. -11) then
            call error(126)
            return
         elseif (info .eq. -12) then
            call error(127)
            return
         elseif (info .eq. -13) then
            call error(128)
            return
         elseif (info .eq. -14) then
            call error(129)
            return
         endif
      endif
      
c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
c     Variable de sortie: x
c     
      top=top+1
      ilw=iadr(lw)
      err=lw+4+n1-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=n1
      istk(ilw+2)=1
      istk(ilw+3)=0
      lw=sadr(ilw+4)
      call unsfdcopy(n1,stk(lw9),1,stk(lw),1)
      lw=lw+n1
      lstk(top+1)=lw-mv
c     
c     Variable de sortie: f
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
      call unsfdcopy(1,stk(lw10),1,stk(lw),1)
      lw=lw+1
      lstk(top+1)=lw-mv
c     
      if ( lhs .ge. 3) then
c     Variable de sortie: lagr
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+m4-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=nl
         istk(ilw+2)=1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(nl,stk(lw13),1,stk(lw),1)
         lw=lw+nl
         lstk(top+1)=lw-mv
      endif
      
c     Remise en place de la pile
      call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return
      
      end
      
