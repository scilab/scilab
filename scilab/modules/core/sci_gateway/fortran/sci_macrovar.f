c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 
      subroutine intmacrovar

      include 'stack.h'
      logical checkrhs,checklhs,ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(.not.checklhs('macrovar',1,1)) return
      if(.not.checkrhs('macrovar',1,1)) return


      ilfun=iadr(lstk(top))
      if (istk(ilfun).lt.0) then
         illist=ilfun
         ilfun=iadr(istk(ilfun+1))
         ref=.true.
      else
         illist=iadr(lstk(top+1))
         ref=.false.
      endif

      if(istk(ilfun).ne.13) then
         err=1
         call error(44)
         return
      endif


      call tradsl(ilfun,idstk(1,top),illist,nlist)
      if(err.gt.0) return
c     store result
      if(.not.ref) then
         il=iadr(lstk(top))
         call icopy(nlist,istk(illist),1,istk(il),1)
         lstk(top+1)=sadr(il+nlist)
      else
         lstk(top+1)=sadr(illist+nlist)
      endif
      return
      end
