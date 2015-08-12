c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine i_sum(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer sel,tops
      integer iadr,sadr

      external memused,mtlbsel
      integer memused,mtlbsel
      integer type
      integer native
      parameter (native=0)
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.3) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      tops=top
c     
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
      ref=ilr.ne.il0
c     
      call  orientandtype(sel,type)
      if (err.gt.0.or.err1.gt.0) return
      if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)
      

      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      mn=m*n
      l1=ilr+4
      l=il0+4
      if(mn.eq.0) then
c     .  as the only empty matrices have a double type this part of code
c     .  should never be used.
         if(ref) then
            err=sadr(l1+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         if(sel.eq.0) then
            istk(ilr)=8
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=it
            call tpconv(4,it,1,0,1,is2,1)
            call gencopy(1,is2,1,istk(l1),1)
            lstk(top+1)=sadr(l1+1)
         else
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=l1
         endif
         return
      endif
      if (sel.gt.2) then
c     sum(a,sel)-->a
         lr=sadr(il0+4)
         err=lr+mn-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call tpconv(it,0, mn,istk(l1),-1,stk(lr),-1)
         istk(ilr)=1
         istk(ilr+1)=m
         istk(ilr+2)=n
         istk(ilr+3)=0
         lstk(top+1)=lr+mn
         return
      endif

      if(sel.eq.0) then
         mr=1
         nr=1
      elseif(sel.eq.1) then
         mr=1
         nr=n
      else
         mr=m
         nr=1
      endif
      if(type.eq.native) then
c     .  return an array of integers
         if(ref) then
            err=sadr(l1+memused(it,mr*nr))-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
     
         istk(ilr)=8
         istk(ilr+1)=mr
         istk(ilr+2)=nr
         istk(ilr+3)=it
         l1=ilr+4
         call genmsum(it,sel,istk(l),m,m,n,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it,mr*nr))
      else
c     .  return an array of doubles
         if(ref) then
            lr=sadr(ilr+4)
         else
            lr=lstk(top+1)
         endif
         err=lr+mr*nr-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call genmsumasdouble(it,sel,istk(l),m,m,n,stk(lr),1)
         if (.not.ref) then
            call dcopy(mr*nr,stk(lr),1,stk(sadr(ilr+4)),1)
            lr=sadr(ilr+4)
         endif
         istk(ilr)=1
         istk(ilr+1)=mr
         istk(ilr+2)=nr
         istk(ilr+3)=0
         lstk(top+1)=lr+mr*nr
      endif
      return
      end

