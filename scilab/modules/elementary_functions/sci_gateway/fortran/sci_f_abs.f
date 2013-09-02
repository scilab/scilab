c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) DIGITEO - 2011 - Michael BAUDIN
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intabs(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      double precision dlapy2
      logical ref
      integer head
      integer iadr,sadr
      double precision x, y, z
      double precision mynan
c     
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(39)
         return
      endif
      
      il = iadr(lstk(top))
      ilr = il
      if(istk(il).lt.0) then
         il = iadr(istk(il + 1))
         ref = .true.
      else
         ref = .false.
      endif
      m = istk(il + 1)
      n = istk(il + 2)
      it = istk(il + 3)

      if(istk(il).eq.1) then
         mn = m*n
         head = 4
         l = sadr(il + 4)
         lr = sadr(ilr + 4)
      elseif(istk(il).eq.5) then
         mn = istk(il + 4)
         head = 5 + m + mn
         l = sadr(il + head)
         lr = sadr(ilr + head)
      elseif(istk(il).eq.2) then
         head = 9 + m*n
         l = sadr(il + head)
         mn = istk(il + 8 + m*n) - 1
         lr = sadr(ilr + head)
      else
         call funnam(ids(1, pt + 1), 'abs', il)
         fun = -1
         return
      endif
      if(ref) then
         err = lr + mn - lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(head, istk(il), 1, istk(ilr), 1)
      endif
      l1 = l - 1
      lr1 = lr - 1
      call returnananfortran(mynan)
      if(it.eq.0) then
         do 11 i = 1,mn
            stk(lr1 + i) = abs(stk(l1 + i))
 11      continue
      else
         k1 = l1 + mn
         do 13 i = 1, mn
              x = stk(l1+i)
              y = stk(k1+i)
              if ( (isanan(x).eq.1).or. (isanan(y).eq.1) ) then
                  z = mynan
              else
                  z = dlapy2(x, y)
              endif
              stk(lr1 + i) = z
 13      continue
         istk(ilr + 3) = 0
      endif
      lstk(top + 1) = lr + mn
      return
      end
c     -------------------------------

