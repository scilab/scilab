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
c
      subroutine n1gc2a(simul,prosca,n,x,f,g,dx,df1,eps,imp,io,
     /                  niter,nsim,info,memh,d,xx,gg,tabaux,h,
     /                  izs,rzs,dzs)
      implicit double precision (a-h,o-z)
c
c parametres
      double precision  zero     , un     , deux     , ro
      parameter  (  zero=0.0d+0, un=1.0d+0, deux=2.0d+0, ro=0.20d+0 )
c declaration des tableaux
      double precision    x(n),g(n),d(n),xx(n),gg(n),tabaux(n),h(*),
     /                    dzs(*)
      real rzs(*)
      integer  izs(*)
c declaration des scalaires
      double precision  f, dx, eps, df1
      double precision   dg1, dg, alpha, normg0, aux1, aux2, mu, eta,
     / omega, normg, gcarre, ggcarr, nu, sigma, sscalg, uscalg,
     / sscaek
      integer  n, memh, imp, io, nsim, niter, info
      integer  memuti, nrzuti, memsup, m, retour, iter,
     /   ntotap, nmisaj, i, iu, is, ieta, inu, j, kj, k, kp1
      logical  gc, iterqn, intfor, redfor, redem, termi
      character bufstr*(4096)
c
      external  simul, prosca
c
c  *************************************************************
c  phase i:determination de la methode ( et de m le cas echeant)
c  *************************************************************
c
      memuti=n*(n+1)/2
c
c memsup est aussi la dimension minimale de la matrice h
      memsup=2*n+2
c
      if (memh .ge. memuti) then
      gc=.false.
      nrzuti=memuti+4*n
      if (imp .gt. 1) then 
      write(bufstr,1) nrzuti
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      endif
      else if (memh .lt. memsup) then
      info=3
      return
      else
      gc=.true.
c m est le nombre de mises a jour admissible
      m=memh / memsup
c memuti est ici le nombre de places memoire utilisees pour stocker h
      memuti=m * memsup
      nrzuti=memuti+4*n
      if (imp .gt. 1) then
        write(bufstr,2) m
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        write(bufstr,3) nrzuti
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
      endif
1     format(40h     methode de quasi-newton. nrz utile=,i7)
2     format(38h     methode du gradient conjugue avec,i3)
3     format(14h mises a jour.,11h nrz utile=,i7)
c
c  ***********************************************
c  phase ii:initialisations propres a l'optimiseur
c  ***********************************************
c
c initialisation des compteurs
      iter=0
      ntotap=1
c
c  ******************************************************************
c  phase iii:demarrage a partir de x(0) avec descente suivant -(grad)
c  ******************************************************************
c
3000  i=0
      nmisaj=0
c
c calcul de la direction de descente
      do 3100 j=1,n
      d(j)=-g(j)
3100  continue
c
      call prosca(n,g,d,dg1,izs,rzs,dzs)
      normg0=sqrt(abs(dg1))
      if (iter .eq. 1) then
      omega=eps * normg0
      endif
c
c ************************************************************
c phase iv:debut de l'iteration x(i-1) donne x(i) le long de d
c ************************************************************
c
4000  if (iter .eq. niter) then
      info=4
      goto 99999
      endif
      iter=iter + 1
      i=i+1
c
c determination du type de pas
      if (gc) then
      iterqn=(i .le. m) .and. (2 .le. i)
      endif
c
c  *******************************
c  phase v:initialisation de alpha
c  *******************************
c
      if (iter .eq. 2) then
      alpha=deux * df1 /(-dg1)
      else if (gc) then
                   if (i.eq.1) then
                   alpha=un / normg0
                   else
                            if (iterqn) then
                            alpha=un
                            else
                            alpha=alpha * dg / dg1
                            endif
                   endif
      else
      alpha=un
      endif
