      subroutine boptim(iero,n,x,f,g,izs,rzs,dzs)
c 
c ======================================================================
c     gestion des macros externals pour la primitive OPTIM
c ======================================================================
c
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr,sadr
c     
      integer tops,vol
      integer izs(*)
      real rzs(*)
      logical allowptr
c+    
      double precision x(n),f(*),g(n),dzs(*)
      double precision xind
      data nordre/1/,mlhs/3/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     nordre est le numero d'ordre de cet external dans la structure
c     de donnee,
c     mlhs (mrhs) est le nombre de parametres de sortie (entree)
c     du simulateur 
c     
      indsim=iero
      mrhs=2
c     
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)

c     on return iero=0 is used to notify to the solver that
c     scilab was not able to evaluate the external
      iero=0


c     
c     transfert des arguments d'entree minimaux du simulateur
c     la valeur de ces arguments vient du contexte fortran (liste d'appel)
c     la structure vient du contexte 
c+    
      call ftob(x,n,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(dble(indsim),1,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
c+    
c     
      tops=istk(il)
      ils=iadr(lstk(tops))
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
c     execution de la macro definissant le simulateur
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

      include '../callinter.h'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
      
c+    
c     transfert des variables  de sortie vers fortran
      call btof(xind,1)
      indsim=int(xind)
      if(err.gt.0.or.err1.gt.0) return
      call btof(g,n)
      if(err.gt.0.or.err1.gt.0) return
      call btof(f,1)
      if(err.gt.0.or.err1.gt.0) return
c+    
c     normal return iero set to 0
      iero=indsim 
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
