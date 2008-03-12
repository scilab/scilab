c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intexp(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      double precision sr,si
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs .ne. 1) then
         call error(42)
         return
      endif

      il=iadr(lstk(top))

      il1=il
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.1) then
         call funnam(ids(1,pt+1),'exp',il1)
         fun=-1
         return
      endif

      if(istk(il).lt.0) then
c     argument is passed by reference
         ilr=il
         il=iadr(istk(il+1))
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=sadr(ilr+4)
         err=lr+mn*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il),1,istk(ilr),1)
         lstk(top+1)=lr+mn*(it+1)
      else
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=l
      endif
      
      if(mn.eq.0) return

      if(it.eq.0) then
         do 10 i=0,mn-1
            stk(lr+i)=exp(stk(l+i))
 10      continue
      else
         do 11 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            stk(lr+i)=exp(sr)*cos(si)
            stk(lr+i+mn)=exp(sr)*sin(si)
 11      continue
      endif
      return
      end
c     -------------------------------
