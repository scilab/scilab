      subroutine i_prod(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE '../stack.h'
      integer id(nsiz)
      logical ref
      integer sel,tops
      integer iadr,sadr

      external memused,mtlbsel
      integer memused,mtlbsel
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
      tops=top
      sel=0
c     
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
      ref=ilr.ne.il0
c     
      if(rhs.eq.2) then
         call  getorient(top,sel)
         if(err.gt.0) return
         if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)
         top=top-1
      endif
      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      mn=m*n
      l1=ilr+4
      l=il0+4
      if(mn.eq.0) then
         if(ref) then
            err=sadr(l1+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         if(sel.eq.0) then
            istk(ilr)=8
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=it
            call tpconv(4,it,1,0,1,is2,1)
            call gencopy(1,is2,1,istk(l1),1)
            lstk(top+1)=sadr(l1+1)
         else
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=l1
         endif
         return
      endif

      if(sel.eq.0) then
         mr=1
         nr=1
      elseif(sel.eq.1) then
         mr=1
         nr=n
      else
         mr=m
         nr=1
      endif
      if(ref) then
         err=sadr(l1+memused(it,mr*nr))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif
      istk(ilr)=8
      istk(ilr+1)=mr
      istk(ilr+2)=nr
      istk(ilr+3)=it
      l1=ilr+4

      call genmprod(it,sel,istk(l),m,m,n,istk(l1),1)

      lstk(top+1)=sadr(l1+memused(it,mr*nr))
      return
      end

