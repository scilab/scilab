

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

C/MEMBR ADD NAME=TILD,SSI=0

      subroutine tild(n,tp,tpti)
c!but
c     pour un polynome p(z)  l'operation tild aboutit a un polynome
c     ptild(z) defini par la relation suivante :
c       ptild(z)= z**n * p(1/z) .
c!liste d'appel
c     Entree : - tp . vecteur des coefficients du polynome a "tilder" .
c              -  n . degre du polynome "tp"
c
c     Sortie : - tpti . vecteur des coefficients du polynome resultant .
c
c!
      implicit double precision (a-h,o-y)
      dimension tp(0:*),tpti(0:*)
c
      do 50 j=0,n
         tpti(j)=tp(n-j)
 50   continue
      return
      end
