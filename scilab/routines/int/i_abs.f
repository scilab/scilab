      subroutine i_abs(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE '../stack.h'
      integer id(nsiz)
      double precision pythag
      logical ref
      external memused
      integer memused
      integer head
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

      mn=m*n
      head=4
      l=il+4
      lr=ilr+4

      if(ref) then 
         err=sadr(lr+memused(mn,it))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(head,istk(il),1,istk(ilr),1)
      endif
      if(l.ne.lr) call gencopy(it,mn,istk(l),1,istk(lr),1)
      call genabs(it, mn, istk(lr), 1)
      lstk(top+1)=sadr(lr+memused(it,mn))
      return
      end
