      subroutine ftob(x,nx,topx)
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
c
c     fortran to scilab
      integer topx,hsize
      double precision x
      dimension x(*)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      ilx=iadr(lstk(topx))
      hsize=4
      if(istk(ilx).eq.2) hsize=9+istk(ilx+1)*istk(ilx+2)
      if(top.ge.bot) then
         call error(18)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      err=lstk(top)+sadr(hsize)+nx-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(hsize,istk(ilx),1,istk(il),1)
      l=sadr(il+hsize)
      call unsfdcopy(nx,x,1,stk(l),1)
      lstk(top+1)=l+nx
      return
      end
