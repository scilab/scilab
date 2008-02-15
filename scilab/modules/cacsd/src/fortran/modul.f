

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

      implicit double precision (a-h,o-z)
      dimension zeror(*),zeroi(*),zmod(*)
c
      do 50 i=1,neq+1
         zmod(i)= sqrt( zeror(i)**2 + zeroi(i)**2 )
 50   continue
c
      return
      end
