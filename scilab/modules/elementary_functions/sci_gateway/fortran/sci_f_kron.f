c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intkron(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops
      logical refa,refb
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top
c
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      il=iadr(lstk(top))
      ilb=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.1) then
         if(fin.eq.19) then
            call funnam(ids(1,pt+1),'kronm',il)
         elseif(fin.eq.20) then
            call funnam(ids(1,pt+1),'kronr',il)
         else
            call funnam(ids(1,pt+1),'kronl',il)
         endif
         fun=-1
         return
      endif
      refb=il.ne.ilb
      mb=istk(il+1)
      nb=istk(il+2)
      itb=istk(il+3)
      lb=sadr(il+4)
      mnb=mb*nb
      top=top-1

      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.1) then
         top=tops
         if(fin.eq.19) then
            call funnam(ids(1,pt+1),'kronm',il)
         elseif(fin.eq.20) then
            call funnam(ids(1,pt+1),'kronr',il)
         else
            call funnam(ids(1,pt+1),'kronl',il)
         endif
         fun=-1
         return
      endif
      refa=il.ne.ilr
      ma=istk(il+1)
      na=istk(il+2)
      ita=istk(il+3)
      la=sadr(il+4)
      mna=ma*na
c
      l=sadr(ilr+4)
      l1=l+mnb*mna*(max(itb,ita)+1)
      lstk(top+1)=l1
      lw=l1
c
      if(fin.eq.19 .or. fin.eq.18) goto 115
      if(fin.eq.20) then
         if(refb) then
            err=lw+mnb*(itb+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dcopy(mnb*(itb+1),stk(lb),1,stk(lw),1)
         endif
         lb=lw
         lw=lb+mnb*(itb+1)
         l=lb
         mn=mnb
         it=itb
      elseif(fin.eq.21) then
         if(refa) then
            err=lw+mna*(ita+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dcopy(mna*(ita+1),stk(la),1,stk(lw),1)
         endif
         la=lw
         lw=la+mna*(ita+1)
         l=la
         mn=mna
         it=ita
      endif

  111 if(it.eq.1) goto 113
      do 112 k=1,mn
         lk=l+k-1
         if(stk(lk).eq.0.0d+0) then
            call error(27)
            return
         endif
         stk(lk)=1.0d+0/stk(lk)
 112  continue
      goto 115
  113 do 114 k=1,mn
         lk=l+k-1
         sr=stk(lk)
         si=stk(lk+mn)
         s=sr*sr+si*si
         if(s.eq.0.0d+0) then
            call error(27)
            return
         endif
         stk(lk)=sr/s
         stk(lk+mn)=-si/s
 114  continue
c
 115  continue

c
c move a and b if necessary
      if(.not.refb) then
         lw=lw+mnb*(itb+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(mnb*(itb+1),stk(lb),-1,stk(lw),-1)
         lb=lw
         lw=lw+mnb*(itb+1)
      endif
      if(.not.refa) then
         lw=lw+mna*(ita+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(mna*(ita+1),stk(la),-1,stk(lw),-1)
         la=lw
      endif

      goto(117,116,118) itb+2*ita
c a et b sont reelles
      call kronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
      goto 999
c a est complexe b est reelle
  116 call kronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
      call kronr(stk(la+mna),ma,ma,na,stk(lb),mb,mb,nb,stk(l+mnb*mna),
     1 ma*mb)
      goto 999
c a est reelle b complexe
  117 call kronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
      call kronr(stk(la),ma,ma,na,stk(lb+mnb),mb,mb,nb,
     1 stk(l+mnb*mna),ma*mb)
      goto 999
  118 call kronc(stk(la),stk(la+mna),ma,ma,na,stk(lb),stk(lb+mnb),
     1 mb,mb,nb,stk(l),stk(l+mnb*mna),ma*mb)
      goto 999
 999  continue
      istk(ilr)=1
      istk(ilr+1)=mb*ma
      istk(ilr+2)=nb*na
      istk(ilr+3)=max(itb,ita)
      return
      end

c     -------------------------------
