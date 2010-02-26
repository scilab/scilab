c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) DIGITEO - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine whereismacro
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if (istk(il).lt.0) il=iadr(istk(il+1))
c
      if(istk(il).eq.11.or.istk(il).eq.13) then
         call putid(id,idstk(1,top))
      endif
      fin=-3
      call funs(id)
      if(err.gt.0) return
      if(fun.eq.0) then
         if(.not.cremat('whereis',top,0,0,0,lr,lc)) return
      else
         call putid(id,idstk(1,fun))
         il=iadr(lstk(top))
         istk(il)=10
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         istk(il+4)=1
         l=il+6
         fun=0
         call namstr(id,istk(l),n,1)
         istk(il+5)=n+1
         lstk(top+1)=sadr(l+n)
      endif
      return
      end
      
