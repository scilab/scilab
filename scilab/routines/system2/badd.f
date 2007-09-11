      subroutine badd(ny,t,y,ml,mu,p,nrowp)
c
c ======================================================================
c      interface pour l'external add de IMPL
c ======================================================================
c
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      double precision y(ny),p(nrowp,ny),t(*)
      common/ierode/iero
      logical allowptr
c     
      integer vol,tops,nordre
      data nordre/2/,mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     nordre=external number
c     mlhs (mrhs) = number ot output (input) parameters of the 
c     external 
      iero=0
      mrhs=3
c     
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)

c     external is a Scilab function
c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1

c     
c     transfer of input parameters
c+     
      call ftob(t,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(y,ny,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
c     
      top=top+1
      if(top+1.ge.bot) then
         call error(18)
         return
      endif
      err=lstk(top)+2+ny*ny-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ilp1=iadr(lstk(top))
      istk(ilp1)=1
      istk(ilp1+1)=ny
      istk(ilp1+2)=ny
      istk(ilp1+3)=0
      lp1=sadr(ilp1+4)
      call unsfdcopy(ny*ny,p,1,stk(lp1),1)
      lstk(top+1)=lp1+ny*ny
c     call ftob(p,ny,istk(il+3))
c+    
c     
      tops=istk(il)
      ils=iadr(lstk(tops))
      if(istk(ils).eq.15) goto 10
c+    
c     adress of external
      fin=lstk(tops)
c     
      goto 40
c     
c     external in a list
 10   nelt=istk(ils+1)
      l=sadr(ils+3+nelt)
      ils=ils+2
c     adress of external
      fin=l
c     
c     additional parameters
c     
      nelt=nelt-1
      if(nelt.eq.0) goto 40
      l=l+istk(ils+1)-istk(ils)
      vol=istk(ils+nelt+1)-istk(ils+1)
      if(top+1+nelt.ge.bot) then
         call error(18)
         return
      endif
      err=lstk(top+1)+vol-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(vol,stk(l),1,stk(lstk(top+1)),1)
      do 11 i=1,nelt
         top=top+1
         lstk(top+1)=lstk(top)+istk(ils+i+1)-istk(ils+i)
 11   continue
      mrhs=mrhs+nelt
 40   continue
c     
c     execute scilab external
c     
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5
c
      include '../callinter.h'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfer of output parameters of external to fortran
      call btof(p,ny*ny)
      if(err.gt.0.or.err1.gt.0) return
c+    
c     normal return
      iero=0
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
c     .  the error has been catched
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end
