c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Francois DELBECQUE
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine residu(p,np,a,na,b,nb,v,tol,ierr)
c     but! calcul de residus
c     calcul de la somme des residus de p/(a.b)
c     aux zeros de a
c     p=polynome de degre np
c     a=                  na
c     b=                  nb
c
c     les zeros de b sont supposes tous differents des
c     zeros de a....
c
c     a,b et p dimensionnes au moins a leur degre+1 dans le pgm
c     appelant.
c     rangement par degres croissants.
c     v=resultat
c     ierr=0 O.K.
c     ierr=1 mauvais appel
c     si a et b ont une racine commune
c     on teste la division par zero par rapport a tol
c     principe du calcul:si a (resp b) est une constante on a
c     v=p(nb)/b(nb+1)/a(1)    (resp v=p(na)/a(na+1)/b(1) )
c     sinon on remplace p et a par le reste de la division
c     euclidienne de p et a par b,puis on inverse les roles
c     de a et b en changeant le signe de v.
c     on itere jusqu a trouver degre de a ou degre de b=0.
c     routines appelees:dpodiv,idegre (bibli blaise Inria)
c
      dimension a(*),b(*),p(*)
      double precision a,b,p,v,r,b1,tol
      v=0.0d+0
      ierr=0
      npp=np
      call idegre(a,na,na)
      call idegre(b,nb,nb)
      if(na.eq.0) return
c
c     b=constante v=... et return
      if(nb.eq.0) then
         b1=b(1)
         if(b1.eq.0.0d+0) then
            ierr=1
            return
         endif
         if(npp.ge.na-1) then
            v=p(na)/a(na+1)/b1
            return
         else
            v=0.0d+0
            return
         endif
      endif
c
c     degre de b >= 1
c
      if(na.le.np) then
c     p=p/a  (reste de la division euclidienne...)
         call dpodiv(p,a,np,na)
         call idegre(p,na-1,np)
      endif
      if(na.le.nb) then
c     b=b/a  (reste de la div euclidienne...)
         call dpodiv(b,a,nb,na)
         call idegre(b,na-1,nb)
      endif
c     ici nb=degre de b < na=degre de a
c     et np=degre de p < na=degre de a
c     si degre de a=1 (b et p =cstes) v=... et return
      if(na.eq.1) then
         b1=b(1)
         if(abs(b1).le.tol) then
            ierr=1
            return
         endif
         v=p(na)/a(na+1)/b1
         return
      endif
c
c     si degre de b=0 v=... et return
      call idegre(b,min(na-1,nb),nb)
      if(nb.eq.0) then
         b1=b(1)
         if(abs(b1).le.tol) then
            ierr=1
            return
         endif
         if(npp.ge.na-1) then
            v=p(na)/a(na+1)/b1
            return
         else
            v=0.0d+0
            return
         endif
      endif
c     si degre de b>0 on itere
      nit=0
 20   continue
      if(nit.ge.1) na=nbb
      nit=nit+1
      nbb=nb
c     a=a/b   (reste de la division euclidienne...)
c     p=p/b   (reste de la division euclidienne...)
c
      call dpodiv(a,b,na,nb)
      call idegre(a,nb-1,na)
      call dpodiv(p,b,np,nb)
      call idegre(p,nb-1,np)
c     b=-a
      do 30 k=1,nb+1
         r=b(k)
         b(k)=-a(k)
         a(k)=r
 30   continue
c
c     si degre de b=0 v=...
c
      call idegre(b,na,nb)
      if(nb.eq.0) then
         b1=b(1)
         if(abs(b1).le.tol) then
            ierr=1
            v=0.0d+0
            return
         endif
         v=p(nbb)/a(nbb+1)/b1
         return
      endif
c     sinon goto 20
      goto 20
      end
