      subroutine i_diag(id)
      INCLUDE '../stack.h'
      integer id(nsiz)
      external memused
      integer memused
c
      integer tops
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
      if(rhs.gt.2.or.rhs.lt.1) then
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

         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(89)
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
c
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
c     standard matrix case
      m=istk(il+1)
      n=istk(il+2)
      mn=m*n
      it=istk(il+3)
      l=il+4
      ref=il.ne.ilr
      lr=ilr+4

      if (m .eq. 1 .or. n .eq. 1) go to 63
c     
c     .  diag(a,k) with a a matrix
      if (k.ge.0) then
         mn=max(0,min(m,n-k))
      else
         mn=max(0,min(m+k,n))
      endif
      if(ref) then
         err=sadr(lr+memused(it,mn))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif
      if(mn.eq.0) then
         istk(ilr)=1
         istk(ilr+1)=0
         istk(ilr+2)=0
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+4)+1
         return
      else
         istk(ilr)=8
         istk(ilr+1)=mn
         istk(ilr+2)=1
         istk(ilr+3)=it
         lstk(top+1)=sadr(lr+memused(it,mn))
         call genmdiag(it,m,n,k,istk(l),istk(lr))
      endif
      return

c     .  diag(vector,k)
 63   mn=m*n
      nn = mn+abs(k)
      l1=lr+memused(it,nn*nn)
      err = sadr(l1 + memused(it,mn)) -  lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilr)=8
      istk(ilr+1)=nn
      istk(ilr+2)=nn
      istk(ilr+3)=it
      lstk(top+1)=sadr(lr+memused(it,nn*nn))
      if(ref) then
         l1=l
      else
         call gencopy(it,mn,istk(l),-1,istk(l1),-1)
      endif
      call gendiag(it,mn,k,istk(l1),istk(lr))
      return
      end

