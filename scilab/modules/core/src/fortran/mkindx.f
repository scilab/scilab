c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine mkindx(n,m)
c     form the list which represents recursive index 
c     (i1)(i2)...(in)(k1,...,km)
c     i1,..in,k1,..km are supposed to be stored recursively in the stack
c!
      include 'stack.h'
      integer n,m
      
      if(err1.gt.0) return
      lhss=lhs
      if(m.gt.1) then
c     .  last is (i,j)
         rhs=m
         fin=1
         lhs=1
         call gwdatastructures1
      endif
      if(n.eq.0) goto 10
      rhs=n
      fin=1
      lhs=1
      call gwdatastructures1
      rhs=1
 10   continue
      lhs=lhss
      return
      end
