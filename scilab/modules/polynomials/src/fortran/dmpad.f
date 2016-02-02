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
C/MEMBR ADD NAME=DMPAD,SSI=0
c     Copyright INRIA
      subroutine dmpad(pm1,d1,l1,pm2,d2,l2,pm3,d3,m,n)
c!but
c     cette subroutine ajoute deux matrices dont les coefficients
c     sont des polynomes: pm3=pm1+pm2
c!listed'appel
c
c     subroutine dmpad(pm1,d1,l1,pm2,d2,l2,pm3,d3,m,n)
c     double precision pm1(*),pm2(*),pm3(*)
c     integer d1(l1*n+1),d2(l2*n+1),d3(m*n+1),m,n,l1,l2
c
c     pm1 : tableau reel contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*l1 + k) )
c           pm1 doit etre de taille au moins d1(l1*n+1)-d1(1)
c     d1 : tableau entier de taille l1*n+1,  si k=i+(j-1)*l1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     l1 : entier definissant le rangement dans d1
c
c     pm2,d2,l2 : definitions similaires a celles de pm1,d1,l1
c     pm3,d3 : definitions similaires a celles de pm1 et d1, l3 est
c              suppose egal a m
c     m : nombre de ligne des matrices pm
c     n : nombre de colonnes des matrices pm
c!
      double precision pm1(*),pm2(*),pm3(*),eps,w
      double precision dlamch
      integer d1(*),d2(*),d3(*),m,n,l1,l2
c
      integer n1,n2,n3,mn,i,k
c
      eps=dlamch('p')
      mn=m*n
      d3(1)=1
c
      i1=-l1
      i2=-l2
      k3=0
c     boucle sur les polynomes
      do 41 j=1,n
         i1=i1+l1
         i2=i2+l2
         do 40 i=1,m
            k1=d1(i1+i)-1
            k2=d2(i2+i)-1
            n1=d1(i1+i+1)-d1(i1+i)
            n2=d2(i2+i+1)-d2(i2+i)
            if(n1.gt.n2) goto 30
c
c     n1.lt.n2
c
 20         do 21 k=1,n1
               w=pm1(k1+k)+pm2(k2+k)
               if(abs(w).gt.max(abs(pm1(k1+k)),abs(pm2(k2+k)))*eps) then
                  pm3(k3+k)=w
               else
                  pm3(k3+k)=0.0d+0
               endif
 21         continue
            if(n1.eq.n2) goto 23
            n3=n1+1
            do 22 k=n3,n2
               pm3(k3+k)=pm2(k2+k)
 22         continue
 23         n3=n2
            d3(i+1+(j-1)*m)=d3(i+(j-1)*m)+n3
            goto 38
c
c     n1.gt.n2
c
 30         do 31 k=1,n2
               w=pm1(k1+k)+pm2(k2+k)
               if(abs(w).gt.max(abs(pm1(k1+k)),abs(pm2(k2+k)))*eps) then
                  pm3(k3+k)=w
               else
                  pm3(k3+k)=0.0d+0
               endif
 31         continue
            n3=n2+1
            do 32 k=n3,n1
               pm3(k3+k)=pm1(k1+k)
 32         continue
            n3=n1
            d3(i+1+(j-1)*m)=d3(i+(j-1)*m)+n3
c
 38         k1=k1+n1
            k2=k2+n2
            k3=k3+n3
 40      continue
 41   continue
      return
      end
