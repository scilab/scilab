      subroutine i_tril(id)
      INCLUDE '../stack.h'
      integer id(nsiz)
      external memused
      integer memused
c
      integer tops
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.gt.2) then
         call error(42)
         return
      endif

      tops=top
c
      k = 0
      if (rhs .eq. 2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1.and.istk(il).ne.8) then
            err=1
            call  error(53)
            return
         endif
         if(istk(il).eq.1) then
            if(istk(il+3).ne.0) then
               err=1
               call  error(52)
               return
            endif
            k = int(stk(sadr(il+4)))
         else
            call tpconv(istk(il+3), 4, 1, istk(il+4),1, k,1)
         endif
         top = top-1
      endif


      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
c     standard matrix case
      m=istk(il+1)
      n=istk(il+2)
      mn=m*n
      it=istk(il+3)
      l=il+4

      if(il.ne.ilr) then
         lr=ilr+4
         err=sadr(lr+memused(it,mn))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il),1,istk(ilr),1)
         call gencopy(it,mn,istk(l),1,istk(lr),1)
         lstk(top+1)=sadr(lr+memused(it,mn))
         l=lr
      endif
      call gentril(it,m,n,k,istk(l))
      return
      end
c     

