c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
    	subroutine intexpm(id)
      INCLUDE 'stack.h'
      integer id(nsiz)
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
         call funnam(ids(1,pt+1),'expm',il1)
         fun=-1
         return
      endif

      if(istk(il).lt.0) then
c     argument is passed by reference
         ilr=il
         il=iadr(istk(il+1))
         m=istk(il+1)
         n=istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=sadr(ilr+4)
         err=lr+m*n*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il),1,istk(ilr),1)
         call unsfdcopy(m*n*(it+1),stk(l),1,stk(lr),1)
         lstk(top+1)=lr+m*n*(it+1)
      else
         m=istk(il+1)
         n=istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=l
      endif

      nn=m*n
      if(nn.eq.0) return

      if(m.ne.n) then
         err=1
         call error(2)
         return
      endif

      le=lstk(top+1)
      lw=le+nn*(it+1)
      ilb=iadr(lw+4*nn*(it+1)+5*n+2*n*it)
      err=sadr(ilb+n+n)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(it.eq.0) then
         call dexpm1(n,n,stk(lr),stk(le),n,stk(lw),istk(ilb),err)
      else
         call wexpm1(n,stk(lr),stk(lr+nn),n,stk(le),stk(le+nn),
     *        n,stk(lw),istk(ilb),err)
      endif
      if(err.ne.0) then
         call error(24)
         return
      endif
      call unsfdcopy(nn*(it+1),stk(le),1,stk(lr),1)
      return
      end
c     -------------------------------    
