c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      logical function allowptr(ifun)
c     this function returns true if the interface given by the index
c     ifun allow to pass the input arguments by address.
      integer ifun
      logical r
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /ippty/
cDEC$ ENDIF      
      common /ippty/ byptr,nbyptr
      r=.false.
      do 10 i=1,nbyptr
         if(byptr(i).eq.ifun) then
            r=.true.
            goto 20
         endif
 10   continue
 20   allowptr=r
      end
