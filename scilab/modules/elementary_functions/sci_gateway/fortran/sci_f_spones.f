c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intspones(fname,id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      character*(*) fname
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if( rhs.ne.1) then
         call error(39)
         return
      endif
      
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) then
         il=iadr(istk(il+1))
         ref=.true.
      else
         ref=.false.
      endif
      if (istk(il).ne.5 .and. istk(il).ne.6) then
         if (istk(il).eq.7) then
            call funnam(ids(1,pt+1),'spones',il)
            fun=-1
         else
            buf=fname//': argument must be a sparse matrix'
            call error(999)
         endif
         return
      endif

      m=istk(il+1)
      nel=istk(il+4)
      lr = sadr(ilr+5+m+nel)
      lfin = lr + nel
      err = lfin - lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if ( ref ) then
         call icopy(5+m+nel,istk(il),1,istk(ilr),1)
      endif
      istk(ilr) = 5   ! type = sparse
      istk(ilr+3) = 0 ! it = 0
      call dset(nel, 1.d0, stk(lr), 1)
      lstk(top+1) = lfin
      return
      end
c     -------------------------------
