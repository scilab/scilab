      subroutine bjlsqrsolv(m,n,x,fvec,fjac,ldfjac,iero)
c     
c ======================================================================
c      external for   lsqrsolve
c ======================================================================
c
c     Copyright INRIA. 
c     Author : Serge Steer INRIA
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      integer n,iflag
      double precision x(n),fvec(m),fjac(ldfjac,*)
      logical allowptr
      integer vol,tops
      data mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c
      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' bjlsqrsolve  top:'//buf(1:4))
      endif
      iflag=iero
c
c     iflag est le numero d'ordre de cet external dans la structure
c     de donnee,
c     mlhs (mrhs) est le nombre de parametres de sortie (entree)
c     du simulateur 
c     
      iero=0
      mrhs=2
c     
      ilp=iadr(lstk(top))

      il=istk(ilp+iflag)
c
      tops=istk(il)
      ils=iadr(lstk(tops))
c
      if(istk(ils).eq.10) then
c     cas d'un simulateur en fortran
         if(iflag.eq.1) then
            call lsqrsolvf(m,n,x,fvec,iflag)
         else
            call lsqrsolvj(m,n,x,fjac,ldfjac,iflag)
         endif
         return
      endif
c     external is a Scilab function

c     on return iero=-1 is used to notify to the  solver that
c     scilab was not able to evaluate the external
      iero=-1
c     
c     transfert des arguments d'entree minimaux du simulateur
c     la valeur de ces arguments vient du contexte fortran (liste d'appel)
c     la structure vient du contexte 
c+    
      call ftob(x,n,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(dble(m),1,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return

c+    
c     
      if(istk(ils).eq.15) goto 10
c     
c     recuperation de l'adresse du simulateur
      fin=lstk(tops)
c     
      goto 40
c     cas ou le simulateur est decrit par une liste
 10   nelt=istk(ils+1)
      l=sadr(ils+3+nelt)
      ils=ils+2
c     
c     recuperation de l'adresse du simulateur
      fin=l
c     
c     gestion des parametres supplementaires du simulateur
c     proviennent du contexte  (elements de la liste
c     decrivant le simulateur
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
 40   continue
c     
c     execution de la macro definissant le simulateur
c     
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         goto 9999
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
      include "../callinter.h"
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfert des variables  de sortie vers fortran
      if (iflag.eq.1) then
         call btof(fvec,m)
      else
         call btof(fjac,m*n)
      endif
      if(err.gt.0.or.err1.gt.0) return
c+    
c     normal return iero set to 0
      iero=0 
      return
c     
 9999 continue
      niv=niv-1
      iflag=-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end
