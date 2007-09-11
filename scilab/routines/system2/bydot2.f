      subroutine bydot2(n,t,y,ydot)
c     
c
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      common/ierode/iero
      common/odecd/nd,iflag
c     
      logical allowptr
      double precision t(*), y(*),ydot(*)
      integer vol,tops,nordre
      data nordre/1/,mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     
c     nordre=external number
c     mlhs (mrhs) = number ot output (input) parameters of the 
c     external 
c  
      iero=0
      mrhs=4
c     
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)
      il1=il
c
      tops=istk(il)
      ils=iadr(lstk(tops))
c
      if(istk(ils).eq.10) then
c     fortran external
         call fydot2(n,t,y,ydot)
         return
      endif
c     
c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1

c     transfer of input parameters
c+    
      call ftob(t,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return

      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=n
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      err=lstk(top)+sadr(4)+n-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(n,y,1,stk(l),1)
      lstk(top+1)=l+n

      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=nd
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      err=lstk(top)+sadr(4)+nd-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(nd,y(n+1),1,stk(l),1)
      lstk(top+1)=l+nd

      call ftob(dble(iflag),1,istk(il1+1))
      if(err.gt.0.or.err1.gt.0) return
c  *****************************************
c+    
c     
c     adress of external
      fin=lstk(tops)
c     
c     external in a list
      if(istk(ils).eq.15) then
      nelt=istk(ils+1)
      l=sadr(ils+3+nelt)
      ils=ils+2
c     
c     adress of external
      fin=l
c     
c     additional parameters
c     
      nelt=nelt-1
      if(nelt.ne.0) then
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
 11      continue
         mrhs=mrhs+nelt
      endif
      endif
c     
c     execute scilab external
c     
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
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

      include '../callinter.h'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfer of output parameters of external to fortran
      if(iflag.eq.0) then
         call btof(ydot,n)
      else
         call btof(ydot,nd)
      endif
      if(err.gt.0.or.err1.gt.0) return
c+    
c     normal return iero set to 0
      iero=0 
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end
