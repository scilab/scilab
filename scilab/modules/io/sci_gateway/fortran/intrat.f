c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intrat
      INCLUDE 'stack.h'
c     
      integer tops
      double precision eps,xxx
      integer iadr,sadr
      character bu1*(bsiz),bu2*(bsiz)
c
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         call writebufio(buf,fin)
         call basout(io,wte,' matio '//buf(1:4))
      endif
c     
      tops=top
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.gt.2) then
         call error(41)
         return
      endif

      if(rhs.eq.2) then
         il=iadr(lstk(top))
         if(istk(il).ne.1) then
            err=2
            call error(52)
            return
         endif
         eps=stk(sadr(il+4))
         top=top-1
      else
         eps=1.d-6
      endif

      lw=lstk(top+1)
      il=iadr(lstk(top))
      l=sadr(il+4)

      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         call funnam(ids(1,pt+1),'rat',iadr(lstk(top)))
         top=tops
         fun=-1
         return
      endif
      l=sadr(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      if(it.ne.0) then
         call funnam(ids(1,pt+1),'rat',iadr(lstk(top)))
         top=tops
         fun=-1
         return
      endif
      mn=m*n*(it+1)
      
      l2 = l
c     
      if(lhs.eq.1) goto 48
      if(top+2.ge.bot) then
         call error(18)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      l2=sadr(il+4)
      err = l2+mn - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
      lstk(top+1)=l2+mn
c     
      xxx=0.0d0
      do 47 i1=0,mn-1
         xxx=max(xxx,abs(stk(l+i1)))
 47   continue
      if(xxx.gt.0.0d0) eps=eps*xxx
 48   do 49 i=1,mn
         i1=i-1
         xxx=stk(l+i1)
         call rat(abs(xxx),eps,ns,nt,err)
         if(xxx.lt.0.0d+0) ns=-ns
         if(err.gt.0) then
            call error(24)
            return
         endif
         stk(l+i1) = dble(ns)
         stk(l2+i1) = dble(nt)
         if (lhs .eq. 1) stk(l+i1) = dble(ns)/dble(nt)
 49   continue
      go to 999
 999  return
      end
