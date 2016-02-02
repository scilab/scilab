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
