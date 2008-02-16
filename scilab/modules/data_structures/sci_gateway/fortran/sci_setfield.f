c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intsetfield()
      include 'stack.h'
      integer typs,topk
      logical eptover
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .ne. 3) then
         call error(39)
         return
      endif

      if ( eptover(1,psiz))  return
      rstk(pt)=0
      call putid(ids(1,pt),idstk(1,top))
      il3r=iadr(lstk(top))
      if(istk(il3r).lt.0) then
         if(abs(istk(il3r)).lt.15.or.abs(istk(il3r)).gt.17) then
            call error(44)
            return
         endif
         typs=abs(istk(il3r))
         il3=iadr(istk(il3r+1))
         if(typs.eq.17) then
c     change  type to tlist
            istk(il3r)=-16
            istk(il3)=16
         endif
      else
         err=3
         call error(118)
         return
      endif
      call intl_i
c     reset the proper variable type
      pt=pt-1
      if (fin.lt.0) call error(21)
      if(err.gt.0) then
         istk(il3)=typs
         return
      endif
      il=iadr(lstk(top))
      if(istk(il).ne.-1.or.istk(il+1).ne.-1) then
         buf='Inexpected error, please report'
         call error(8881)
      endif
      topk=istk(il+2)
      ilk=iadr(lstk(topk))
      istk(ilk)=typs
c     return an null variable
      istk(il)=0
      return
      end

