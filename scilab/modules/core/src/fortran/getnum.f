c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine getnum
c----------------------------------------------
c create a scalar variable
c x=stk(lstk(isiz)) ==> stored in data base 
c----------------------------------------------
      include 'stack.h'
      double precision x
      integer bl(nsiz),ix(2),lr,lc
      logical cremat,compil
      equivalence (x,ix(1))
      data bl/nsiz*673720360/
      if (err1.gt.0) return
      x=stk(lstk(isiz))
      if (compil(6,ix,0,0,0)) return
c     compilation getnum :<6 num>
      if (top+2 .ge. bot) then
         call error(18)
         return
      endif
      top = top+1
      call putid(idstk(1,top),bl)
      infstk(top)=0
      if (.not.cremat(' ',top,0,1,1,lr,lc)) return  
      stk(lr)=x 
      return
      end
