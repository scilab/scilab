      subroutine onface(nq,tq,tg,ng,nprox,ierr,w)
C!but
C     il est question ici de calculer (ou d estimer)
C     le polynome (ou point) qui se situe a l'intersection
C     de la recherche et de la face-frontiere du domaine.
C!liste d'appel
C     subroutine onface(nq,tq,nprox)
C
C     double precision tq(0:nq),w(*)
C     integer nq,nprox,ierr
C
C     Entree :
C     - nq. est le degre du polynome q(z) avant toute
C        modification.
C     - tq. est le tableau de ses coefficients
C     - nprox. est l indice de la face par laquelle on estime
C        que la recherche a franchi la frontiere du domaine.
C
C     Sortie :
C     -nq. est alors le degre des polynomes de la face
C       traversee et donc du polynome intersection. Sa valeur
C       est inferieur de 1 ou 2 a sa valeur precedente.
C     - tq. contient en sortie les coefficients du polynome
C       intersection dans le domaine de la face traversee.
C
C     Tableau de travail
C     - w : 12*nq+ng+1
C!
c     Copyright INRIA
      implicit double precision (a-h,o-y)
      dimension tq(0:nq), w(*),tg(ng+1)
C
      dimension tps(0:1), taux2(0:2), tabeta(0:2), xx(1)
      common /sortie/ io,info,ll
C
C     decoupage du tableau de travail
      lqaux = 1
      lqdot = lqaux
      lrq0 = lqdot + nq + 1
      lrq1 = lrq0 + nq
      lrgd0 = lrq1 + nq
      lrgd1 = lrgd0 + nq
      lgp = lrgd1 + nq
      lgp1 = lgp + 2*nq - 2
      lbeta = lgp1
      lw = lbeta + 2*nq - 2
      lfree = lw+3*nq+ng+1

C
      nqvra = nq
C
      tps(1) = 1.0d+0
      tps(0) = 1.0d+0
C
      if (nprox .ne. 0) then
        tps(0) = real(nprox)
C     calcul du reste de la division de q par tps
        call horner(tq,nq,-tps(0),0.0d+0,srq,xx)
C     calcul du reste de la division de qdot  par 1+z
        call feq1(nq,t,tq,tg,ng,w(lqdot),w(lw))
        call horner(w(lqdot),nq,-tps(0),0.0d+0,srgd,xx)
C
        call daxpy(nq,(-srq)/srgd,w(lqdot),1,tq,1)
C
        call dpodiv(tq,tps,nq,1)
        if (info .gt. 0) call outl2(70,1,1,xx,xx,x,x)
        if (info .gt. 1) call outl2(71,1,1,tq,xx,x,x)
        call dcopy(nq,tq(1),1,tq,1)
        nq = nq - 1
C
      elseif (nprox .eq. 0) then
C
        taux2(2) = 1.0d+0
        taux2(1) = 0.0d+0
        taux2(0) = 1.0d+0
C
        call dcopy(nq+1,tq,1,w(lqaux),1)
        do 200 ndiv = 0,nq-2
          call dpodiv(w(lqaux),taux2,nq-ndiv,2)
          w(lrq1+ndiv) = w(lqaux+1)
          w(lrq0+ndiv) = w(lqaux)
C
          do 180 j = 2,nq-ndiv
            w(lqaux+j-1) = w(lqaux+j)
 180      continue
          w(lqaux) = 0.0d+0
 200    continue
        w(lrq1-1+nq) = w(lqaux+1)
        w(lrq0-1+nq) = w(lqaux)
C
        call feq1(nq,t,tq,tg,ng,w(lqaux),w(lw))
        nqdot = nq - 1
C
        do 240 ndiv = 0,nqdot-2
          call dpodiv(w(lqaux),taux2,nqdot-ndiv,2)
          w(lrgd1+ndiv) = w(lqaux+1)
          w(lrgd0+ndiv) = w(lqaux)
C
          do 220 j = 2,nqdot-ndiv
            w(lqaux+j-1) = w(lqaux+j)
 220      continue
          w(lqaux) = 0.0d+0
 240    continue
        w(lrgd1-1+nqdot) = w(lqaux+1)
        w(lrgd0-1+nqdot) = w(lqaux)
C
C     - construction du polynome gp(z) dont on cherchera une racine
C     comprise entre -2 et +2 -----------------------------
C
        call dset(2*nq-2,0.0d+0,w(lgp),1)
        call dset(2*nq-2,0.0d+0,w(lgp1),1)
C
        do 260 j = 1,nq
          do 258 i = 1,nqdot
            k = i + j - 2
            w(lgp+k) = w(lgp+k) + ((-1)**k)*w(lrq0-1+j)*w(lrgd1-1+i)
            w(lgp1+k) = w(lgp1+k) + ((-1)**k)*w(lrq1-1+j)*w(lrgd0-1+i)
 258      continue
 260    continue
C
        call ddif(2*nq-2,w(lgp1),1,w(lgp),1)
        ngp = 2*nq - 3
        call rootgp(ngp,w(lgp),nbeta,w(lbeta),ierr,w(lw))
        if (ierr .ne. 0) return
C
        do 299 k = 1,nbeta
C
C     - calcul de t (coeff multiplicateur) -
C
          auxt1 = 0.0d+0
          do 280 i = 1,nq
            auxt1 = auxt1 + w(lrq1-1+i)*((-w(lbeta-1+k))**(i-1))
 280      continue
C
          auxt2 = 0.0d+0
          do 290 i = 1,nqdot
            auxt2 = auxt2 + w(lrgd1-1+i)*((-w(lbeta-1+k))**(i-1))
 290      continue
C
          tmult = (-auxt1) / auxt2
C
          if (k .eq. 1) then
            t0 = tmult
            beta0 = w(lbeta)
          elseif (abs(tmult) .lt. abs(t0)) then
            t0 = tmult
            beta0 = w(lbeta-1+k)
          endif
C
 299    continue
C
        call feq1(nq,t,tq,tg,ng,w(lqdot),w(lw))
        call daxpy(nq,t0,w(lqdot),1,tq,1)
C
        tabeta(2) = 1.0d+0
        tabeta(1) = beta0
        tabeta(0) = 1.0d+0
        call dpodiv(tq,tabeta,nq,2)
        if (info .gt. 0) call outl2(70,2,2,xx,xx,x,x)
        if (info .gt. 1) call outl2(71,2,2,tq,xx,x,x)
C
        call dcopy(nq-1,tq(2),1,tq,1)
        nq = nq - 2
C
      endif
C
      return
      end

