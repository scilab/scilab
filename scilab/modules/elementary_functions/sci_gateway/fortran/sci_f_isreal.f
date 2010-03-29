c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intisreal(id)
      INCLUDE 'stack.h'
      integer id(nsiz)
      double precision eps,mx
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(39)
         return
      endif
      
      if(rhs.eq.1) then
c     .  check for  real storage
         il=iadr(lstk(top))
         ilr=il
         if(istk(il).lt.0) il=iadr(istk(il+1))
         it=istk(il+3)
         
         if(istk(il).eq.1.or.istk(il).eq.2.or.istk(il).eq.5) then
            istk(ilr)=4
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=abs(1-it)
            lstk(top+1)=sadr(ilr+4)
         else
            call ref2val
            call funnam(ids(1,pt+1),'isreal',iadr(lstk(top)))
            fun=-1
            return
         endif
      else
c     .  check for zero imaginary part
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=2
            call error(53)
            return
         endif
         eps=stk(sadr(il+4))
         top=top-1

         il=iadr(lstk(top))
         ilr=il
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1.and.istk(il).ne.2.and.istk(il).ne.5) then
            top=top+1
            call ref2val
            call funnam(ids(1,pt+1),'isreal',iadr(lstk(top-1)))
            fun=-1
            return
         endif
         m=istk(il+1)
         n=istk(il+2)
         it=istk(il+3)
         if(it.eq.0) then
            istk(ilr)=4
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=abs(1-it)
            lstk(top+1)=sadr(ilr+4)
         else
            if(istk(il).eq.1) then
               mn=istk(il+1)*istk(il+2)
               l=sadr(il+4)+mn
            elseif(istk(il).eq.2) then
               mn=istk(il+8+m*n)-1
               l=sadr(il+9+istk(il+1)*istk(il+2))
            elseif(istk(il).eq.5) then
               mn=istk(il+4)
               l=sadr(ilr+5+m+mn)+mn
            endif

            mx=0.0d0
            if(mn.gt.0) then
               do 10 i=0,mn-1
                  mx=max(mx,abs(stk(l+i)))
 10            continue
            endif
            it=1
            if(mx.le.eps) it=0
            istk(ilr)=4
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=abs(1-it)
            lstk(top+1)=sadr(ilr+4)
         endif
      endif
      return
      end
c     -------------------------------
