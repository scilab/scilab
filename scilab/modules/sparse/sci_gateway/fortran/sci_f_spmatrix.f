c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intspmatrix(id)
      include 'stack.h'
      integer id(nsiz),top0,tops
      integer iadr, sadr
      double precisionx sz, szr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      tops=top
      top0=top+1-rhs

      if (rhs .lt.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     checking variable ne
      il=iadr(lstk(top))
      if(istk(il).ne.1.or.istk(il+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      if(rhs.eq.2) then
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).eq.1) then
            mr=int(stk(sadr(il+4)))
            nr=1
         elseif(istk(il+1)*istk(il+2).eq.2) then
            mr=int(stk(sadr(il+4)))
            nr=int(stk(sadr(il+4)+1))
         else
            top=tops
            call ref2val
            call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
      elseif(rhs.gt.3) then
            top=tops
            call ref2val
            call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
            fun=-1
            return
      elseif(rhs.eq.3) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         l=sadr(il+4)
         nr=stk(l)
c     checking variable mr
         top=top-1
         il=iadr(lstk(top))
         if(istk(il).ne.1.or.istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         l=sadr(il+4)
         mr=stk(l)
      endif
c     checking variable sp
      top=top-1
      il1 = iadr(lstk(top))
      if (istk(il1) .ne. 5.and.istk(il1) .ne. 6) then
         err = 1
         call error(219)
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)

C     to avoid integer overflow
      sz=dble(m)*dble(n)
      szr=dble(mr)*dble(nr)
      
      if(sz.ne.szr) then
         call error(60)
         return
      endif
      if(mr.eq.m) return

      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      if(istk(il1).eq.5) then
         if(mr.gt.m) then
            ls=sadr(il1+5+mr+nel)
            ils=iadr(ls+nel*(it+1))
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nel*(it+1),stk(l),-1,stk(ls),-1)
            call icopy(nel+m,istk(il1+5),-1,istk(ils),-1)
         else
            ls=sadr(il1+5+mr+nel)
            ils=iadr(lw)
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),1,istk(ils),1)
            call unsfdcopy(nel*(it+1),stk(l),1,stk(ls),1)
         endif
         if(it.eq.0) then
            call dspmat(m,n,stk(ls),nel,istk(ils),mr,istk(il1+5),
     $           istk(iw))
         else
            call wspmat(m,n,stk(ls),stk(ls+nel),nel,istk(ils),mr,
     $           istk(il1+5),istk(iw))
         endif
         lstk(top+1)=ls+nel*(it+1)
      else
         if(mr.gt.m) then
            ils=il1+5+mr+nel
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),-1,istk(ils),-1)
         else
            ls=sadr(il1+5+mr+nel)
            ils=iadr(lw)
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),1,istk(ils),1)
         endif
         call lspmat(m,n,nel,istk(ils),mr,istk(il1+5),istk(iw))
         lstk(top+1)=sadr(il1+5+mr+nel)
      endif
      istk(il1+1)=mr
      istk(il1+2)=nr
      return
      end
c			======================================
