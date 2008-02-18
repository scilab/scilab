c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intpdiv(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer voln,vold,v2
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1.and.lhs.ne.2) then
         call error(41)
         return
      endif
      if(rhs.ne.2) then
         call error(42)
         return
      endif
c
      lw=lstk(top+1)
c
      iln=iadr(lstk(top+1-rhs))

      il=iln
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'pdiv',il)
         return
      endif
c
      ild=iadr(lstk(top))

      il=ild
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'pdiv',il)
         return
      endif
c
      if(istk(iln).lt.0) then
c     move d and copy value of n in place of its reference
         k=istk(iln+2)
         voln=lstk(k+1)-lstk(k)
         vold=lstk(top+1)-lstk(top)
         err=lstk(top)+voln+vold-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vold,stk(lstk(top)),-1,stk(lstk(top-1)+voln),-1)
         call unsfdcopy(voln,stk(lstk(k)),1,stk(lstk(top-1)),1)
         lstk(top)=lstk(top-1)+voln
         ild=iadr(lstk(top))
         lstk(top+1)=lstk(top)+vold
      endif

      if(istk(ild).lt.0) then
c      copy value of b in place of its reference
         k=istk(ild+2)
         vold=lstk(k+1)-lstk(k)
         err=lstk(top)+vold-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vold,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vold
      endif
      lw=lstk(top+1)

      il1=iln
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
      if(mn1.ne.1) then
         err=1
         call error(89)
         return
      endif
      if(istk(il1).eq.2) then
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         call icopy(4,istk(il1+4),1,id,1)
         nn=istk(id1+1)-2
      else
         nn=0
         id(1)=0
      endif
c
c     denominateur

      nd=istk(ild+1)*istk(ild+2)
      if(nd.ne.1) then
         err=2
         call error(89)
         return
      endif
      itd=istk(ild+3)
      if(istk(ild).eq.1) then
         ld=sadr(ild+4)
         nd=0
         if (id(1).eq.0) then
            err=2
            call error(54)
            return
         endif
         v2=1
      else
         if(id(1).eq.0) call icopy(4,istk(ild+4),1,id,1)
         do 61 i=1,4
            if(id(i).ne.istk(ild+3+i)) then
               call error(43)
               return
            endif
 61      continue
         nd=istk(ild+9)-2
         ld=sadr(ild+10)
         v2=istk(ild+9)-1
      endif

c     test degre du denominateur
 64   continue
      if(itd.eq.0) then
         if(stk(ld+nd).ne.0.0d+0) goto 65
      else
         if(abs(stk(ld+nd))+abs(stk(ld+v2+nd)).ne.0.0d+0) goto 65
      endif
      nd=nd-1
      if(nd.lt.0) then
         call error(27)
         return
      endif
      goto 64
c
 65   if(nn.lt.nd) then
c        . No division to perform
         if(lhs.eq.2) then
            istk(ild)=1
            ld=sadr(ild+4)
            stk(ld)=0.0d+0
            lstk(top+1)=ld+1
         else
            top=top-1
            istk(il1)=1
            stk(l1)=0.0d+0
            lstk(top+1)=l1+1
         endif
         goto 999
      endif
c
      if(it1.eq.0) then
         if (itd.eq.0) then
            call dpodiv(stk(l1),stk(ld),nn,nd)
         else
            l1i=lw
            lw=l1i+nn+1
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ldi=ld+v2
            call dset(nn+1,0.0d0,stk(l1i),1)
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         endif
      else
         l1i=l1+nn+1
         if (itd.eq.0) then
            ldi=lw
            lw=ldi+nd+1
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(nd+1,0.0d0,stk(ldi),1)
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         else
            ldi=ld+v2
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         endif
      endif
c
      itr=max(it1,itd)
      nq=nn-nd

      if(lhs.eq.1) then
c     .  only q is returned
         top=top-1
c     .  set type
         istk(il1+3)=itr
c     .  set length
         istk(il1+9)=nq+2
c     .  install real and imaginary part
         call unsfdcopy(nq+1,stk(l1+nd),1,stk(l1),1)
         if(itr.eq.1) call unsfdcopy(nq+1,stk(l1i+nd),1,stk(l1+nq+1),1)
      else
c     .  computes remainder degree
         nr=nd
 66      nr=nr-1
         if(nr.lt.0) goto 67
         if (itr.eq.0) then
            if(stk(l1+nr).eq.0.0d+0) goto 66
         else
            if(stk(l1+nr).eq.0.0d+0.and.stk(l1i+nr).eq.0.0d+0) goto 66
         endif
 67      continue

c
         if(itr.eq.0) then
c     .     real result
            if(nr.gt.0) then
c     .        remainder is a polynomial
               istk(id1+1)=nr+2
               lstk(top)=l1+(nr+1)
            else
c     .        remainder is a scalar
               istk(il1)=1
               istk(il1+1)=1
               istk(il1+2)=1
               lr=sadr(il1+4)
               stk(lr)=stk(l1)
c????               stk(lr+1)=stk(l1i)
               lstk(top)=lr+1
            endif
            ilq=iadr(lstk(top))
            lq=sadr(ilq+10)
            inc=1
            if (l1+nd.lt.lq) inc=-1
            call unsfdcopy(nq+1,stk(l1+nd),inc,stk(lq),inc)
            if(nr.lt.0) stk(lr)=0.0d+0
            istk(ilq)=2
            istk(ilq+1)=1
            istk(ilq+2)=1
            istk(ilq+3)=0
            call icopy(4,id,1,istk(ilq+4),1)
            idq=ilq+8
            istk(idq)=1
            istk(idq+1)=nq+2
            lstk(top+1)=lq+nq+1
            goto 999
         else
c     .     complex result
c     .     preserve quotient coeff
            lqs=lw
            lw=lqs+sadr(10)+(nq+1)*2
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nq+1,stk(l1+nd),1,stk(lqs),1)
            call unsfdcopy(nq+1,stk(l1i+nd),1,stk(lqs+nq+1),1)

c     .     set remainder
            if(nr.ge.0) then
               istk(il1+3)=1
               call unsfdcopy(nr+1,stk(l1i),1,stk(l1+nr+1),1)
               istk(id1+1)=nr+2
               lstk(top)=l1+(nr+1)*2
            else
               istk(il1+3)=0
               stk(l1)=0.0d0
               istk(id1+1)=2
               lstk(top)=l1+1
            endif
c     .     set quotient
            ilq=iadr(lstk(top))
            istk(ilq)=2
            istk(ilq+1)=1
            istk(ilq+2)=1
            istk(ilq+3)=1
            call icopy(4,id,1,istk(ilq+4),1)
            idq=ilq+8
            istk(idq)=1
            istk(idq+1)=nq+2
            lq=sadr(ilq+10)
            call unsfdcopy(2*(nq+1),stk(lqs),1,stk(lq),1)
            lstk(top+1)=lq+(nq+1)*2
         endif
      endif

      goto 999
 999  return
      end
c			=======================================
