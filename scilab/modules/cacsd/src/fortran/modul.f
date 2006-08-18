      subroutine modul(neq,zeror,zeroi,zmod)
c!but
c     ce sous programme calcule le vecteur des modules d'un vecteur
c     de nombres complexes
c!liste d'appel
c     subroutine modul(neq,zeror,zeroi,zmod)
c     double precision zeror(neq),zeroi(neq),zmod(neq)
c
c     neq : longueur des vecteurs
c     zeror (zeroi) : vecteurs des parties reelles (imaginaires) du
c            vecteur de nombres complexes
c     zmod : vecteur des modules
c!
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension zeror(*),zeroi(*),zmod(*)
c
      do 50 i=1,neq+1
         zmod(i)= sqrt( zeror(i)**2 + zeroi(i)**2 )
 50   continue
c
      return
      end
