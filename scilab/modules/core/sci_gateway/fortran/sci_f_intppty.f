c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intintppty()
      include 'stack.h'
      logical checkrhs,checklhs,cremat,getscalar
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /ippty/
cDEC$ ENDIF            
      common /ippty/ byptr,nbyptr
      integer iadr
c
c    
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
      if(.not.checkrhs('intppty',0,1)) return
      if(.not.checklhs('intppty',1,1)) return
      
      if(rhs.eq.0) then
c     query
         top=top+1
         if(.not.cremat('intppty',top,0,1,nbyptr,l,lc)) return
         call int2db(nbyptr,byptr,1,stk(l),1)
      else
c     set
         if(.not.getscalar('intppty',top,top,l)) return
         num=stk(l)
         call setippty(num)
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
      endif
      return
      end

      integer function isbyref(ifun)
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /ippty/
cDEC$ ENDIF            
      common /ippty/ byptr,nbyptr
      do 10 i=1,nbyptr
         if(byptr(i).eq.ifun) then
            isbyref=1
            return
         endif
 10   continue
      isbyref=0
      end
      
