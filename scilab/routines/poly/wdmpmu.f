C/MEMBR ADD NAME=WDMPMU,SSI=0
c     Copyright INRIA
      subroutine wdmpmu(mp1r,mp1i,d1,nl1,mp2r,d2,nl2,
     & mp3r,mp3i,d3,l,m,n)
c!purpose
c  ce sous programme effectue le calcul du produit de matrices
c de polynomes a coefficients complexes
c
c           mp3 = mp1 * mp2
c
c! calling sequence
c
c     pm1 : tableau contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*nl1 + k) )
c           pm1 doit etre de taille au moins d1(nl1*n+1)-d1(1)
c     d1 : tableau entier de taille nl1*n+1,  si k=i+(j-1)*nl1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     nl1 : entier definissant le rangement dans d1
c
c     pm2,d2,nl2 : definitions similaires a celles de pm1,d1,nl1
c     pm3,d3 : definitions similaires a celles de pm1 et d1, nl3 est
c              suppose egal a l
c     l : nombre de lignes de pm1
c     m : nombre de ligne des matrices pm
c     n : nombre de colonnes des matrices pm
c avec les conventions suivantes:
c
c     -si l =  0 signifie  que  mp1 est un polynome, et que la
c multiplication s'entend  alors  au sens de la multiplication
c de tous les coefficients de mp2 par mp1.
c
c     -si n =  0 signifie que mp2 est un polynome, et  que  la
c multiplication s'entend au sens de la multiplication de tous
c les coefficients de mp1 par mp2.
c
c     -si m =  0 la multiplication s'entend element par element,
c il est alors suppose que mp1 et mp2, donc mp3 sont de dimension
c l x n.
c
c!
c auteur: c. klimann, inria
c date: 25 fevrier 1985.
c
c
c&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
c
c
      double precision mp1r(*),mp1i(*),mp2r(*),mp3r(*),mp3i(*)
      integer d1(*), d2(*), d3(*)
      integer l, m, n
      integer nl1, nl2
      integer i, j, k, k1, k2, k3
      integer p1, p2, p3
c
c
      d3(1)= 1
      if (l .eq. 0 .or. m .eq. 0 .or. n .eq. 0) goto 500
c
      p2 = -nl2
      p3 = -l
      do 10 j = 1, n
      p2 = p2 + nl2
      p3 = p3 + l
         do 11 i = 1, l
            mp3r(d3(p3+i)) = 0.0d+0
            mp3i(d3(p3+i)) = 0.0d+0
            k3 = 0
            p1 = i - nl1
            do 12 k = 1, m
               p1 = p1 + nl1
               k2 = d2(p2+k+1) - d2(p2+k) - 1
               k1 = d1(p1 + 1) - d1(p1) - 1
               kk=k3
               call dpmul(mp1r(d1(p1)),k1,mp2r(d2(p2+k)),k2,
     &         mp3r(d3(p3+i)),kk)
               call dpmul(mp1i(d1(p1)),k1,mp2r(d2(p2+k)),k2,
     &         mp3i(d3(p3+i)),k3)
   12       continue
            d3(p3 + i + 1) = d3(p3 + i) + k3 + 1
   11 continue
   10 continue
      return
  500 if (l .eq. 0) goto 600
      if (m .eq. 0) goto 700
      p1 = -nl1
      p3 = -l
      k2 = d2(2) - d2(1) - 1
      do 510 j = 1, m
      p1 = p1 + nl1
      p3 = p3 + l
         do 510 i = 1, l
            k3 = 0
            k1 = d1(p1 + i + 1) - d1(p1 + i) - 1
            mp3r(d3(p3 + i)) = 0.0d+0
            kk=k3
            call dpmul(mp1r(d1(p1+i)),k1,mp2r(1),k2,mp3r(d3(p3+i)),kk)
            mp3i(d3(p3 + i)) = 0.0d+0
            call dpmul(mp1i(d1(p1+i)),k1,mp2r(1),k2,mp3i(d3(p3+i)),k3)
            d3(p3 + i + 1) = d3(p3 + i) + k3 + 1
510   continue
      return
600   k1 = d1(2) - d1(1) - 1
      p2 = -nl2
      p3 = -m
      do 610 j = 1, n
      p2 = p2 + nl2
      p3 = p3 + m
         do 610  i = 1, m
            k3 = 0
            k2 = d2(p2 + i + 1) - d2(p2 + i) - 1
            mp3r(d3(p3+i)) = 0.0d+0
            kk=k3
            call dpmul(mp1r(1),k1,mp2r(d2(p2+i)),k2,mp3r(d3(p3+i)),kk)
            mp3i(d3(p3+i)) = 0.0d+0
            call dpmul(mp1i(1),k1,mp2r(d2(p2+i)),k2,mp3i(d3(p3+i)),k3)
            d3(p3 + i + 1) = d3(p3 + i) + k3 + 1
610   continue
      return
  700 p1 = -nl1
      p2 = -nl2
      p3 = -l
      do 710 j = 1, n
         p1 = p1 + nl1
         p2 = p2 + nl2
         p3 = p3 + l
         do 710 i = 1, l
            k1 = d1(p1 + i + 1) - d1(p1 + i) - 1
            k2 = d2(p2 + i + 1) - d2(p2 + i) - 1
            mp3r(d3(p3+i)) = 0.0d+0
            k3 = 0
            call dpmul(mp1r(d1(p1+i)),k1,mp2r(d2(p2+i)),k2,
     1                 mp3r(d3(p3+i)),k3)
            mp3i(d3(p3+i)) = 0.0d+0
            k3 = 0
            call dpmul(mp1i(d1(p1+i)),k1,mp2r(d2(p2+i)),k2,
     1                 mp3i(d3(p3+i)),k3)
            d3(p3 + i + 1) = d3(p3 + i) + k3  + 1
710   continue
      return
      end
