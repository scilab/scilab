c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine dmpcle(pm1,d1,m,n,d2,epsr,epsa)
c!but
c     Dans une matrice polynomiale mp1 cette routine met (pour
c     chaque polynome) a zero les coefficients de polynomes
c     de module inferieur a epsa ou epsr*(norme un du vecteur
c     des coefficients du polynome)
c
c!listed'appel
c
c     subroutine dmpclea(pm1,d1,m,n,d2,epsr,epsa)
c     double precision pm1(*),epsr,epsa
c     integer d1(*),d2(*),m,n
c
c     pm1 : tableau reel contenant les coefficients des polynomes,
c     le coefficient de degre k du polynome pm1(i,j) est range
c     dans pm1( d1(i + (j-1)*ld1 + k) )
c     pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c     d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c     du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c     d1(k+1)-d1(k) -1
c     m : nombre de ligne de la matrice pm1
c     n : nombre de colonne de matrice pm1
c     d2 : tableau de travail meme taille que d1
      double precision pm1(*),norm,epsr,epsa,eps
      integer d1(*),d2(*),m,n,count
      logical ok
c
c
c
      mn=m*n
      ld1=mn+1
      if (mn.eq.1) then
         lmin=d1(1)
         lmax=d1(2)-1
         norm=0.d0
         do 10 l=lmin,lmax
            norm=norm+abs(pm1(l))
 10      continue
         eps=max(epsa,epsr*norm)
         ll=lmax+1
         count=0
         ok=.false.
         do 20 k=lmin,lmax
            ll=ll-1
            if (abs(pm1(ll)).le.eps) then
               pm1(ll)=0.d0
               if (ll.eq.lmax) ok=.true.
               if (ok.eqv..true.) count=count+1
            else
               ok=.false.
            endif
 20      continue
         d1(2)=d1(2)-count
         if (d1(2).le.d1(1)) d1(2)=d1(1)+1
         return
      endif
      do 30 k=1,ld1
         d2(k)=d1(k)
 30   continue
      do 70 k=1,mn
         lmin=d2(k)
         lmax=d2(k+1)-1
         norm=0.d0
         do 40 l=lmin,lmax
            norm=norm+abs(pm1(l))
 40      continue
         eps=max(epsa,epsr*norm)
         ll=lmax+1
         count=0
         ok=.false.
         do 50 l=lmin,lmax
            ll=ll-1
            if (abs(pm1(ll)).le.eps) then
               if (ll.eq.lmax) ok=.true.
               if (ok.eqv..true.) count=count+1
               pm1(ll)=0.d0
            else
               ok=.false.
            endif
 50      continue
         d1(k+1)=d1(k)+d2(k+1)-d2(k)-count
         if (d1(k+1).le.d1(k)) d1(k+1)=d1(k)+1
 70   continue
      l1=d1(2)
      do 90 k=2,mn
         lmin=d2(k)
         ivol=d1(k+1)-d1(k)
         do 80 l=0,ivol-1
            pm1(l1+l)=pm1(lmin+l)
 80      continue
         l1=l1+ivol
 90   continue
      return
      end
