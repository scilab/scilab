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


