      logical function scischur(re,im)
      INCLUDE 'stack.h'
      logical scifunction, createcvar
      common /scisch/ lf, nx, nf
      integer iadr
      double precision re, im
c     
      iadr(l) = l+l-1
c     
      scischur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=re
      stk(lx+1)=im
      if(.not.scifunction(nx,lf,1,1)) return
c     stk(lx)=fct([re,im])  evaluated by scilab fct pointed to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scischur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scischur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scichk()
c     checks fct passed to schur
      INCLUDE 'stack.h'
      logical scifunction, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scisch/ lf, nx, nf
      iadr(l) = l+l-1
      scichk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.scifunction(nx,lf,1,1)) then
c     error into fct passed to schur (schur(A,tst))
         return
      endif
c     check return value of fct
      ilx=iadr(lx-2)
      if(istk(ilx).ne.1 .and. istk(ilx).ne.4) then
         err=nf
         call error(268)
         return
      endif
      if(istk(ilx+1)*istk(ilx+2).ne.1) then
         err=nf
         call error(268)
         return
      endif
      scichk=.true.
      return
      end


