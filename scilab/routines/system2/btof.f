      subroutine btof(x,nx)
c    scilab to fortran
c
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
      integer hsize
      double precision x
      dimension x(*)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).ne.1) goto 10
      hsize=4
      n=istk(il+1)*istk(il+2)*(istk(il+3)+1)
   40 if(n.ne.nx) then
         call error(98)
         return
      endif
      lx=sadr(il+hsize)
      call unsfdcopy(nx,stk(lx),1,x,1)
      top=top-1
      return
   10 if(istk(il).ne.2) then
         call error(98)
         return
      endif
      mn=istk(il+1)*istk(il+2)
      hsize=9+mn
      n=(istk(il+8+mn)-1)*(istk(il+3)+1)
      goto 40
      end
