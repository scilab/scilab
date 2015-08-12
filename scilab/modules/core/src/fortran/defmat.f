c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine defmat
c     ---------------------------------
C     Add an empty matrix on the stack 
C     and blank var on vars
C     ---------------------------------
      include 'stack.h'
      logical compil,cremat
      integer bl(nsiz)
      data bl/nsiz*673720360/
      if(err1.gt.0) return
c     compilation  [4]
      if(compil(4,0,0,0,0)) return 
      if(top+2 .ge. bot) then
         call error(18)
         return
      endif
      top = top+1
      call putid(idstk(1,top),bl)
      infstk(top)=0
      if (.not.cremat(fname,top,0,0,0,lr,lc)) return      
      return
      end