c
c ***************************
c phase vi:recherche lineaire
c ***************************
c
      dg=dg1
      intfor=( gc .and. (.not.iterqn)).or. ((.not.gc) .and.(i.eq.1))
      do 6000 j=1,n
      xx(j)=x(j)
      gg(j)=g(j)
6000  continue
      call n1gc2b(n,simul,prosca,xx,f,dg,alpha,d,x,g,imp,io,retour,
     /           ntotap,nsim,intfor,dx,eps,izs,rzs,dzs)
c
      if (imp .gt. 3) then
      write(bufstr,6003)
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      endif
      if ((retour .eq. 4).or.((retour .eq. 1).and.(i .eq. 1))) then
      info=6
      return
      else if (retour .eq. 1) then
      if (imp .gt. 1) then
        write(bufstr,6002) iter,ntotap
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
      goto 3000
      else
c calcul de (g,g)
      if((i .gt. 1) .and. gc) ggcarr=gcarre
      call prosca(n,g,g,gcarre,izs,rzs,dzs)
      normg=sqrt(gcarre)
      if (imp .gt. 2) then
        write(bufstr,6001)iter,ntotap,f
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
      if (retour .eq. 2) then
      info=0
      goto 99999
      else if (retour .eq. 3)then
      info=5
      goto 99999
      endif
      endif
