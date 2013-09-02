c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intsparse(id)
      include 'stack.h'
      integer id(nsiz),top0,tops
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
c     
      lw = lstk(top+1)
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     creation de matrice creuse
      if(rhs.eq.1) then
c     sparse(x)
         il=iadr(lstk(top))
         if(istk(il).eq.5.or.istk(il).eq.6) return
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0.or.m.eq.-1) return
         if(istk(il).eq.1) then
            it=istk(il+3)
            l=sadr(il+4)
c     
            ilr=iadr(lw)
            lw=sadr(ilr+m+m*n)

            ls=lw
            li=ls+m*n
            lw=ls+m*n*(it+1)
            
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            if(it.eq.0) then
               call dful2sp(m,n,stk(l),nel,istk(ilr),stk(ls),0.0d0)
            else
               call wful2sp(m,n,stk(l),stk(l+m*n),nel,
     $              istk(ilr),stk(ls),stk(li),0.0d0,0.0d0)
            endif
            call icopy(m+nel,istk(ilr),1,istk(il+5),1)
            l=sadr(il+5+m+nel)
            call unsfdcopy(nel,stk(ls),1,stk(l),1)
            if(it.eq.1) call unsfdcopy(nel,stk(li),1,stk(l+nel),1)
            istk(il)=5
            istk(il+4)=nel
            lstk(top+1)=l+nel*(it+1)
         elseif(istk(il).eq.4) then
            l=il+3
            ilr=iadr(lw)
            lw=sadr(ilr+m*n)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call lful2sp(m,n,istk(l),nel,istk(ilr))
            call icopy(m+nel,istk(ilr),1,istk(il+5),1)
            l=sadr(il+5+m+nel)
            istk(il)=6
            istk(il+3)=0
            istk(il+4)=nel
            lstk(top+1)=l
         else
            call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
            fun=-1
            return
         endif
         return
      endif
c
c     sparse([i,j],value,[m,n])
      if (rhs .ne. 3.and.rhs.ne.2) then
         call error(39)
         return
      endif
      m=-1
      n=-1
      if(rhs.eq.3) then
c     [m,n] given
         il=iadr(lstk(top))
         if(istk(il).ne.1.or.istk(il+3).ne.0) then
            err=1
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.2) then
            err=1
            call error(60)
            return
         endif
         l=sadr(il+4)
         m=stk(l)
         n=stk(l+1)
         if(m.lt.0.or.n.lt.0) then
            call error(60)
            return
         endif
         if (m.eq.0 .or. n.eq.0) then  ! m*n correction (Bruno)
            top=top0
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)+1
            return
         endif
         top=top-1
         lw=lstk(top+1)
      endif
      
      ilij=iadr(lstk(top-1))
      if(istk(ilij).ne.1.or.istk(ilij+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      if(istk(ilij+1).ne.0.and.istk(ilij+2).ne.2) then
         err=2
         call error(60)
         return
      endif
      nel=istk(ilij+1)*istk(ilij+2)/2
      lij=sadr(ilij+4)
      ilij=iadr(lij)
      if(nel.gt.0) then
         call entier(nel*2,stk(lij),istk(ilij))
      endif
c     test if indices are all strictly positive
      do 05 i=0,2*nel-1
         if (istk(ilij+i).le.0) then
            call error(21)
            return
         endif
 05   continue

c     compute number of rows if is not given
      if(rhs.eq.2) then
         mm=0
         do 10 i=0,nel-1
            mm=max(mm,istk(ilij+i))
 10      continue
      else
         mm=m
      endif
c     
      ilv=iadr(lstk(top))
      if(istk(ilv).ne.1.and.istk(ilv).ne.4) then
         call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
         fun=-1
         top=tops
         return
      endif
      if(istk(ilv+1)*istk(ilv+2).ne.nel) then
         err=2
         call error(60)
         return
      endif
      il1=ilij
      if(istk(ilv).eq.1) then
         itv=istk(ilv+3)
         lv=sadr(ilv+4)
         lind=iadr(max(lw,sadr(il1+5+mm+nel)+nel*(itv+1)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         if(itv.eq.0) then
            call dij2sp(m,n,nel,istk(ilij),stk(lv),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         else
            call wij2sp(m,n,nel,istk(ilij),stk(lv),stk(lv+nel0),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         endif
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=5
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=itv
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         inc=1
         if(l.gt.lv) then
            inc=-1
            if(itv.eq.1) then
               call unsfdcopy(nel,stk(lv+nel0),inc,stk(l+nel),inc)
            endif
            call unsfdcopy(nel,stk(lv),inc,stk(l),inc)
         else
            inc=1
            call unsfdcopy(nel,stk(lv),inc,stk(l),inc)
            if(itv.eq.1) then
               call unsfdcopy(nel,stk(lv+nel0),inc,stk(l+nel),inc)
            endif
         endif
         
         lstk(top+1)=l+nel*(itv+1)
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
      else
         lv=ilv+3
         lind=iadr(max(lw,sadr(il1+5+mm+nel)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         call lij2sp(m,n,nel,istk(ilij),istk(lv),
     $        istk(lind),mm+nel,istk(liw),ierr)
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=6
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=0
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         lstk(top+1)=l
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
      endif
      return
      end

c			======================================
