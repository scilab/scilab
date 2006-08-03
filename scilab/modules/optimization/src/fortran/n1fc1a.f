      subroutine n1fc1a(simul,prosca,n,mode,xn,fn,g,df0,eps0,dx,imp,
     &                  zero,io,ntot,iter,nsim,memax,s,gd,x,sa,gg,al,
     &                  aps,anc,poids,q,jc,ic,r,a,e,rr,xga,y,w1,w2,izs,
     &                  rzs,dzs)
C ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     Copyright INRIA
C ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
C minimisation d'une fonction hemiderivable par une methode de faisceau.
C la direction est obtenue par projection de l'origine
C sur un polyedre genere par un ensemble de gradients deja calcules
C et la recherche lineaire donne un pas de descente ou un pas nul.
C l'algorithme minimise f a eps0 pres (si convexite)
C ou eps0 est une tolerance donnee par l'utilisateur.
C
C        mode
C                <=0 mode=indic de simul
C                1 fin normale
C                2 appel incoherent
C                3 reduire l'echelle des x
C                4 max iterations
C                5 max simulations
C                6 impossible d'aller au dela de dx
C                7 fprf2 mis en echec
C                8 on commence a boucler
C      imp
C                <0 indic=1 toutes les -imp iterations
C                0 pas d'impressions
C                1 impressions initiales et finales
C                2 impressions a chaque convergence
C                3 une impression par iteration
C                4 informations n1fc1 et nlis2
C                >4 debug
C                         5 tolerances diverses
C                         6 poids
C                         >6 fprf2
C         --------------------------------------------------
C fait appel aux subroutines suivantes:
C --------subroutine fprf2 (calcul de la direction)
C --------subroutines fremf2 et ffinf1 (esclaves de fprf2)
C --------subroutine frdf1 (reduction du faisceau)
C --------subroutine nlis2 (recherche lineaire)
C --------subroutine simul (module de simulation)
C --------subroutine prosca (produit de dualite donnant le gradient)
C cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
C cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      implicit double precision (a-h,o-z)
      external simul, prosca
      dimension xn(n), g(n), izs(*), dzs(*), x(n), gd(n), gg(n)
      dimension s(n), sa(n), jc(*), ic(*), r(*), a(*), e(*), rr(*),
     &          xga(*), y(*), w1(*), w2(*)
      dimension q(*), al(memax), aps(memax), anc(memax), poids(memax)
      real rzs(*)
      dimension i5(1), d3(1), d4(1)
C
C         initialisations
C
      itmax = iter
      iter = 0
      itimp = 0
      napmax = nsim
      nsim = 1
      logic = 1
      logic2 = 0
      tmax = 1.d20
      eps = df0
      epsm = eps
      df = df0
      mode = 1
      ntot = 0
      iflag = 0