6001  format(4x,6h n1gc2,3x,i4,6h iters,3x,i4,7h simuls,3x,2hf=,d16.9)
6002  format(4x,6h n1gc2,3x,i4,6h iters,3x,i4,7h simuls,
     / 33h necessite d'un redemarrage total)
6003  format()
c
c  ******************************************************
c  phase vii:test d'arret par obtention de la convergence
c  ******************************************************
c
      termi=normg .lt. omega
      if (termi) then
      info=1
      goto 99999
      else
      continue
      endif
c
c *******************************************
c  phase viii:test x(i) point de redemarrage?
c *******************************************
c
c doit on forcer un redemarrage?
      redfor=gc .and. ((i .eq. 1) .or. (i .eq. m+n))
      if (redfor) then
      redem=.true.
      else if (gc .and. .not. iterqn) then
      call prosca(n,g,gg,aux1,izs,rzs,dzs)
      redem=abs(aux1) .gt. abs(ro * ggcarr)
      else
      redem=.false.
      endif
c
c  ********************
c  phase ix:mise a jour
c  ********************
c
c calcul de s stocke dans d et de y stocke dans xx
      do 9000 j=1,n
      d(j)=alpha * d(j)
      xx(j)=g(j)-gg(j)
9000  continue
      if (redem) then
c cas ou x(i) est un point de redemarrage
      i=1
      nmisaj=1
c sauvegarde de s qui est actuellement dans d
c                u=h*y=y
c                nu=(y,hy)=(y,y)
c                eta=(s,y)
c calcul des indices
      inu=1
      ieta=inu + 1
      iu=ieta
      is=iu + n
c
      do 9100 j=1,n
      h(iu +j)=xx(j)
      h(is +j)=d(j)
9100  continue
      call prosca(n,xx,xx,nu,izs,rzs,dzs)
      h(inu)=nu
      call prosca(n,d,xx,eta,izs,rzs,dzs)
      h(ieta)=eta
c h1 est maintenant definie
c calcul de h1*g que l'on range dans xx
      call fmulb1(n,h,g,xx,tabaux,nmisaj,prosca,izs,rzs,dzs)
c
      else if (gc) then
c cas de gc sans redamarrage
c calcul de h*y range dans gg
       call fmulb1(n,h,xx,gg,tabaux,nmisaj,prosca,izs,rzs,dzs)
c calculs de  nu, eta, sscalg, uscalg
      call prosca(n,xx,gg,nu,izs,rzs,dzs)
      call prosca(n,d,xx,eta,izs,rzs,dzs)
      call prosca(n,d,g,sscalg,izs,rzs,dzs)
      call prosca(n,gg,g,uscalg,izs,rzs,dzs)
c calcul de sigma et de mu
      sigma=(uscalg -(un + nu / eta)* sscalg) / eta
      mu=sscalg /eta
c calcul de h*g que l'on range dans xx
      call fmulb1(n,h,g,xx,tabaux,nmisaj,prosca,izs,rzs,dzs)
c calcul de la nouvelle direction de recherche:
c h*g - mu * u - sigma * s
      do 9200 j=1,n
      xx(j)= xx(j) - mu * gg(j) - sigma * d(j)
9200  continue
c
c cas d'une iteration de type quasi newton
      if (iterqn) then
      nmisaj=nmisaj + 1
c sauvegarde des termes utiles pour stocker la matrice mise a jour
      inu=inu + memsup
      ieta=inu + 1
      iu=ieta
      is=iu + n
      do 9300 j=1,n
      h(iu +j)=gg(j)
      h(is +j)=d(j)
9300  continue
      h(inu)=nu
      h(ieta)=eta
      endif
c cas de la methode quasi newton
      else
c calcul de eta=(s,y)
      call prosca(n,d,xx,eta,izs,rzs,dzs)
      if (i .eq. 1) then
c etape initiale calcul de l'approximation initiale de l'inverse de la
c matrice hessienne
c calcul de nu=(y,h0*y)=(y,y)
      call prosca(n,xx,xx,nu,izs,rzs,dzs)
c stockage de cette matrice h=(eta / nu) * i
      kj=1
      aux1=eta / nu
      do 9500 k=1,n
      h(kj)=aux1
      kj=kj +1
      kp1=k+1
      if (n .ge. kp1) then
      do 9400 j=kp1,n
      h(kj)=zero
      kj=kj +1
9400  continue
      endif
      gg(k)=aux1 * xx(k)
9500  continue
      nu=eta
      else
      call fmuls1(n,h,xx,gg)
      call prosca(n,xx,gg,nu,izs,rzs,dzs)
      endif
c calcul de la matrice mise a jour (utilisation de la formule bfgs )
c nu, eta et h*y (stocke dans gg) sont connus
      aux1=un + nu / eta
      kj=1
      do 9800 k=1,n
c calcul du vecteur contenant la keme colonne de h
      lk=k
      km1=k-1
      if (k .ge. 2) then
      do 9610 l=1,km1
      tabaux(l)=h(lk)
      lk=lk + (n-l)
9610  continue
      endif
      do 9620 l=k,n
      tabaux(l)=h(lk)
      lk=lk+1
9620  continue
c
      call prosca(n,xx,tabaux,aux2,izs,rzs,dzs)
      do 9630 l=1,n
      tabaux(l)=zero
9630  continue
      tabaux(k)=un
      call prosca(n,tabaux,d,sscaek,izs,rzs,dzs)
      kj=k-n
      do 9700 j=1,k
      kj=kj+n-j+1
      h(kj)=h(kj) - ( (aux2 - aux1*sscaek)*d(j) + sscaek*gg(j) )/eta
9700  continue
9800  continue
      endif
c
c  *****************************************************
c  phase x :calcul de la nouvelle direction de recherche
c  *****************************************************
c
      if (gc) then
c xx contient -d
      do 10000 j=1,n
      d(j)=-xx(j)
10000 continue
c
       else
c cas de la methode de quasi newton
c la nouvelle direction d egale -(h * g)
      call fmuls1(n,h,g,d)
      do 10100 j=1,n
      d(j)=-d(j)
10100 continue
      endif
c
c test:la direction de recherche est elle bien de descente
      call prosca(n,d,g,dg1,izs,rzs,dzs)
      if (dg1 .ge. zero) then
      info=7
      if (imp .gt. 1) then
        write(bufstr,10101) dg1
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
      goto 99999
      else
      goto 4000
      endif
c
c retour au programme appelant
99999 niter=iter
      nsim=ntotap
      if (i .eq. 0) then
      eps=normg0
      else
      eps=normg
      endif
10101 format(40h n1gc2a   erreur dans la hessienne   dg=,d9.2)
      end
