c     -------------------------------
c			INRIA
c     -------------------------------
      subroutine getorient(k,sel)
      INCLUDE 'stack.h'
c
      integer sel,row,col,star
      integer iadr,sadr
c
      data row/27/,col/12/,star/47/,mtlb/22/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      sel=-1
      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.1) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         sel=stk(sadr(il+4))
         if(sel.ne.1.and.sel.ne.2) then
            err=2
            call error(44)
            return
         endif
      elseif (istk(il).eq.10) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(il+6).eq.row) then
            sel=1
         elseif(istk(il+6).eq.col) then
            sel=2
         elseif(istk(il+6).eq.star) then
            sel=0
         elseif(istk(il+6).eq.mtlb) then
            sel=-1
         else
            err=2
            call error(44)
            return
         endif
      else
         err=2
         call error(44)
         return
      endif
      return
      end

c     -------------------------------
