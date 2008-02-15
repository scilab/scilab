

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      double precision function phi(tq,nq,tg,ng,w)
C%but
C calcule la fonction phi
c%liste d'appel
c     Entree :
c        tg . tableau des coefficients de la fonction g .
c        ng . degre du polynome g
c        tq . tableau des coefficients du polynome q
c        nq . degre du polynome q
c        w  . tableau de travail de taille nq+ng+1
c     Sortie :
c        phi
c%

      implicit double precision (a-h,o-y)

      dimension tq(nq+1),tg(ng+1),w(nq+ng+1)
c
      ltr=1
      lfree=ltr+nq+ng+1
      call lq(nq,tq,w(ltr),tg,ng)
C
      ltlq=ltr
      call calsca(nq,tq,w(ltlq),y0,tg,ng)
C
      phi = 1.0d+0 - y0
C
      return
      end


