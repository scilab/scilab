c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine stackgl(id,n)
c     get variables out of a stored list
      include 'stack.h'
      integer iadr,sadr,fins,typ
      integer id(nsiz)
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if(n.eq.0) return
      fins=fin
      top=top+1
      ill=iadr(lstk(top))
      istk(ill)=1
      istk(ill+1)=1
      istk(ill+2)=n
      istk(ill+3)=0
      ll=sadr(ill+4)
      do 10 i=1,n
         stk(ll)=i
         ll=ll+1
 10   continue 
      lstk(top+1)=ll
      fin=-4
      call stackg(id)
      if(fin.eq.0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         if(err.gt.0) return
      endif
      il1=iadr(lstk(top))
      typ=abs(istk(il1))
      if (typ.ne.15) then 
         call putid(ids(1,pt+1),id)
         call error(140)
         return
      endif
      fin=3
      rhs=1
      lhss=lhs
      lhs=n
      call lstops
      if(err.gt.0) return
      lhs=lhss
      fin=fins
      return
      end
      
