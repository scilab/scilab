c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine i_cumsum(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer sel,tops
      external memused,mtlbsel
      integer memused,mtlbsel
      integer type
      integer native
      parameter (native=0)
c
      integer iadr,sadr
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
      sel=0
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
c
      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      l=il0+4
      l1=ilr+4
      mn=m*n
c
      if (sel.gt.2) then
c     cumsum(a,sel)-->a
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

      if (type.eq.native) then
c     .  result in integers
         if(ref) then
            err=sadr(l1+memused(it,mn))-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il0),1,istk(ilr),1)
            call gencopy(it,mn,istk(l),1,istk(l1),1)
         endif
         lstk(top+1)=sadr(l1+memused(it,mn))
         if(mn.eq.0) return

         call genmcusum(it,sel,istk(l1),istk(l1),m,m,n)
      else
C     . result in floats
         if(ref) then
            lr=sadr(ilr+4)
         else
            lr=lstk(top+1)
         endif
         err=lr+mn-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call genmcusumd(it,sel,istk(l1),stk(lr),m,m,n)
         if(ref) then
            lstk(top+1)=lr+mn
         else
            l1=sadr(ilr+4)
            call dcopy(mn,stk(lr),1,stk(l1),1)
            lstk(top+1)=l1+mn
         endif
         istk(ilr)=1
         istk(ilr+3)=0
      endif
      end
