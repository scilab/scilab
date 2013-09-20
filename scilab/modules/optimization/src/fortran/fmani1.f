c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine fmani1 (mode,n,d,w,indi)
c
      implicit double precision (a-h,o-z)
      dimension d(n),w(n),indi(n)
c
      if(mode.eq.-1) go to 20
      do 10 i=1,n
   10 w(indi(i))=d(i)
      return
   20 do 30 i=1,n
   30 w(i)=d(indi(i))
      return
      end
