      subroutine n1gc2b(n,simul,prosca,xinit,f,dg,alpha,d,
     /                  xfinal,gfinal,imp,io,retour,ntotap,nsim,
     /                  intfor,dx,eps,izs,rzs,dzs)
c
c     Copyright INRIA

      implicit double precision (a-h,o-z)
c     parametres
      double precision  zero      , deux     , trois
      parameter (       zero=0.0d+0 , deux=2.0d+0, trois=3.0d+0 )
      double precision  dixiem      , petit
      parameter (       dixiem=.10d+0 , petit=.00010d+0 )
      double precision  unplus       , unmoin      , envir1
      parameter  (      unplus=1.010d+0, unmoin=.990d+0, envir1=.90d+0 )
c declarations des tableaux
      double precision  xinit(n),d(n),xfinal(n),gfinal(n),dzs(*)
      real rzs(*)
      integer     izs(*)
c declarations des scalaires
      double precision f, finit, dg, alpha, eps, dx, ap, dp, fp,
     /                  aux1, aux2, pas, at, dal, bsup, delta
      integer  n, imp, io, retour, nsim, ntotap, nappel, indic, j
      logical  intfor, maxpas, rfinie, accept, encadr, depas
      external prosca, simul
c
c initialisations
      depas=.false.
      bsup=zero
      finit=f
      nappel=0
      ap=zero
      fp=finit
      dp=dg
      if (imp .gt. 3) write(io,1) alpha, dg
c calcul de la longueur du pas
      call prosca(n,d,d,pas,izs,rzs,dzs)
      pas=sqrt(pas)
c test d'erreur dans la recherche lineaire
1000  continue
      if (alpha * pas .le. dx) then
      if (imp .gt. 3) write(io,1001)
      retour=1
      return
      else if (ntotap .eq. nsim) then
      retour=3
      return
      else
      continue
      endif
c calcul du nouveau point susceptible d'etre xfinal
      do 2000 j=1,n
      xfinal(j)=xinit(j) + alpha * d(j)
2000  continue
c calculs de f et g en ce point
      indic=4
      call simul(indic,n,xfinal,f,gfinal,izs,rzs,dzs)
      nappel=nappel + 1
      ntotap=ntotap + 1
      if (indic .lt. 0) then
      depas=.true.
      if (imp . gt. 3) then
      write(io,2001) alpha,indic
      endif
      delta=alpha - ap
      if (delta .le. dx) then
      retour=4
      return
      else
      bsup=alpha
      alpha=delta * dixiem + ap
      goto 1000
      endif
      endif
c calcul de la derivee suivant d au point xfinal
      call prosca(n,d,gfinal,dal,izs,rzs,dzs)
c
      if (imp .gt. 3) then
      aux2=f - finit
      write(io,2002) alpha, aux2, dal
      endif
      if (indic .eq. 0) then
      retour=2
      return
      endif
      maxpas=(f .gt. finit) .and. (dal .lt. zero)
      if (maxpas) then
      alpha=alpha / trois
      ap=zero
      fp=finit
      dp=dg
      rfinie=.false.
c
      else
c test:le nouveau point est il acceptable
      aux1=finit + petit * alpha * dg
      aux2=abs(dal/dg)
      accept=(f .le. aux1) .and. (aux2 .le. envir1)
      if (accept) then
c doit on faire une interpolation
      rfinie=(nappel .gt. 1) .or. (.not. intfor) .or. (aux2 .le. eps)
      else
      rfinie=.false.
      endif
c
      if (.not. rfinie) then
c la recherche lineaire n'est pas finie. interpolation
      aux1=dp + dal- trois*(fp-f)/(ap-alpha)
      aux2=aux1 * aux1 - dp * dal
      if (aux2 .le. zero) then
      aux2=zero
      else
      aux2=sqrt(aux2)
      endif
      if (dal-dp+ deux * aux2 .eq. zero) then
      retour=4
      return
      endif
      at=alpha - (alpha-ap)*(dal+aux2-aux1)/(dal-dp+ deux * aux2)
c test:le minimum a t-il ete encadre
      encadr=(dal/dp) .le. zero
      if (encadr) then
c le minimum a ete encadre
      if (abs(alpha - ap) .le. dx) then
      retour=4
      return
      endif
      aux1=unplus * min(alpha,ap)
      aux2=unmoin * max(alpha,ap)
      if ((at .lt. aux1) .or. (at .gt. aux2)) at=(alpha + ap)/deux
      else
c le minimum n'a pas ete encadre
      aux1=unmoin * min(ap,alpha)
      if ((dal .le. zero) .or. (at .le. zero) .or. (at .ge. aux1)) then
      aux1=unplus * max(ap,alpha)
      if ((dal .gt. zero) .or. (at .le. aux1))  then
      if (dal .le. zero) then
      at=deux * max(ap,alpha)
      else
      at=min(ap,alpha) / deux
      endif
      endif
      endif
      endif
      if ( (depas) .and. (at .ge. bsup)) then
      delta=bsup - alpha
      if (delta .le. dx) then
      retour=4
      return
      else
      at=alpha + delta * dixiem
      endif
      endif
      ap=alpha
      fp=f
      dp=dal
      alpha=at
      endif
      endif
      if (rfinie) then
      retour=0
      return
      else
      goto 1000
      endif
1     format(7h n1gc2b,6x,5h  pas,d10.3,5h  dg=,d9.2)
1001  format(21h n1gc2b    fin sur dx)
2001  format(7h n1gc2b,20x,d10.3,8h  indic=,i3)
2002  format(7h n1gc2b,20x,d10.3,2d11.3)
      end
