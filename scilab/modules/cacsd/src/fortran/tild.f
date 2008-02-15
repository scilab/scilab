

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
