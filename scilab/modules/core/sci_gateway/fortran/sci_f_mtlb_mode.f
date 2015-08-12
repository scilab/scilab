c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intmtlbmode
      include 'stack.h'

      logical checkrhs,checklhs,crebmat,getbmat,checkval
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /mtlbc/
cDEC$ ENDIF            
      common /mtlbc/ mmode
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('mtlb_mode',1,1)) return
      if(.not.checkrhs('mtlb_mode',0,1)) return

      if(rhs.eq.0) then
         top=top+1
         if(.not.crebmat('mtlb_mode',top,1,1,l)) return
         istk(l)=mmode
      else
         if(.not.getbmat('mtlb_mode',top,top,m,n,l)) return
         if(.not.checkval('mtlb_mode',m*n,1) ) return
         mmode=istk(l)
         call objvide('mtlb_mode',top)
      endif
      return
      end
