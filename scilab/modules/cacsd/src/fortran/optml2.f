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

      subroutine optml2(feq,jacl2,neq,q,nch,w,iw)
C!but
C      Routine de recherche de minimum du probleme d'approximation L2
C       par lsoda ( Lsoda = routine de resolution d'equa diff )
C!liste d'appel
C     subroutine optml2(feq,jacl2,neq,q,nch,w,iw)
C
C     external feq,jacl2
C     double precision q(*),w(*)
C     integer nch,iw(*)
C
C     Entrees :
C     - feq est la subroutine qui calcule le gradient,
C        oppose de la derivee premiere de la fonction phi.
c     - neq. tableau entier de taille 3+(npara+1)*(npara+2)
c         neq(1)=nq est le degre effectif du polynome  q.
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de fourier dans
c               q est neq(3)+2
C     - neq est le degre du polynome q
c     - tq. tableau reel de taille au moins
c               6+dgmax+5*nq+6*ng+nq*ng+nq**2*(ng+1)
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         5+5*nq+5*ng+nq*ng+nq**2*(ng+1)
C     - nch est l indice (valant 1 ou 2) qui classifie l
C       appel comme etant soit celui de la recherche et de la
C       localisation d un minimum local, soit de la
C       confirmation d un minimum local.
C
C     Sorties :
C     - neq est toujours le degre du polynome q (il peut  avoir varie).
C     - q est le polynome (ou plutot le tableau contenant
C         ses coefficients) qui resulte de la recherche ,il peut
C         etre du meme degre que le polynome initial mais aussi
C         de degre inferieur dans le cas d'une sortie de face.
C
C     Tableau de travail
C     - w de taille 25+26*nq+ng+nq**2
C     - iw de taille 20+nq
C!

      implicit double precision (a-h,o-y)
      dimension q(*), w(*), iw(*), xx(1)
      integer neq(*)
      double precision x,phi0,phi,gnrm
C
      external feq, jacl2
cDEC$ ATTRIBUTES DLLIMPORT:: /sortie/
cDEC$ ATTRIBUTES DLLIMPORT:: /comall/
cDEC$ ATTRIBUTES DLLIMPORT:: /no2f/
      common /temps/ t
      common /comall/ nall1 /sortie/ io,info,ll
      common /no2f/ gnrm
c
      nq=neq(1)
      ng=neq(2)
      ltg=1+neq(3)
C
c     taille des tableaux de travail necessaires a lsode
      lrw = nq**2 + 9*nq + 22  
      liw = 20+nq

C     decoupage du tableau de travail w
      lqi = 1
      lqdot = lqi + nq + 1
      latol = lqdot + nq
      lrtol = latol + nq
      lwork = lrtol + nq
      lfree = lwork + 24+22*nq+ng+nq**2
c
      lw = lwork + lrw

C     decoupage du tableau de travail iw 
      liww=1
      lifree=liww+liw
C
      nqbac = nq
C
C     --- Initialisation de lsode ------------------------
C
      if (nch .eq. 1) t = 0.0d+0
      t0 = t
      tt = 0.10d+0
      tout = t0 + tt
      itol = 4
C
      if (nq .lt. 7) then
        ntol = int((nq-1)/3) + 5
      else
        ntol = int((nq-7)/2) + 7
      endif
      call dset(nq,10.0d+0**(-(ntol)),w(lrtol),1)
      call dset(nq,10.0d+0**(-(ntol+2)),w(latol),1)
C
      itask = 1
      if (nch .eq. 1) istate = 1
      if (nch .eq. 2) istate = 3
      iopt = 0
      mf = 21
C
C     --- Initialisation du nombre maximal d'iteration ---
C
      if (nch .eq. 1) then
        if (nq .le. 11) then
          nlsode = 11 + 2*(nq-1)
        else
          nlsode = 29
        endif
      else
        nlsode = 19
      endif
      ilcom = 0
      ipass = 0
C
C     --- Appel  de lsode --------------------------------
C
 210  do 290 i = 1,nlsode
C
 220    ilcom = ilcom + 1
C
C     -- Reinitialisation de la Tolerance --
C
        if (ilcom.eq.2 .and. nch.eq.1) then
          call dset(nq,1.0d-05,w(lrtol),1)
          call dset(nq,1.0d-07,w(latol),1)
          istate = 3
        elseif (ilcom.eq.2 .and. nch.eq.2) then
          w(lrtol) = 1.0d-08
          w(latol) = 1.0d-10
          w(lrtol+1) = 1.0d-07
          w(latol+1) = 1.0d-09
          w(lrtol+nq-1) = 1.0d-05
          w(latol+nq-1) = 1.0d-07
          do 240 j = 2,nq-2
            w(lrtol+j) = 1.0d-06
            w(latol+j) = 1.0d-08
 240      continue
          istate = 3
        endif
C
C     --------------------------------------
C
        call dcopy(nq+1,q,1,w(lqi),1)
        ti = t
        touti = tout
C
        if (info .gt. 1) call outl2(30,nq,nq,q,xx,t,tout)
C

        call lsode(feq,neq,q,t,tout,itol,w(lrtol),w(latol),itask,
     &             istate,iopt,w(lwork),lrw,iw(liww),liw,jacl2,mf)
C
        call front(nq,q,nbout,w(lw))
C
        call feq(neq,t,q,w(lqdot))
        dnorm0 = dnrm2(nq,w(lqdot),1)
        if (info .gt. 1) call outl2(31,nq,nbout,q,dnorm0,t,tout)
C
C     -- test pour degre1 -----------
        if (nall1.gt.0 .and. nq.eq.1 .and. nbout.gt.0) return
C
C
C     -- Istate de lsode ------------
C
        if (istate .eq. -5) then
          if (info .gt. 0) call outl2(32,nq,nq,xx,xx,x,x)
          call dscal(nq,0.10d+0,w(lrtol),1)
          call dscal(nq,0.10d+0,w(latol),1)
          if (t .eq. 0.0d+0) istate = 1
          if (t .ne. 0.0d+0) istate = 3
          ilcom = 0
          goto 220
        endif
C
        if (istate .eq. -6) then
C     echec de l'integration appel avec de nouvelles tolerances
          if (info .gt. 0) call outl2(33,nq,nq,xx,xx,x,x)
          if (info .gt. 1)
     &      call outl2(34,nq,itol,w(latol),w(lrtol),t,tout)
          iopt = 0
          itol = 4
          call dset(nq,0.10d-05,w(lrtol),1)
          call dset(nq,0.10d-05,w(latol),1)
          if (info .gt. 1) call outl2(35,nq,itol,w(latol),w(lrtol),x,x)
          if (info .gt. 0) call outl2(36,nq,nq,xx,xx,x,x)
          istate = 3
          if (t .ne. tout) goto 220
        endif
C
        if (istate.lt.-1 .and. istate.ne.-6 .and. istate.ne.-5) then
          if (info .gt. 0) call outl2(37,nq,iopt,xx,xx,x,x)
          nch = 15
          return
        endif
C
C     -------------------------------
C
C     -- Sortie de face -------------
C
        if (nbout.gt.0 .and. nbout.ne.99) then
          call domout(neq,q,w(lqi),nbout,ti,t,itol,w(lrtol),
     &          w(latol),itask,istate,iopt,w(lwork),lrw,iw(liww),liw,
     &          jacl2,mf,job)
          nq=neq(1)
          if (job .eq. -1) then
C     anomalie dans la recherche de l'intersection
            nch = 16
            return
          endif
          if (job .eq. 1) then
            nch = nq - nqbac
            return
          endif
        endif
C
C     -------------------------------
C
C     -- test sur le gradient -------
C
        epstop = (1.0d-06)**nch
        call feq(neq,t,q,w(lqdot))
        dnorm0 = dnrm2(nq,w(lqdot),1)
        if (dnorm0 .lt. epstop) goto 299
C
C     -------------------------------
C
C     -- Istate de lsode (suite) ----
C
        if (istate.eq.-1 .and. t.ne.tout) then
          if (info .gt. 0) call outl2(38,nq,nq,xx,xx,x,x)
          istate = 2
          goto 220
        endif
C
C     -------------------------------
C
        tt = sqrt(10.0d+0) * tt
        tout = t0 + tt
C
 290  continue
C
      if (nch.eq.2 .and. dnorm0.gt.(1.0d-06)) then
        ipass = ipass + 1
        if (ipass .lt. 5) then
          if (info .gt. 0) then
             call lq(nq,q,w(lw),q(ltg),ng)
             x=sqrt(gnrm)
             call dscal(nq,x,w(lw),1)
             call outl2(14,nq,nq,q,w(lw),x,x)

             phi0= abs(phi(q,nq,q(ltg),ng,w(lw)))
             call feq(neq,t,q,w(lqdot))
             call outl2(17,nq,nq,q,w(lqdot),phi0,x)
          endif
          goto 210
        else
          if (info .gt. 0) call outl2(39,nq,nq,xx,xx,x,x)
          nch = 17
          return
        endif
      endif
C
 299  return
C
      end

