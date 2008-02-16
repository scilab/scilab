c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine mtlist(typdsc)
c     create a tlist whose type description is given by external
c     function typdsc
c
      include 'stack.h'
      integer iadr,sadr
      external typdsc
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lf=lstk(top+1)
      top=top+1-rhs
      ld=lstk(top)
c     get the size of type description variable
      call typdsc(n1,'size')

      err=lf+sadr(rhs+4+n1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      il=iadr(ld)
      l1=sadr(il+rhs+4)
      ilt=iadr(l1)
      l=sadr(ilt+n1)
      call unsfdcopy(lf-ld,stk(ld),-1,stk(l),-1)
      istk(il)=16
      istk(il+1)=rhs+1
c     set the type
      call typdsc(istk(ilt),'set')
      nt=sadr(ilt+n1)-l1
c     set the pointers
      istk(il+2)=1
      istk(il+3)=nt+1
      do 41 i=1,rhs
         istk(il+3+i)=istk(il+2+i)+lstk(top+i)-lstk(top-1+i)
 41   continue
      lstk(top+1)=l+lf-ld
      return
      end
