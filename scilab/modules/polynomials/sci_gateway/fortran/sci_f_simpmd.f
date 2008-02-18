c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intsimpmd(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer simpmd
      common/csimp/ simpmd
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.1) then
         call error(39)
         return
      endif
      if(rhs.le.0) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=4
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=simpmd
         lstk(top+1)=sadr(il+4)
      else
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.4) then
            err=1
            call error(208)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(36)
            return
         endif
         simpmd=istk(il+3)
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=sadr(il+1)
      endif
      return
      end
c			=======================================
