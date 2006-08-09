      subroutine dimin(lig1,col1,v1,d1,v2,d2,lig2,col2,ligr,colr,ierr)
c!but
c
c     Cette subroutine donne les dimensions (lignes, colonnes) de
c     la  matrice  qui resulte de l'insertion  d'une
c     matrice  de polynomes mat2 (ou strings) dans une autre mat1
c     d'apres deux vecteurs donnes.
c
c!parametres d'appel
c
c     call dimin(lig1,col1,v1,d1,v2,d2,lig2,col2,ligr,colr,ierr)
c
c     ou
c
c     lig1, col1: entiers, dimensions de mat1
c
c     v1: vecteur entier
c
c     d1: longueur du vecteur v1, s'il est positif doit etre egal
c         a lig2.
c
c     v2, d2: analogues aux anterieurs, si d2 est positif doit etre
c             egal a col2.
c
c     Note: si d1 (ou d2 ou les deux sont negatifs, ca signifie
c           qu'on prendra toutes les colonnes (ou lignes) de
c           mat1.
c
c     lig2, col2: analogues aux correspondents 1
c
c     ligr, colr: parametres de sortie, matrice r et
c     autres valeurs relationnes.
c
c     ierr: si 0 terminaison correcte,
c           si 1 un des vecteurs v1 ou v2 a une longueur nulle,
c           si 2 mauvaises dimensions de mat2
c
c!auteur:
c     carlos klimann, inria, 26-XI-85.
c
c
c!
c     Copyright INRIA
      integer v1(*),v2(*)
      integer lig1,col1,d1,d2,lig2,col2,ligr,colr,ierr
      integer noo1,noo2
c
c     ========================================
c
c     cas ou l'un des deux vecteurs v1 ou v2 est
c     de dimension nulle
c
c     ========================================
c
      if (d1.eq.0.or.d2.eq.0)then
c     erreur pour un des vecteurs de dimensions nulles
      ierr=1
      return
      endif
c
c     ========================================
c
c     cas ou d1 ou d2 sont negatifs
c
c     ========================================
c
      if(d1.gt.0.and.d2.gt.0)goto 05
      if(d1.lt.0.and.d2.lt.0) then
      if(lig1.ne.lig2.or.col1.ne.col2) then
c     erreur pour mauvaises dimensions de mat2
      ierr=2
      return
      endif
      ligr=lig1
      colr=col1
      goto 999
      endif
c
c     ========================================
c
c     cas du premier vecteur a dimension negative
c
c     ========================================
c
      if(d1.lt.0) then
c     determination de noo2
      noo2=0
      do 06 i=1,d2
      if(v2(i).gt.noo2)noo2=v2(i)
06    continue
c     dimensions de r
      ligr=max(lig1,1)
      colr=max(col1,noo2)
      go to 999
      endif
c
c     ========================================
c
c     cas ou les dimensions du deuxieme vecteur sont negatives
c
c     ========================================
c
      if(d2.lt.0) then
c     determination de noo1
      noo1=0
      do 09 i=1,d1
      if(v1(i).gt.noo1)noo1=v1(i)
09    continue
c     dimensions de r
      ligr=max(lig1,noo1)
      colr=max(col1,1)
      goto 999
      endif
c
c     ========================================
c
c     cas general - ou les dimensions de deux vecteurs sont positives
c
c     ========================================
c
c     determination des dimensions correctes
05    if(d1.ne.lig2.or.d2.ne.col2)then
      ierr=2
      return
      endif
c     determination de noo1 et noo2
      noo1=0
      do 01 i=1,d1
      if(v1(i).gt.noo1)noo1=v1(i)
01    continue
      noo2=0
      do 02 i=1,d2
      if(v2(i).gt.noo2)noo2=v2(i)
02    continue
c     dimensions de r
      ligr=max(lig1,noo1)
      colr=max(col1,noo2)
999   ierr=0
      return
      end
