c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
c     --------------------------
c     SCILAB function : semidef
c     --------------------------
      subroutine scisemidef(fname)
      
      character*(*) fname
	INCLUDE 'stack.h'
      double precision abstol,reltol,nu,tv
      integer sz,upsz
      integer iadr, sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1


      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 6) then
         call error(39)
         return
      endif
      if (lhs .gt. 4) then
         call error(41)
         return
      endif
c     checking variable x (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 1) then
         err = 1
         call error(53)
         return
      endif
      m = istk(il1+1)*istk(il1+2)
      l1 = sadr(il1+4)
c     checking variable Z (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         err = 2
         call error(53)
         return
      endif
      m2 = istk(il2+1)*istk(il2+2)
      l2 = sadr(il2+4)
c     checking variable F (number 3)
c     
      il3 = iadr(lstk(top-rhs+3))
      if (istk(il3) .ne. 1) then
         err = 3
         call error(53)
         return
      endif
      m3 = istk(il3+1)*istk(il3+2)
      l3 = sadr(il3+4)
c     checking variable ind (number 4)
c     
      il4 = iadr(lstk(top-rhs+4))
      if (istk(il4) .ne. 1) then
         err = 4
         call error(53)
         return
      endif
      m4 = istk(il4+1)*istk(il4+2)
      l4 = sadr(il4+4)
c     checking variable c (number 5)
c     
      il5 = iadr(lstk(top-rhs+5))
      if (istk(il5) .ne. 1) then
         err = 5
         call error(53)
         return
      endif
      m5 = istk(il5+1)*istk(il5+2)
      l5 = sadr(il5+4)
c     checking variable pars (number 6)
c     
      il6 = iadr(lstk(top-rhs+6))
      if (istk(il6) .ne. 1) then
         err = 6
         call error(53)
         return
      endif
      m6 = istk(il6+1)*istk(il6+2)
      if (m6 .ne. 5) then
         err = 6
         call error(89)
         return
      endif
      l6 = sadr(il6+4)
      nu=stk(l6)
      abstol=stk(l6+1)
      reltol=stk(l6+2)
      tv=stk(l6+3)
      iters=stk(l6+4)
c     
c     cross variable size checking
c     
      if (m .ne. m5) then
         call error(42)
         return
      endif
      call entier(m4,stk(l4),istk(iadr(l4)))
      nn8=2
      lw8=lw
      lw=lw+nn8
      n=0
      sz=0
      upsz=0
      maxn=0
      lind=iadr(l4)
      do 10 i=0,m4-1
         indi=istk(lind+i)
         n=n+indi
         sz=sz+indi*(indi+1)/2
         upsz=upsz+indi*indi
         maxn=max(maxn,indi)
 10   continue
c     optimal block size for dgels ????
      nb=32
      lwork=(m+2)*sz+upsz+2*n+max(m+sz*nb,3*maxn+maxn*(maxn+1),3*m)
      nn14=lwork
      lw14=lw
      lw=lw+nn14
      ilw15=iadr(lw)
      lw=sadr(ilw15+m)
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     call sp(m, L,  F,     ind,           c,      x,      Z,
      call spf(m,m4,stk(l3),istk(iadr(l4)),stk(l5),stk(l1),stk(l2),
c     ul,      nu,abstol,reltol,tv,iters,work,     lwork,info)
     &     stk(lw8),nu,abstol,reltol,tv,iters,stk(lw14),lwork,
     &     istk(ilw15),info)
      if(info.lt.0) then
         call error(230)
         return
      endif

c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
      if(lhs .ge. 1) then
c     
c     output variable: x
c     
         top=top+1
      endif
      if(lhs .ge. 2) then
c     
c     output variable: Z
c     
         top=top+1
      endif
      if(lhs .ge. 3) then
c     
c     output variable: ul
c     
         top=top+1
         lw=lstk(top)
         err=lw+4+nn8-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         ilw=iadr(lw)
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=nn8
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(nn8,stk(lw8),1,stk(lw),1)
         lw=lw+nn8
         lstk(top+1)=lw
      endif
c     
      if(lhs .ge. 4) then
c     
c     output variable: [info iters]
c     
         top=top+1
         lw=lstk(top)
         err=lw+4+2-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         ilw=iadr(lw)
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=2
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         stk(lw)=info
         stk(lw+1)=iters
         lw=lw+2
         lstk(top+1)=lw
      endif
      return
      end
c     --------------------------
