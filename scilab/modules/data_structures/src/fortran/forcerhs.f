c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine forcerhs(n,ierr)
c     this routine allows to change the apparent lhs of a list
c     extraction when it extraction appears in an input argument
c     list. The lhs is changed  from 1 to the number of extracted
c     variables.
c
      integer n,ierr
      include 'stack.h'

      ierr=0
c      write(6,'(''forcerhs'',i5)') n
      if(rstk(pt-1).eq.617) then
c      in a compiled function
         pstk(pt-1)=pstk(pt-1)+n-1
      elseif(rstk(pt-1).eq.501) then
c      in a compiled function retained for 2.4.1  compatiblity
         ids(5,pt-1)=ids(5,pt-1)+n-1
      elseif(pt.ge.4) then
         if(rstk(pt-3).eq.307.or.rstk(pt-3).eq.702) then
            pstk(pt-3)=pstk(pt-3)-1+n
         else
            ierr=1
         endif
      else
         ierr=1
      endif
      return
      end

