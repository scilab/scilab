C/MEMBR ADD NAME=DMPINS,SSI=0
c     Copyright INRIA
      subroutine dmpins(mat1,dep1,lig1,col1,mat2,
     & dep2,lig2,col2,matr,depr,ligr,colr)
c!but
c
c     Cette subroutine fait l'insertion (ampliation?) d'une matrice
c     de polynomes mat2 dans une autre mat1 d'apres deux vecteurs.
c     On suppose donnees depr, ligr et colr.
c     Le resultat viendra dans une troisieme matrice.
c
c!parametres d'appel
c
c     call dmpins(mat1,dep1,lig1,col1,mat2,dep2,lig2,
c    1            col2,matr,depr,ligr,colr)
c
c     ou
c
c     mat1: matrice double precision de polynomes
c
c     dep1: matrice entiere qui donne les deplacements relatifs des
c           elements de mat1
c
c     lig1, col1: entiers, dimensions de mat1
c
c     mat2 ,dep2, lig2, col2: analogues aux correspondents 1
c     (correspond a m dans l'example)
c
c     matr, depr, ligr, colr: parametres de sortie, matrice matr et
c     autres valeurs relationnes.
c
c
c     attention!: aucune de matrices mat1, mat2,matr,dep1, dep2 ou depr
c     doit coincider. Cas contraire, les resultats seraient
c     imprevisibles.
c
c!auteur:
c     carlos klimann, inria, 12-XI-85.
c
c
c!
      double precision mat1(*),mat2(*),matr(*)
      integer dep1(*),dep2(*),depr(*)
c
      integer lig1,col1,lig2,col2,ligr,colr
      depr(1)=1
      kr=1
c
      do 20 j=1,colr
      do 20 i=1,ligr
      kr=kr+1
      lr=depr(kr)
      if (lr .lt. 0) then 
         goto 11
      elseif (lr .eq. 0) then  
         goto 12
      else
         goto 13
      endif
   11 l2=-lr
      l=dep2(l2+1)-dep2(l2)
      call dcopy(l,mat2(dep2(l2)),1,matr(depr(kr-1)),1)
      depr(kr)=depr(kr-1)+l
      goto 20
   12 matr(depr(kr-1))=0.0d+0
      depr(kr)=depr(kr-1)+1
      goto 20
   13 l1=lr
      l=dep1(l1+1)-dep1(l1)
      call dcopy(l,mat1(dep1(l1)),1,matr(depr(kr-1)),1)
      depr(kr)=depr(kr-1)+l
c
   20 continue
      return
       end
