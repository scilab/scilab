      subroutine front(nq,tq,nbout,w)
C!but
C     cette routine calcule le nombre de racines  du polynome q(z) qui
C     sont situees a l'exterieur du cercle unite .
C!liste d'appel
C     subroutine front(nq,tq,nbout,w)
C     dimension tq(0:*),w(*)
C     Entree :
C     - nq . est le degre du polynome q(z)
C     - tq   . le tableau du polynome en question
C
C     Sortie :
C     -nbout . est le nombre de racine a l'exterieur du  du cercle unite
C     tableau de travail
C     -w 3*nq+1
C!
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension tq(nq+1), w(*)
C
      integer fail
C
      lpol = 1
      lzr = lpol + nq + 1
      lzi = lzr + nq
      lzmod = lpol
      lfree = lzi + nq
C
      call dcopy(nq+1,tq,1,w(lpol),-1)
      call rpoly(w(lpol),nq,w(lzr),w(lzi),fail)
      call modul(nq,w(lzr),w(lzi),w(lzmod))
C
      nbout = 0
      nbon = 0
      do 110 i = 1,nq
        if (w(lzmod-1+i) .gt. 1.0d+0) then
          nbout = nbout + 1
        endif
        if (w(lzmod-1+i) .eq. 1.0d+0) then
          nbon = nbon + 1
        endif
 110  continue
C
      return
      end

