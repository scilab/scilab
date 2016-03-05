c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
C/MEMBR ADD NAME=FRANCK,SSI=0
c
      subroutine franck(a,na,n,job)
c!but
c     cette subroutine genere la matrice de franck d'ordre n
c     definie par : a(i,j)=j si i.le.j , a(j,j-1)=j , a(i,j)=0
c     si i.gt.j+1 . ou son inverse
c!liste d'appel
c     subroutine franck(a,na,n,job)
c
c     double precision a(na,n)
c     integer na,n,job
c
c     a :tableau contenant apres execution la matrice
c     na:nombre de ligne du tableau a
c     n : dimension de la matrice
c     job : entier caracterisant le resultat demande
c           job = 0 : matrice de franck
c           job = 1 : son inverse
c!sous programme appeles
c     dble real (fortran)
c!
      double precision a(na,n)
c variables internes
      integer n1,k,l,ls
      double precision x,dble
c
      if(job.eq.1) goto 50
c
      a(1,1)=dble(real(n))
      if(n.eq.1) return
      do 20 k=2,n
      x=dble(real(n+1-k))
      a(k,k-1)=x
      do 10 l=1,k
      a(l,k)=x
   10 continue
   20 continue
      if(n.eq.2) return
      do 40 l=3,n
      n1=l-2
      do 40 k=1,n1
      a(l,k)=0.0d+0
   40 continue
      return
c
   50 continue
      if(n.eq.1) return
      n1=n-1
      do 60 k=1,n1
      a(k,k+1)=-1.0d+0
      a(k+1,k+1)=dble(real(n+1-k))
   60 continue
      a(1,1)=1.0d+0
      do 66 ksd=1,n1
      ls=n-ksd
      do 65 l=1,ls
      klig=n+1-l
      kcol=klig-ksd
      a(klig,kcol)=-a(klig-1,kcol)*l
   65 continue
   66 continue
c
      if(n.lt.3) return
      do 70 kcol=3,n
      n1=kcol-2
      do 70 klig=1,n1
      a(klig,kcol)=0.0d+0
   70 continue
      return
      end
