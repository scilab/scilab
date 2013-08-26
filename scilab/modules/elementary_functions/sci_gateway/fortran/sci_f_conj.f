c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
			subroutine intconj(id)
c     WARNING : argument of this interface may be passed by reference
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
      if(rhs.ne.1) then
         call error(39)
         return
      endif
      
      il=iadr(lstk(top))
      il1=il
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(abs(istk(il1)).gt.2) then
         call funnam(ids(1,pt+1),'conj',il1)
         fun=-1
         return
      endif

      if(istk(il).lt.0) then
         k=istk(il+2)
         err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1
     $        ,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

      if(istk(il).eq.1) then
         mn=m*n
         l=sadr(il+4)
      else
         l=sadr(il+9+m*n)
         mn=istk(il+8+m*n)-1
      endif
      if(it.eq.1) call dscal(mn,-1.0d+0,stk(l+mn),1)

      return
      end
c     -------------------------------
