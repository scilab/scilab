      subroutine btofm(x,ldx,nx,mx)
c    scilab to fortran matrix transfer
c
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
      double precision x
      dimension x(*)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(istk(il+3).eq.1) then
         err=1
         call error(82)
         return
      endif
      if(istk(il+1).ne.nx.or.istk(il+2).ne.mx) then
         err=1
         call error(98)
         return
      endif
      lx=sadr(il+4)
      call dmcopy(stk(lx),istk(il+1),x,ldx,nx,mx)
      top=top-1
      return
      end
