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

      subroutine jacl2(neq,t,tq,ml,mu,pd,nrowpd)
c!but
c     jacl2 cree la matrice  jacobienne necessaire a Lsoda,
c     qui correspond en fait a la hessienne du probleme
c     d'approximation L2.
c!liste d'appel
c     entree :
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome q
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de 
c               fourier dans tq est neq(3)+2
c         neq(4:(nq+1)*(nq+2)) tableau de travail entier
c     - t est une variable parametrique necessaire a Lsoda.
c     - tq. tableau reel de taille au moins
c               7+dgmax+5*nq+6*ng+nq*ng+nq**2*(ng+1)
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         5+5*nq+5*ng+nq*ng+nq**2*(ng+1)
c     - ml et mu sont les parametres du stockage par bande
c        de la matrice qui n a pas lieu ici ,ils donc ignores.
c
c     sortie :
c     - pd. est le tableau ou l on range la matrice pleine
c       dont les elements sont etablis par la sub. Hessien
c     - nrowpd. est le nombre de ligne du tableau pd
c!

      implicit double precision (a-h,o-y)
      dimension tq(*),pd(nrowpd,*)
      dimension neq(*)
      
c
      call hessl2(neq,tq,pd,nrowpd)
      nq=neq(1)
c      write(6,'(''jac='')')
c      do 10 i=1,nq
c         write(6,'(5(e10.3,2x))') (pd(i,j),j=1,nq)
c 10   continue
c
      return
      end
      subroutine jacl2n(neq,t,tq,ml,mu,pd,nrowpd)
c!but
c     jacl2 cree la matrice  jacobienne necessaire a Lsoda,
c     qui correspond en fait a la hessienne du probleme
c     d'approximation L2.
c!liste d'appel
c     entree :
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome q
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de 
c               fourier dans tq est neq(3)+2
c         neq(4:(nq+1)*(nq+2)) tableau de travail entier
c     - t est une variable parametrique necessaire a Lsoda.
c     - tq. tableau reel de taille au moins
c               7+dgmax+5*nq+6*ng+nq*ng+nq**2*(ng+1)
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         5+5*nq+5*ng+nq*ng+nq**2*(ng+1)
c     - ml et mu sont les parametres du stockage par bande
c        de la matrice qui n a pas lieu ici ,ils donc ignores.
c
c     sortie :
c     - pd. est le tableau ou l on range la matrice pleine
c       dont les elements sont etablis par la sub. Hessien
c     - nrowpd. est le nombre de ligne du tableau pd
c!
      implicit double precision (a-h,o-y)
      dimension tq(*),pd(nrowpd,*)
      dimension neq(*)
      
c
      call hessl2(neq,tq,pd,nrowpd)
      nq=neq(1)
      do 20 i=1,nq
         do 10 j=1,nq
            pd(i,j)=-pd(i,j)
 10      continue
 20   continue
c      write(6,'(''jac='')')
c      do 10 i=1,nq
c         write(6,'(5(e10.3,2x))') (pd(i,j),j=1,nq)
c 10   continue
c
      return
      end
