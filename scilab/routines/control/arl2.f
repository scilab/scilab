      subroutine arl2(f,nf,num,tq,dgmin,dgmax,errl2,w,iw,inf,ierr,ilog)
C!but
C     Cette procedure a pour but de gerer l'execution dans
C     le cas ou un unique polynome approximant est desire
C!liste d'appel
C      subroutine arl2(f,nf,num,tq,dgmin,dgmax,errl2,w,
C     $     inf,ierr,ilog)
C
C     double precision tq(dgmax+1),f(nf),num(dgmax)
C     double precision w(*)
C     integer dgmin,dgmax,dginit,info,ierr,iw(*)
C
C     Entree :
C     dgmin. est le degre du polynome de depart quand il est
C        fourni, (vaux 0 s'il ne l'est pas).
C     dginit. est le premier degre pour lequel aura lieu la
C        recherche.
C     dgmax. est le degre desire du dernier approximant
C     tq. est le tableau contenant le polynome qui peut etre
C        fourni comme point de depart par l'utilisateur.
C
C     Sortie :
C     tq. contient la solution obtenu de degre dgmax.
C     num. contient les coefficients du numerateur optimal
C     errl2. contient l'erreur L2 pour l'optimum retourne
C     ierr. contient l'information sur le deroulement du programme
C          ierr=0 : ok
C          ierr=3 : boucle indesirable sur 2 ordres
C          ierr=4 : plantage lsode
C          ierr=5 : plantage dans recherche de l'intersection avec une face
C
C  tableau de travail
C     w: dimension:  32+32*dgmax+7*ng+dgmax*ng+dgmax**2*(ng+2)
C     iw : dimension  29+dgmax**2+4*dgmax
C!sous programme appeles
C  optml2,feq,jacl2,outl2,lq,phi (arl2)
C  dcopy,dnrm2,dscal,dpmul1
C!origine
C M Cardelli, L Baratchart  INRIA sophia-Antipolis 1989, S Steer 
C!organigramme
C arl2
C    optml2
C          outl2
C          feq
C          domout
C                 onface
C                        rootgp
C                        feq
C                        outl2
C                 outl2
C                 phi
C                 lsode
C                 front
C                 watfac
C          front
C          lsode
C                 feq
C                 jacl2
C                        hessl2
C                              lq
C    outl2
C          feq 
C          phi
C          lq
C          jacl2
C    phi
C          lq
C          calsca
C    feq
C          lq
C          calsca       
C    lq
C!
c     Copyright INRIA
      integer dgmin,dgmax,dginit,info,ierr,iw(*)
      double precision tq(dgmax+1),f(nf),num(dgmax),w(*),x
C
      double precision errl2,xx(1)
      double precision tps(2),tms(2),dnrm2,sqrt,phi,gnrm,phi0
      integer dg,dgback,dgr
      external feq, jacl2
      common /sortie/ io,info,ll
      common /no2f/ gnrm
C
c     taille des tableaux de travail necessaires a lsode
      lrw = dgmax**2 + 9*dgmax + 22  
      liw = 20+dgmax

C     decoupage du tableau de travail w
      ncoeff=nf
      ng=nf-1
      ltq   = 1
      ltg   = ltq+dgmax+1
      lwode = ltg+ng+1
      ltr   = lwode+5+5*dgmax+5*ng+dgmax*ng+dgmax**2*(ng+1)
      lfree = ltr + 25+26*dgmax+ng+dgmax**2

c     les lrw elements de w suivant w(ltr) ne doivent pas etre modifies 
c     d'un appel de optml2 a l'autre 
      lw=ltr+lrw
C
C     decoupage du tableau de travail iw
      liwode = 1
      liww   = liwode+4+(dgmax+1)*(dgmax+2)
      lifree = liww+20+dgmax
      iw(liwode+1)=ng
      iw(liwode+2)=dgmax
      ll   = 80
      info = inf
      io   = ilog
C
C test validite des arguments
C
      if (dgmin .gt. 0) then
        dginit = dgmin
        call dcopy(dgmin+1,tq,1,w(ltq),1)
      else
        w(ltq) = 1.d0
        dginit = 1
      endif
C
      dgr=dginit
      ierr = 0
      ntest1 = -1
C
      ng = nf - 1
      call dcopy(nf,f,1,w(ltg),1)
      gnrm = dnrm2(nf,f,1)
      call dscal(nf,1.0d+0/gnrm,w(ltg),1)
      gnrm = gnrm**2
