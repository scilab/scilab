      subroutine i_cumprod(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE '../stack.h'
      integer id(nsiz)
      logical ref
      integer sel,tops
      external memused,mtlbsel
      integer memused,mtlbsel
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      sel=0
      tops=top
c
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
      ref=ilr.ne.il0
c
      if(rhs.eq.2) then
         call getorient(top,sel)
         if(err.gt.0) return
         top=top-1
         if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)
      endif

      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      l=il0+4
      l1=ilr+4
      mn=m*n

c
      if(ref) then
         err=sadr(l1+memused(it,mn))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il0),1,istk(ilr),1)
         call gencopy(it,mn,istk(l),1,istk(l1),1)
      endif
      lstk(top+1)=sadr(l1+memused(it,mn))
      if(mn.eq.0) return

      call genmcuprod(it,sel,istk(l1),m,m,n)

      end
