c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
			subroutine intimag(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
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
      if(rhs.ne.1) then
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
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

      if(istk(il).eq.1) then
         mn=m*n
         l=sadr(il+4)
         lr=sadr(ilr+4)
         if (ref) then
            err=lr+mn-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il),1,istk(ilr),1)
         endif
         if(it.eq.1) then
            call unsfdcopy(mn,stk(l+mn),1,stk(lr),1)
         else
            call dset(mn,0.0d+0,stk(lr),1)
         endif
         istk(ilr+3)=0
         lstk(top+1)=lr+mn
      elseif(istk(il).eq.2) then
         l=sadr(il+9+m*n)
         lr=sadr(ilr+9+m*n)
         id1=ilr+8
         mn=istk(il+8+m*n)-1
         if (ref) then
            err=lr+mn-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(9+m*n,istk(il),1,istk(ilr),1)
         endif
         if(it.eq.1) then
            call unsfdcopy(mn,stk(l+mn),1,stk(lr),1)
            ilw=iadr(lr+mn)
            call dmpcle(stk(lr),istk(id1),m,n,istk(ilw),0.0d0,0.0d0)
         else
            call dset(m*n,0.0d+0,stk(lr),1)
            do 21 i=1,m*n+1
               istk(id1-1+i)=i
 21         continue
         endif
         istk(ilr+3)=0
         lstk(top+1)=lr+istk(id1+m*n)

      elseif (istk(il) .eq. 5) then  ! sparse case added by Bruno
         nel = istk(il+4)
         l = sadr(il+5+nel+m) + nel
         if (it .eq. 1) then  ! compute nelr (for imag part)
            nelr = 0
            do i=0,nel-1
               if (stk(l+i) .ne. 0.d0) nelr = nelr+1
            enddo
         else
            nelr = 0
         endif
         lr = sadr(ilr+5+nelr+m)
         if (ref) then
            err=lr+nelr-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
*     form the resulting sparse matrix
         if (nelr .eq. nel) then
            if (ref) then
               call icopy(5+m+nel,istk(il),1,istk(ilr),1)
            endif
            call unsfdcopy(nel,stk(l),1,stk(lr),1)
         else
            istk(ilr) = 5
            istk(ilr+1) = m
            istk(ilr+2) = n
            istk(ilr+4) = nelr
            if (nelr .eq. 0) then
               call iset(m, 0, istk(ilr+5),1) ! mnel(i)=0 i=1..m 
            else
               if (.not.ref .and. nelr .lt. nel) then
                  lrs = lr
                  lr = l        ! else there will be a problem due to overlapping arrays
                                ! (the value result array will erase the end of the icol
                                ! array of the input)
               endif
               icol = il + 5 + m
               icolr = ilr + 5 + m
               k = l
               kr = lr
               do i = 0,m-1
                  mnelir = 0
                  do j = 1, istk(il+5+i) ! loop on non nul coef of row i+1
                     if (stk(k) .ne. 0.d0) then 
                        mnelir = mnelir + 1
                        istk(icolr) = istk(icol)
                        stk(kr) = stk(k)
                        icolr = icolr + 1
                        kr = kr + 1
                     endif
                     k = k + 1
                     icol = icol + 1
                  enddo
                  istk(ilr+5+i) = mnelir
               enddo
               if (.not.ref .and. nelr .lt. nel) then
                  call unsfdcopy(nelr,stk(lr),1,stk(lrs),1)
               endif
            endif
         endif
         istk(ilr+3) = 0 ! it = 0
         lstk(top+1)=lr+nelr
      else
         call funnam(ids(1,pt+1),'imag',il)
         fun=-1
         return
      endif
      return
      end
c     -------------------------------
