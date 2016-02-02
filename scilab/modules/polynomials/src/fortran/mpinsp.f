c     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c     Copyright (C) 1985-2008 - Carlos KLIMANN
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine mpinsp(dep1,lig1,col1,v1,d1,v2,d2,dep2,
     &     lig2,col2,depr,ligr,colr,defsz,iwork,ierr)
c     !but
c
c     Cette subroutine pretraite l'insertion  d'une
c     matrice de polynomes mat2 dans une autre mat1 d'apres deux
c     vecteurs.Pour calculer le  volume qu'occupera le resultat
c
c     !parametres d'appel
c
c     call mpinsp(dep1,lig1,col1,v1,d1,v2,d2,dep2,lig2,col2
c     & ,depr,ligr,colr,defsz,iwork,ierr)
c
c     ou
c
c     dep1: matrice entiere qui donne les deplacements relatifs des
c     elements de mat1
c
c     lig1, col1: entiers, dimensions de mat1
c
c     v1: vecteur entier
c
c     d1: longueur du vecteur v1
c
c     v2, d2: analogues aux anterieurs
c
c     dep2, lig2, col2: analogues aux correspondents 1
c
c     depr: contient les information issues du pretraitement:
c     depr(1) contient le volume des coeff de la matrice resultat
c     depr(1+i) contient un pointeur vers dep1 si positif
c     vers dep2 si negatif
c     vers 0 si nul
c
c     ligr, colr: entiers dimensions de la matrice de sortie
c     depr. S'ils ne sont pas connus au prealable
c     peuvent etre calcules par la subroutine dimin.
c     defsz: entier: taille de l'element par default
c     iwork: tableau entier de taille colr+ligr
c     ierr: si 0 terminaison correcte,
c     sinon les dimensions de mat2 ne sont pas compatibles
c
c
c     attention!: aucune de matrices dep1, dep2 ou depr ne
c     doit coincider. Dans le cas contraire, les resultats seraient
c     imprevisibles.
c

c
c
c     !
      integer dep1(*),v1(*),v2(*),dep2(*),depr(*),iwork(*)
      integer lig1,col1,d1,d2,lig2,col2,ligr,colr,ierr,defsz
c
      integer volr
c
      ierr=0
      volr=0
      if (d1.eq.0.or.d2.eq.0) return

      if(d1.gt.0.or.d2.gt.0) goto 10
c
c     cas  (:,:)
c
      if(lig1.ne.lig2.or.col1.ne.col2) then
         if(lig2*col2.eq.1) then
            ir=lig1*col1+1
            do 04 i=1,ir
               depr(i+1)=-1
 04         continue
            volr=(dep2(2)-dep2(1))*lig1*col1
            goto 999
         else
            ierr=1
            return
         endif
      endif
c
      ir=lig1*col1+1
      do 05 i=1,ir
         depr(i+1)=-i
 05   continue
      volr=dep2(ir)-dep2(1)
      goto 999
c
 10   if(d1.lt.0) then
c     cas (:,.)
         inc2=1
         if(max(1,lig1).ne.lig2) then
            if(lig2.eq.1.and.col2.eq.1) then
               inc2=0
            else
               ierr=1
               return
            endif
         endif
c
c     toutes les lignes pour un choix de colonnes
c
         kr=1
         volr=0
c     trouver les colonnes qui sont modifiées
         call iset(colr,0,iwork,1)
         do i=1,d2
            iwork(v2(i))=i
         enddo
         do 19 jr=1,colr
c     la colonne jr est elle a modifier ?
            id2=iwork(jr)
            if(id2.eq.0) goto 14
c     oui
            if(inc2.eq.1) then
               k2=lig2*(id2-1)
               do 12 ir=1,ligr
                  kr=kr+1
                  depr(kr)=-1
 12            continue
               volr=volr+ligr*(dep2(2)-dep2(1))
            else
               do 13 ir=1,ligr
                  kr=kr+1
                  depr(kr)=-(k2+ir)
 13            continue
               volr=volr+dep2(k2+ligr+1)-dep2(k2+1)
            endif
            go to 19
c     non
 14         if(jr.le.col1) then
