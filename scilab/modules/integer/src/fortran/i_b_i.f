c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine i_b_i
c     implicit vector
      include 'stack.h'

Cc (DLL Intel Fortran)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /mtlbc/
cDEC$ ENDIF
      common /mtlbc/ mmode
      
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1     
c
      ityp=0
c     
      il1=iadr(lstk(top-rhs+1))
      if(abs(istk(il1)).ne.8) then
         fin=-fin
         return
      endif
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ityp=istk(il1)
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=il1+4
      mn1=m1*n1

      il2=iadr(lstk(top-rhs+2))
      if(abs(istk(il2)).ne.8) then
         fin=-fin
         return
      endif
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=il2+4
      mn2=m2*n2

      if(rhs.eq.3 ) then
         il3=iadr(lstk(top-rhs+3))
         if(abs(istk(il3)).ne.8) then
            fin=-fin
            return
         endif
         if(istk(il3).lt.0) il3=iadr(istk(il3+1))
         m3=istk(il3+1)
         n3=istk(il3+2)
         it3=istk(il3+3)
         l3=il3+4
         mn3=m3*n3
      else
         it3=it2
         mn3=mn2
      endif

      if(it1.ne.it2.or.it1.ne.it3) then
         fin=-fin
         return
      endif
      if (mn1.ne.1) then
         err=1
         call error(204)
      endif
      if (mn2.ne.1) then
         err=2
         call error(204)
      endif
      if (mn3.ne.1) then
         err=3
         call error(204)
      endif

      if (rhs.eq.2) then
         call genimpl2dim(it1,istk(l1),istk(l2),nr)
         if (nr.eq.0) goto 10

         err=sadr(l1+memused(it1,nr))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call genimpl2(it1,istk(l1),istk(l2),istk(l1))
      else
         call genimpl3dim(it1,istk(l1),istk(l2),istk(l3),nr)
         if (nr.le.0) goto 10
         err=sadr(l1+memused(it1,nr))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call genimpl3(it1,istk(l1),istk(l2),istk(l3),istk(l1))
      endif
      top=top-rhs+1
      istk(il1)=8
      istk(il1+1)=1
      istk(il1+2)=nr
      istk(il1+3)=it1
      lstk(top+1)=sadr(l1+memused(it1,nr))
      return
 10   top=top-rhs+1
      istk(il1)=1
      istk(il1+1)=0
      istk(il1+2)=0
      istk(il1+3)=0
      lstk(top+1)=sadr(il1+4)
      return
      end