C
      tps(1) = 1.0d+0
      tps(2) = 1.0d+0
      tms(1) = -1.0d+0
      tms(2) = 1.0d+0
C
C     ---- Boucle de calcul ---------------------------------------------
C
      do 500 nnn = dginit,dgmax
C
        ifaceo = 0
C
        if (nnn .eq. dginit) then
          if (dgmin .gt. 0) then
            dg = dginit
            goto 230
          else
            dg = dginit - 1
          endif
        endif
C
 200    dg = dg + 1
C
C     -- Initialisation du nouveau point de depart --
C     (dans l'espace de dimension dg , Hyperespace superieur
C     d'une dimension par rapport au precedent ).
C
        if (ntest1 .eq. 1) then
          call dpmul1(w(ltq),dg-1,tps,1,w(ltr))
          call dcopy(dg+1,w(ltr),1,w(ltq),1)
        elseif (ntest1 .eq. -1) then
          call dpmul1(w(ltq),dg-1,tms,1,w(ltr))
          call dcopy(dg+1,w(ltr),1,w(ltq),1)
        endif
C
C     ------------------------
C
 230    dgback = dg
C
        if (info .gt. 1) call outl2(20,dg,dgback,xx,xx,x,x)
C
        nch = 1
        iw(liwode)=dg
        call optml2(feq,jacl2,iw(liwode),w(ltq),nch,w(ltr),iw)
        dg=iw(liwode)
        if (info .gt. 1) then
             call lq(dg,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(dg,x,w(lw),1)
             call outl2(nch,dg,dg,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),dg,w(ltg),ng,w(lw)))
             lqdot=lw
             call feq(iw(liwode),t,w(ltq),w(lqdot))
             call outl2(17,dg,dg,w(ltq),w(lqdot),phi0,x)
        endif

        if (nch .ge. 15) then
          if(nch.eq.17) then
             call dcopy(dg+1,w(ltq),1,tq,1)
             dgr=dg
             goto 231
          endif
          ierr = 4 + nch - 15
          goto 510
        endif
C
        if (nch .lt. 0) then
          ifaceo = ifaceo + 1
          ntest1 = (-1) * ntest1
          if (dg .eq. 0) goto 200
          goto 230
        endif
C
        if (info .gt. 1) call outl2(21,dg,dg,xx,xx,x,x)
        nch = 2
        iw(liwode)=dg
        call optml2(feq,jacl2,iw(liwode),w(ltq),nch,w(ltr),iw)
        if (info .gt. 0) then
             call lq(dg,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(dg,x,w(lw),1)
             call outl2(nch,dg,dg,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),dg,w(ltg),ng,w(lw)))
             lqdot=lw
             call feq(iw(liwode),t,w(ltq),w(lqdot))
             call outl2(17,dg,dg,w(ltq),w(lqdot),phi0,x)
        endif
        if (nch .ge. 15) then
          if(nch.eq.17) then
             call dcopy(dg+1,w(ltq),1,tq,1)
             dgr=dg
             goto 231
          endif
          ierr = 4 + nch - 15
          goto 510
        endif
C
        if (nch .lt. 0) then
          ifaceo = ifaceo + 1
          ntest1 = (-1) * ntest1
          if (dg .eq. 0) goto 200
          goto 230
        endif
C
C
 231    if (ifaceo .eq. 8) then
          if (info .ge. 0) call outl2(22,dg,dg,xx,xx,x,x)
          ierr = 3
          goto 510
        endif
C
        if (dg .lt. nnn) goto 200
        call dcopy(dg+1,w(ltq),1,tq,1)
        dgr=dg
C
 500  continue
C
C Fin de la recherche Optimale
C numerateur optimal
 510  gnrm = sqrt(gnrm)
      call lq(dgr,tq,w(ltr),w(ltg),ng)
      call dcopy(dgr,w(ltr),1,num,1)
      call dscal(dgr,gnrm,num,1)
C     Le gradient de la fonction critere y vaut :-tqdot
C     call feq(dg,t,w(ltq),tqdot)
C     valeur du critere
      lw = ltg+ncoeff+1
      errl2 = sqrt(phi(tq,dgr,w(ltg),ng,w(lw))) * gnrm
      dgmax=dgr
C
      return
      end