c     la colonne designee existe dans mat1
               k1=(jr-1)*lig1
               do 17 ir=1,ligr
                  kr=kr+1
                  depr(kr)=(k1+ir)
 17            continue
               volr=volr+dep1(k1+ligr+1)-dep1(k1+1)
               goto 19
            endif
c     si non, inserer un string vide
            do 18 ir=1,ligr
               kr=kr+1
               depr(kr)=0
 18         continue
            volr=volr+ligr*defsz
 19      continue
         goto 999
      endif
c
      if(d2.lt.0) then
c     cas (.,:)
         inc2=1
         if(col1.ne.max(1,col2)) then
            if(lig2.eq.1.and.col2.eq.1) then
               inc2=0
            else
               ierr=1
               return
            endif
         endif
c
c     toutes les colonnes pour un choix de lignes
c
         call iset(ligr,0,iwork,1)
         do i=1,d1
            iwork(v1(i))=i
         enddo

         do 29 ir=1,ligr
            kr=ir+1-ligr
c     la ligne ir est elle a modifier ?
            id1=iwork(ir)
            if(id1.eq.0) goto 24
c     oui
            if(inc2.eq.1) then
               k2=id1-lig2
               do 22 jr=1,colr
                  kr=kr+ligr
                  k2=k2+lig2
                  depr(kr)=-k2
                  volr=volr+dep2(k2+1)-dep2(k2)
 22            continue
            else
               do 23 jr=1,colr
                  kr=kr+ligr
                  depr(kr)=-1
                  volr=volr+colr*(dep2(2)-dep2(1))
 23            continue
            endif
            go to 29
c     non
 24         if(ir.le.lig1) then
c     la ligne designee existe dans mat1
               k1=ir-lig1
               do 27 jr=1,colr
                  kr=kr+ligr
                  k1=k1+lig1
                  volr=volr+dep1(k1+1)-dep1(k1)
                  depr(kr)=k1
 27            continue
               goto 29
            endif
c     si non, inserer des zeros
            do 28 jr=1,colr
               kr=kr+ligr
               depr(kr)=0
 28         continue
            volr=volr+colr*defsz
 29      continue
         goto 999
      endif
c
c     cas general
c
      kr=2
      inc2=1
      if(lig2.eq.1.and.col2.eq.1) inc2=0

      call iset(colr+ligr,0,iwork,1)
      do i=1,d2
         iwork(v2(i))=i
      enddo

      do i=1,d1
         iwork(colr+v1(i))=i
      enddo

      do 40 jr=1,colr
c     la colonne jr est elle a modifier ?
         id2=iwork(jr)
         if(id2.eq.0) goto 35
c
         do 34 ir=1,ligr
c     la ligne ir est-elle a modifier
            id1=iwork(colr+ir)
            if(id1.eq.0) goto 32
c
            if(inc2.eq.1) then
               k2=id1+lig2*(id2-1)
               depr(kr)=-k2
               volr=volr+dep2(k2+1)-dep2(k2)
            else
               depr(kr)=-1
               volr=volr+dep2(2)-dep2(1)
            endif
            kr=kr+1
            go to 34
c
 32         if(ir.gt.lig1.or.jr.gt.col1) goto 33
            k1=ir+lig1*(jr-1)
            depr(kr)=k1
            kr=kr+1
            volr=volr+dep1(k1+1)-dep1(k1)
            goto 34
c
 33         depr(kr)=0
            kr=kr+1
            volr=volr+defsz
 34      continue
         goto 40
c     non
c     toutes les lignes de la colonne designee
 35      if(jr.gt.col1) goto 38
         k1=(jr-1)*lig1
         do 36 ir=1,lig1
            depr(kr)=k1+ir
            kr=kr+1
 36      continue
         volr=volr+dep1(k1+lig1+1)-dep1(k1+1)
         if(lig1.ge.ligr) goto 40
         do 37 ir=lig1+1,ligr
            depr(kr)=0
            kr=kr+1
 37      continue
         volr=volr+(ligr-lig1)*defsz
         goto 40
 38      do 39 ir=1,ligr
            depr(kr)=0
            kr=kr+1
 39      continue
         volr=volr+ligr*defsz
 40   continue
c
 999  depr(1)=volr
      return
      end
