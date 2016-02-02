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

      subroutine lq(nq,tq,tr,tg,ng)
c!but
c     cette routine calcule a  partir de g(z) et q(z) le
c     polynome Lq(z) defini comme le reste , tilde , de la division
c     par q(z) du produit g(z) par le tilde de q(z) .
c!liste d'appel
c     Entree :
c        tg . tableau des coefficients de la fonction g .
c        ng . degre du polynome g
c        tq . tableau des coefficients du polynome q
c        nq . degre du polynome q
c     Sortie :
c        tr . tableau [tlq,tvq]
c             tlq =tr(1:nq) coefficients du polynome Lq 
c             tvq =tr(nq+1:nq+ng+1) coefficients du quotient vq de la 
c                    division par q du polynome gqti .
c!


      implicit double precision (a-h,o-z)
      dimension tq(nq+1),tr(nq+ng+1),tg(ng+1)
c
c     calcul de tg*tq~
      call tild (nq,tq,tr)
      call dpmul1(tg,ng,tr,nq,tr)
c
c     division euclidienne de tg*tq~ par tq
      call dpodiv(tr,tq,ng+nq,nq)
c
c     calcul du tilde du reste  sur place
      do 10 j=1,int(nq/2)
         temp=tr(j)
         tr(j)=tr(nq+1-j)
         tr(nq+1-j)=temp
 10   continue
c
      return 
      end