C
C          initialisation du faisceau
C          calcul du diametre de l'epure et du test d'arret
C
      aps(1) = 0.d0
      anc(1) = 0.d0
      poids(1) = 0.d0
      nta = 0
      kgrad = 1
      memax1 = memax + 1
      do 50 i = 1,n
 50   q(i) = -g(i)
      call prosca(n,g,g,ps,izs,rzs,dzs)
      if (ps .gt. 0.d0) goto 60
      mode = 2
      if (imp .ne. 0) call n1fc1o(io,3,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      goto 900
 60   diam2 = 100. * df0 * df0 / ps
      eta2 = 1.d-2 * eps0 * eps0 / diam2
      ap = zero * df0 / diam2
      if (imp .gt. 2) call n1fc1o(io,4,i1,i2,i3,i4,i5,d1,d2,d3,d4)
C
C              boucle
C
 100  iter = iter + 1
      itimp = itimp + 1
      if (iter .lt. itmax) goto 110
      if (imp .gt. 0) call n1fc1o(io,5,iter,i2,i3,i4,i5,d1,d2,d3,d4)
      mode = 4
      goto 900
 110  ntot = ntot + 1
      if (logic .eq. 3) ro = ro * dsqrt(s2)
      if (itimp .ne. -imp) goto 200
      itimp = 0
      indic = 1
      call simul(indic,n,xn,f,g,izs,rzs,dzs)
C
C         calcul de la direction
C
 200  eps = dmin1(eps,epsm)
      eps = dmax1(eps,eps0)
      call fremf2(prosca,iflag,n,ntot,nta,memax1,q,poids,e,a,r,izs,rzs,
     &            dzs)
      call fprf2(iflag,ntot,nv,io,zero,s2,eps,al,imp,u,eta2,memax1,jc,
     &           ic,r,a,e,rr,xga,y,w1,w2)
C
C         fin anormale de fprf2
C
      if (iflag .eq. 0) goto 250
      if (imp .gt. 0) call n1fc1o(io,6,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      mode = 7
      goto 900
 250  nta = ntot
      call ffinf1(n,nv,jc,xga,q,s)
      u = dmax1(u,0.d0)
      s2 = dmax1(s2,0.d0)
C
C          tests d'arret (nb. si nr g est interieur a g
C                                alors nr g est "nul")
C
      if (nv .lt. n+2) goto 260
      eta2 = dmax1(eta2,10.d0*s2)
      if (imp .ge. 2) call n1fc1o(io,7,i1,i2,i3,i4,i5,eta2,d2,d3,d4)
 260  if (s2 .gt. eta2) goto 300
C
C         calcul de la precision
      z = 0.d0
      do 270 k = 1,nv
        j = jc(k) - 1
        if (j .gt. 0) z = z + xga(k)*poids(j)
 270  continue
      epsm = dmin1(eps,z)
      if (imp.ge.2) call n1fc1o(io,8,iter,nsim,i3,i4,i5,fn,epsm,s2,d4)
      if (epsm .gt. eps0) goto 280
      mode = 1
      if (imp .gt. 0) call n1fc1o(io,9,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      goto 900
C
C         diminution de epsilon
 280  epsm = dmax1(0.1d0*epsm,eps0)
      eps = epsm
      if (logic .eq. 3) tol = 0.01d0 * eps
      iflag = 2
      goto 200
C
C                 suite des iterations
C                    impressions
C
 300  if (imp .gt. 3) call n1fc1o(io,10,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      if (imp .gt. 2) call n1fc1o(io,11,iter,nsim,nv,i4,i5,fn,eps,s2,u)
      if (imp .ge. 6) call n1fc1o(io,12,ntot,i2,i3,i4,i5,d1,d2,d3,poids)
C                test de non-pivotage
      if (logic .ne. 3) goto 350
      z = 0.d0
      do 310 i = 1,n
        z1 = s(i) - sa(i)
 310  z = z + z1*z1
      if (z .gt. 10.d0*zero*zero*s2) goto 350
      if (imp .gt. 0) call n1fc1o(io,13,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      mode = 8
      goto 900
C
C                recherche lineaire
C
 350  iflag = 3
      s3 = s2 + u*eps
      if (logic .eq. 3) goto 365
      ro = 2. * df / s3
      tol = 0.01d0 * eps
      goto 370
 365  ro = ro / dsqrt(s2)
      tol = dmax1(0.6d0*tol,0.01d0*eps0)
 370  fa = fn
      alfa = 0.2d0
      beta = 0.1d0
      fpn = -s3
      if (memax .eq. 1) tol = 0.d0
C                 calcul de la resolution minimale, fonction de dx
      tmin = 0.d0
      do 372 i = 1,n
 372  tmin = dmax1(tmin,dabs(s(i)/dx))
      tmin = 1.d0 / tmin
      if (iter .eq. 1) roa = ro
      call nlis2(simul,prosca,n,xn,fn,fpn,ro,tmin,tmax,s,s2,g,gd,alfa,
     &           beta,imp,io,logic,nsim,napmax,x,tol,ap,tps,tnc,gg,izs,
     &           rzs,dzs)
      if (logic.eq.0 .or. logic.eq.2 .or. logic.eq.3) goto 380
C                 sortie par anomalie dans nlis2
      if (imp .le. 0) goto 375
      if (logic.eq.6 .or. logic.lt.0)
     &  call n1fc1o(io,14,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      if (logic .eq. 4) call n1fc1o(io,15,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      if (logic .eq. 5) call n1fc1o(io,16,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      if (logic .eq. 1) call n1fc1o(io,17,i1,i2,i3,i4,i5,d1,d2,d3,d4)
 375  if (logic .eq. 1) mode = 3
      if (logic .eq. 4) mode = 5
      if (logic .eq. 5) mode = 0
      if (logic .eq. 6) mode = 6
      if (logic .lt. 0) mode = logic
      goto 900
 380  if (logic .ne. 3) goto 385
      do 382 i = 1,n
 382  sa(i) = s(i)
 385  if (iter .gt. 1) goto 390
C
C              1ere iteration, ajustement de ap, diam et eta
      if (logic .eq. 0) tps = (fn-fa) - ro*fpn
      ap = zero * zero * dabs(tps) / (s2*ro*ro)
      ajust = ro / roa
      if (logic .ne. 3) diam2 = diam2 * ajust * ajust
      if (logic .ne. 3) eta2 = eta2 / (ajust*ajust)
      if (imp .ge. 2) call n1fc1o(io,18,i1,i2,i3,i4,i5,diam2,eta2,ap,d4)
 390  mm = memax - 1
      if (logic .eq. 2) mm = memax - 2
      if (ntot .le. mm) goto 400
C
C      reduction du faisceau pour entrer le nouvel element
C
      call frdf1(prosca,n,ntot,mm,kgrad,al,q,s,poids,aps,anc,memax1,r,e,
     &           ic,izs,rzs,dzs)
      iflag = 1
      nta = ntot
      if (imp .ge. 2)
     &  call n1fc1o(io,19,iter,nsim,ntot,i4,i5,fn,d2,d3,d4)
C
 400  if (imp .ge. 5) call n1fc1o(io,20,logic,i2,i3,i4,i5,ro,tps,tnc,d4)
      if (logic .eq. 3) goto 500
C
C                 iteration de descente
C
      iflag = min0(iflag,2)
      df = fa - fn
      if (ntot .eq. 0) goto 500
C
C               actualisation des poids
C
      s3n = ro * dsqrt(s2)
      do 430 k = 1,ntot
        nk = (k-1)*n + 1
        call prosca(n,q(nk),s,ps,izs,rzs,dzs)
        z1 = dabs(aps(k)+(-df+ro*ps))
        z2 = anc(k) + s3n
        poids(k) = dmax1(z1,ap*z2*z2)
        aps(k) = z1
        anc(k) = z2
 430  continue
C
C                actualisation de eps
C
      eps = ro * s3
      kgrad = ntot + 1
C
C       nouvel element du faisceau (pour les trois types de pas)
C
 500  nt1 = ntot + 1
      if (logic .eq. 3) goto 510
      aps(nt1) = 0.d0
      anc(nt1) = 0.d0
      poids(nt1) = 0.d0
      goto 520
 510  aps(nt1) = tps
      anc(nt1) = dsqrt(tnc)
      poids(nt1) = dmax1(tps,ap*tnc)
 520  nk = ntot * n
      do 530 i = 1,n
        nki = nk + i
 530  q(nki) = -g(i)
C
C      traitement pour logic=2 (on ajoute encore un gradient)
      if (logic .ne. 2) goto 550
      ntot = ntot + 1
      logic = 3
      logic2 = 1
      do 540 i = 1,n
 540  g(i) = gd(i)
      goto 390
 550  logic = logic - logic2
      logic2 = 0
      goto 100
C
C                epilogue
C
 900  if (iter .le. 1) goto 990
      do 910 i = 1,n
 910  g(i) = -s(i)
 990  return
      end
