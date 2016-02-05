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

      subroutine watfac(nq,tq,nface,newrap,w)
c!but
c     Cette procedure est charge de determiner quelle est
c     la face franchie par la trajectoire du gradient.
c!liste d'appel
c     subroutine watfac(nq,tq,nface,newrap,w)
c     dimension tq(0:nq),w(3*nq+1)
c
c     Entrees :
c     - nq. est toujours le degre du polynome q(z)
c     - tq. est le tableau des coefficients de ce polynome.
c
c     Sortie  :
c     - nface contient l indice de la face que le chemin
c       de la recherche a traverse.
c       Les valeurs possibles de nface sont: 0 pour la face
c       complexe, 1 pour la face 'z+1' et -1 pour la face  'z-1'.
c     - newrap est un parametre indiquant s'il est necessaire
c       ou pas d'effectuer un nouveau un rapprochement.
c
c     Tableaux de travail
c     - w : 3*nq+1
c!

      implicit double precision (a-h,o-z)
      dimension tq(nq+1),w(*)
      logical fail
c
      lpol=1
      lzr=lpol+nq+1
      lzi=lzr+nq
      lzmod=lpol
      lfree=lzi+nq
c
      call dcopy(nq+1,tq,1,w(lpol),-1)
      call rpoly(w(lpol),nq,w(lzr),w(lzi),fail)
      call modul(nq,w(lzr),w(lzi),w(lzmod))
c
      nmod1=0
      do 110 j=1,nq
         if (w(lzmod-1+j).ge.1.0d+0) then
            nmod1=nmod1+1
            if(nmod1.eq.1) indi=j
         endif
 110  continue
c
      if (nmod1.eq.2) then
         if(w(lzi-1+indi).eq.0.0d+0) then
            newrap=1
            return
         else
            nface=0
         endif
      endif
c
      if (nmod1.eq.1) then
         if (w(lzr-1+indi).gt.0.0d+0) then
            nface=-1
         else
            nface=1
         endif
      endif
c
      newrap=0
c
      return
      end
