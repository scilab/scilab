c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
    	subroutine inttan(id)
      INCLUDE 'stack.h'
c     
c     interface for tan
c     
      integer id(nsiz)

      integer iadr,sadr
     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

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
         call funnam(ids(1,pt+1),'tan',il1)
         fun=-1
         return
      endif

      if(istk(il).lt.0) then
c        argument is passed by reference
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
         do i=0,mn-1
            stk(lr+i)=tan(stk(l+i))
         enddo
      else
         do i=0,mn-1
            call wtan(stk(l+i),stk(l+mn+i),stk(lr+i),stk(lr+mn+i))
         enddo
      endif
      return
      end
c     -------------------------------
