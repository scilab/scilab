c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intsize(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      integer sel,tops
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

      if(rhs.gt.2.or.rhs.lt.1) then
         call error(42)
         return
      endif
c
      sel=-1
      tops=top
c
      il=iadr(lstk(tops-rhs+1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      
      if(istk(il).eq.15.or.istk(il).eq.16) then
c     list or tlist case
c     ------------------
         ll=sadr(il+istk(il+1)+3)
         ilt=iadr(ll)
         if(istk(il).eq.16.and.istk(ilt).eq.10) then
            mnt=istk(ilt+1)*istk(ilt+2)
            if((istk(ilt+5).eq.2.and.istk(ilt+5+mnt).eq.27).or.
     +           (istk(ilt+5).eq.4.and.
     +           (istk(ilt+5+mnt).eq.21.and.istk(ilt+6+mnt).eq.28.and.
     +           istk(ilt+7+mnt).eq.28))) then
c     size of  'lss' or 'r' typed list
               top=tops
               call funnam(ids(1,pt+1),'size',il)
               fun=-1
               return
            endif
         endif
C     size of standard list
         if(lhs*rhs.ne.1) then
            err=1
            call error(39)
            return
         endif
         ilr=iadr(lstk(top))
         istk(ilr)=1
         n=istk(il+1)
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         l=sadr(ilr+4)
         stk(l)=dble(n)
         lstk(top+1)=l+1
c
      elseif(istk(il).le.10) then
c     matrix type variable type
c     -------------------------

         if(rhs.eq.2) then
            if(lhs.ne.1) then
               call error(41)
               return
            endif
            call getorient(top,sel)
            if(err.gt.0) return
            top=top-1
         endif
         m=istk(il+1)
         n=istk(il+2)
         l=sadr(il+4)
         ilr=iadr(lstk(top))
         istk(ilr)=1
         istk(ilr+1)=1
         lr=sadr(ilr+4)

         if(err.gt.0) return
         if(lhs.eq.1) then
            if(sel.eq.-1) then
               istk(ilr+2)=2
               istk(ilr+3)=0
               stk(lr) = m
               stk(lr+1) = n
               lstk(top+1)=lr+2
            elseif(sel.eq.1) then
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(lr) = m
               lstk(top+1)=lr+1
            elseif(sel.eq.2) then
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(lr) = n
               lstk(top+1)=lr+1
            elseif(sel.eq.0) then
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(lr) = m*n
               lstk(top+1)=lr+1
            else
               istk(ilr)=1
               istk(ilr+1)=1
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(lr) = 1.0D0
               lstk(top+1)=lr+1
            endif
         else
            istk(ilr)=1
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=0
            stk(lr) = m
            lstk(top+1)=lr+1
            top = top + 1
            ilr=iadr(lr+1)
            lr=sadr(ilr+4)
            err=lr+1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(ilr)=1
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=0
            stk(lr) = n
            lstk(top+1)=lr+1

         if(lhs.gt.2) then
            do k=3,lhs
               top = top + 1
               ilr=iadr(lr+1)
               lr=sadr(ilr+4)
               err=lr+1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
               istk(ilr)=1
               istk(ilr+1)=1
               istk(ilr+2)=1
               istk(ilr+3)=0
               stk(lr) = 1.0d0
               lstk(top+1)=lr+1
            enddo
         endif

         endif
      else
c     other cases
c     -----------
         top=tops
         call funnam(ids(1,pt+1),'size',il)
         fun=-1
      endif
      return
      end
c

      integer function mtlbsel(dims,ndims)
      integer dims(ndims),sel
      sel=0
      do 10 i=1,ndims
         if(dims(i).gt.1) then
            sel=i
            goto 20
         endif
 10   continue
 20   continue
      mtlbsel=sel
      end
c     -------------------------------
