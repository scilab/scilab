c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine getdimfromvar(k,num,n)
      integer k,num,n
c
      INCLUDE 'stack.h'
      integer iadr,sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      
      if(istk(il).eq.1) then
         if(istk(il+3).ne.0) then
            err=num
            call  error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=num
            call error(89)
            return
         endif
         n=max(int(stk(sadr(il+4))),0)
      elseif(istk(il).eq.8) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=num
            call error(89)
            return
         endif
         call tpconv(istk(il+3),4,1,istk(il+4),1,n,1)
         n=max(n,0)
      else
         err=num
         call  error(53)
         return
      endif
c     
      end
c     -------------------------------
      
