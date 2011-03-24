c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intlstcat
c     catenate two or more lists
      include 'stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .lt. 1) then
         call error(39)
         return
      endif
      if (lhs .gt. 1) then
         call error(41)
         return
      endif
c
      if(rhs.eq.1) return
c
      lw=lstk(top+1)
      nel=0
      n1=0
      do 10 i=rhs-1,0,-1
         il=iadr(lstk(top-i))
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            nel=nel+1
            n1=n1+1
         else
            nel=nel+istk(il+1)
         endif
 10   continue
      if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) n1=n1+3
c     computes table of entry sizes istk(ilp+i),total size isz
      ilp=iadr(lw)+n1
      iln=ilp+nel
      err=sadr(iln)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ill=ilp-1
      isz=0
      do 12 i=rhs-1,0,-1
         il=iadr(lstk(top-i))
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            ill=ill+1
            istk(ill)=lstk(top-i+1)-lstk(top-i)
            isz=isz+istk(ill)
         else
            if(istk(il+1).gt.0) then
               do 11 j=1,istk(il+1)
                  ill=ill+1
                  istk(ill)=istk(il+2+j)-istk(il+1+j)
                  isz=isz+istk(ill)
 11            continue
            endif
         endif
 12   continue
c     copy entries
      ln=sadr(iln+3+nel)
      le=ln+isz
      err=le-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ill=ilp+nel
      do 20 i=1,rhs
         l=lstk(top+1-i)
         il=iadr(l)
         if(istk(il).ne.15.and.istk(il).ne.16.and.istk(il).ne.17) then
            lo=l
            ill=ill-1
            n=istk(ill)
         else
            ill=ill-istk(il+1)
            lo=sadr(il+3+istk(il+1))
            n=istk(il+2+istk(il+1))-1
         endif
         if(n.gt.0) then
            le=le-n
            call unsfdcopy(n,stk(lo),1,stk(le),1)
         endif
 20   continue
      top=top-rhs+1
      il=iadr(lstk(top))
      istk(il)=15
      istk(il+1)=nel
      istk(il+2)=1
      do 21 i=1,nel
         istk(il+2+i)=istk(il+1+i)+istk(ilp-1+i)
 21   continue
      l=sadr(il+3+nel)
      call unsfdcopy(isz,stk(ln),1,stk(l),1)
      lstk(top+1)=l+isz
      return
      end
