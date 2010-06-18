c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c Copyright (C) INRIA - 2010 - Serge Steer
c Copyright (C) DIGITEO - 2010 - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intrat
      INCLUDE 'stack.h'
c      
c     [N,D]=rat(x [,tol])
c     y=rat(x [,tol])      
c     
      character tmpbuf * (bsiz)
      integer tops
      double precision eps, xxx
      integer iadr, sadr
      logical ref
      integer il, m, n, it, l
      integer ns, nt
      integer iln, ln, ild, ld      
c
     
      iadr(l) = l + l - 1
      sadr(l) = (l / 2) + 1
c     
      if (ddt .eq. 4) then
         write(tmpbuf(1:4), '(i4)') fin
         call basout(io, wte, ' intrat '//tmpbuf(1:4))
      endif
c     
      tops = top
c
      if(rhs .gt. 2) then
        call error(42)
        return
      endif
      if(lhs .gt. 2) then
        call error(41)
        return
      endif

      if(rhs .eq. 2) then
c     .  check tolerance
         il = iadr(lstk(top))
         if (istk(il) .lt. 0) il = iadr(istk(il + 1))
         if (istk(il) .ne. 1) then
            err = 2
            call error(52)
            return
         endif
         eps = stk(sadr(il + 4))
         top = top - 1
      else
         eps = 1.d-6
      endif

c     get x
      il = iadr(lstk(top))
      ref = istk(il) .lt. 0
      if(ref) il = iadr(istk(il + 1))
      if (istk(il) .ne. 1) then
        call funnam(ids(1, pt + 1), 'rat', iadr(lstk(top)))
        top = tops
        fun = -1
        return
      endif
      
      l = sadr(il + 4)
      m = istk(il + 1)
      n = istk(il + 2)
      it = istk(il + 3)
      if(it .ne. 0) then
        call funnam(ids(1, pt + 1), 'rat', iadr(lstk(top)))
        top = tops
        fun = -1
        return
      endif
      mn = m * n
      
c     
c     "Allocate" output variables
      if (ref) then
c     .  x as been passed by reference, "allocate" n in place of the reference
        iln = iadr(lstk(top))
        ln = sadr(iln + 4)
        lstk(top + 1) = ln + mn
      else
c     .  x as been passed by value, n will replace x
        iln = il
        ln = l
      endif
 
      if(lhs .eq. 2) then
c     "allocate" d
        if(top+2 .ge. bot) then
            call error(18)
            return
        endif
        top = top + 1
        ild = iadr(lstk(top))
        ld = sadr(ild + 4)
        lstk(top + 1) = ld + mn
      endif

c     Check if there is enough room in the stack 
      err = lstk(top + 1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif

c     Create output variable headers
c     n
      istk(iln) = 1
      istk(iln + 1) = m
      istk(iln + 2) = n
      istk(iln + 3) = it
      lstk(top + 1) = ln + mn
      if (lhs .eq. 2) then
c     .  d
        istk(ild) = 1
        istk(ild + 1) = m
        istk(ild + 2) = n
        istk(ild + 3) = it
        lstk(top + 1) = ld + mn
      endif

c     Make tolerance relative to the element with maximum absolute value
      xxx = 0.0d0
      do i1 = 0, mn - 1
        xxx = max(xxx, abs(stk(l + i1)))
      enddo
      
      if(xxx .gt. 0.0d0) eps = eps * xxx

c     Loop on each entries 
      do i1 = 0, mn - 1
c     .  compute the decomposition
        xxx = stk(l + i1)
        call rat(abs(xxx), eps, ns, nt, err)
        if(err .gt. 0) then
            call error(24)
            return
        endif
        
        if(xxx .lt. 0.0d+0) ns = -ns

c     .  store
        if(lhs .eq. 2) then
            stk(ln + i1) = dble(ns)
            stk(ld + i1) = dble(nt)
         else
            stk(ln + i1) = dble(ns) / dble(nt)
         endif
      enddo
      go to 999
 999  return
      end
