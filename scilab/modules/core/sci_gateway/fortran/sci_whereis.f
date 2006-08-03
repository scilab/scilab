      subroutine intwhereis
c     Copyright INRIA
      include 'stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
      topk=top
c
      if(.not.checklhs('whereis',1,1)) return
      if(.not.checkrhs('whereis',1,1)) return


      il=iadr(lstk(top))
      if (istk(il).lt.0) il=iadr(istk(il+1))
c
      if(istk(il).eq.11.or.istk(il).eq.13) then
         call putid(id,idstk(1,top))
      elseif(istk(il).eq.10) then
         if(.not.getsmat('whereis',topk,top,m,n,1,1,l,n1)) return
         if(.not.checkval('whereis',m*n,1) ) return
         call namstr(id,istk(l),n1,0)
      else
         err=1
         call error(44)
         return
      endif
      fin=-3
      call funs(id)
      if(err.gt.0) return
      if(fun.eq.0) then
         if(.not.cremat('whereis',top,0,0,0,lr,lc)) return
      else
         call putid(id,idstk(1,fun))
         il=iadr(lstk(top))
         istk(il)=10
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         istk(il+4)=1
         l=il+6
         fun=0
         call namstr(id,istk(l),n,1)
         istk(il+5)=n+1
         lstk(top+1)=sadr(l+n)
      endif
      return
      end