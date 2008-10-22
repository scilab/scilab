c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intprod(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer sel,tops
      integer iadr,sadr
      integer mtlbsel
      double precision t,tr,ti
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      tops=top
      sel=0
c
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
      ref=ilr.ne.il0
c
      if(istk(il0).eq.1) then
c     standard matrix case
         if(rhs.eq.2) then
            call getorient(top,sel)
            if(err.gt.0) return
            top=top-1
            if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)
         endif
         m=istk(il0+1)
         n=istk(il0+2)
         it=istk(il0+3)
         mn=m*n
         l1=sadr(ilr+4)
         l=sadr(il0+4)
         if(mn.eq.0) then
            if(ilr.ne.il0) then
               err=l1+1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            if(sel.eq.0) then
               istk(ilr)=1
               istk(ilr+1)=1
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(l1)=1.0d0
               lstk(top+1)=l1+1
            else
               istk(ilr)=1
               istk(ilr+1)=0
               istk(ilr+2)=0
               istk(ilr+3)=0
               lstk(top+1)=l1
            endif
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
         if(ref) then
            err=l1+mr*nr*(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         istk(ilr)=1
         istk(ilr+1)=mr
         istk(ilr+2)=nr
         istk(ilr+3)=it
         l1=sadr(ilr+4)

         if(ref) then
            if(it.eq.0) then
               call dmprod(sel,stk(l),m,m,n,stk(l1),1)
            else
               call wmprod(sel,stk(l),stk(l+m*n),m,m,n,stk(l1),
     $              stk(l1+mr*nr),1) 
            endif
         else
            if(it.eq.0) then
               call dmprod(sel,stk(l),m,m,n,stk(l1),1)
            else
               call wmprod(sel,stk(l),stk(l+m*n),m,m,n,stk(l),
     $              stk(l+m*n),1)  
               call unsfdcopy(mr*nr,stk(l+m*n),-1,stk(l1+mr*nr),-1)
            endif
         endif
         lstk(top+1)=l1+mr*nr*(it+1)
      elseif(istk(il0).eq.5) then
c     sparse matrix case
         if(rhs.eq.2) then
            call getorient(top,sel)
            if(err.gt.0) return
            top=top-1
         endif
         if(sel.ne.0) then
            top=tops
            call funnam(ids(1,pt+1),'prod',il0)
            fun=-1
            return
         endif
         m=istk(il0+1)
         n=istk(il0+2)
         it=istk(il0+3)
         mn=istk(il0+4)
         l=sadr(il0+5+m+mn)
         lr=sadr(ilr+5+m+mn)
         if(ilr.ne.il0) then
            err=lr+(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
c         
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=it
         l1=sadr(ilr+4)
         if(mn.eq.m*n) then
            if(it.eq.0) then
               t=1.0d0
               call dvmul(mn,stk(l),1,t,0)
               stk(l1)=t
            else
               tr=1.0d0
               ti=0.0d0
               call wvmul(mn,stk(l),stk(l+mn),1,tr,ti,0)
               stk(l1)=tr
               stk(l1+1)=ti
            endif
         else
            stk(l1)=0.0d0
            istk(ilr+3)=0
            it=0
         endif
         lstk(top+1)=l1+it+1
c
      elseif(istk(il0).eq.2) then
         top=tops
         fin=9
         fun=16
         return
      else
         top=tops
         call funnam(ids(1,pt+1),'prod',il0)
         fun=-1
      endif
      return
      end
c     -------------------------------
      
