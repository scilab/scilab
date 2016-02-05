      subroutine domout(neq,q,qi,nbout,ti,touti,itol,rtol,atol,itask

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

     $     ,istate,iopt,w,lrw,iw,liw,jacl2,mf,job)
C!but
C     Etant sortie du domaine d'integration au cours
C     de l'execution de la routine Optm2, il s'agit ici de
C     gerer ou d'effectuer l'ensemble des taches necessaires
C     a l'obtention du point de la face par lequel la
C     'recherche' est passee.
C!liste d'appel
C     subroutine domout(neq,q,qi,nbout,ti,touti,itol,rtol,atol,itask,
C    *     istate,iopt,w,lrw,iw,liw,jacl2,mf,job)
C
C     double precision  atol(neq(1)+1),rtol(neq(1)+1),q(neq(1)+1),
C    *                  qi(neq(1)+1)
C     double precision w(*),iw(*)
C
C     Entree :
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome q
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de 
c               fourier dans tq est neq(3)+2
c         neq(4:(nq+1)*(nq+2)) tableau de travail entier
c     - tq. tableau reel de taille au moins
c               7+dgmax+5*nq+6*ng+nq*ng+nq**2*(ng+1)
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         5+5*nq+5*ng+nq*ng+nq**2*(ng+1)
c
C     - toutes les variables et tableaux de variables necessaires a 
C               l'execution de la routine Lsode
C     - qi. est le dernier point obtenu de la trajectoire
C        qui soit a l'interieur du domaine.
C     - q(1:nq+1). est celui precedemment calcule, qui se situe a
C       l'exterieur.
C
C     Sortie :
C     - q(1:nq+1). est cense etre le point correspondant a l'inter-
C        section entre la face et la trajectoire.
C     - job. est un parametre indiquant si le franchissement
C            est verifie.
C            si job=-1 pb de detection arret requis
C
C     Tableaux de travail
C     - w : 24+22*nq+ng+nq**2
C     - iw : 20+nq
C!
      implicit double precision (a-h,o-z)
      dimension atol(*), rtol(*), w(*), iw(*), q(*),
     &          qi(*), xx(1)

      integer neq(*)
      external feq, jacl2
cDEC$ ATTRIBUTES DLLIMPORT:: /sortie/
      common /sortie/ io,info,ll
C
      nq=neq(1)
      ng=neq(2)
      nqmax=neq(3)
c
      lq=1
      ltg=lq+nqmax+1
c
      lrw=nq**2 + 9*nq + 22
      liw=20+nq
c

      lrwork = 1
      lw     = lrwork + nq**2 + 9*nq + 22
      lqex   = lw+12*nq+ng+1
      free   = lqex + nq + 1

C
      tout = touti
      nboute = 0
C
C     --- Etape d'approche de la frontiere ----------------------------
C
      kmax = int(log((tout-ti)/0.006250d+0)/log(2.0d+0))
      k0 = 1
      if (info .gt. 1) call outl2(40,nq,kmax,xx,xx,x,x)
 314  do 380 k = k0,kmax
        tpas = (tout-ti) / 2.0d+0
        if (nbout .gt. 0) then
          istate = 1
          call dcopy(nq+1,qi,1,q,1)
          t = ti
          tout = ti + tpas
        else
          call dcopy(nq+1,q,1,qi,1)
          ti = t
          tout = ti + tpas
        endif
 340    if (info .gt. 1) call outl2(41,nq,nq,q,xx,t,tout)
        tsave=t
        call lsode(feq,neq,q,t,tout,itol,rtol,atol,itask,istate,iopt,
     &             w(lrwork),lrw,iw,liw,jacl2,mf)
        if (info .gt. 1) call outl2(42,nq,nq,q,xx,t,tout)
        if (istate.eq.-1 .and. t.ne.tout) then
          if (info .gt. 1) call outl2(43,nq,nq,xx,xx,x,x)
           if (t.le.tsave) then
              job=-1
              return
           endif
          istate = 2
          goto 340
        endif
        call front(nq,q,nbout,w(lw))
        if (info .gt. 1) call outl2(44,nq,nbout,xx,xx,x,x)
        if (nbout .gt. 0) then
          nboute = nbout
          call dcopy(nq+1,q,1,w(lqex),1)
        endif
        if (istate .lt. 0) then
          if (info .gt. 1) call outl2(45,nq,istate,xx,xx,x,x)
          job = -1
          return
        endif
        if (k.eq.kmax .and. nboute.eq.0 .and. tout.ne.touti) then
          tout = touti
          goto 340
        endif
 380  continue
c
      if (nboute .eq. 0) then
        job = 0
        return
      elseif (nboute .gt. 2) then
        newrap = 1
        nqsav = nq
        goto 390
      endif
C
      call watfac(nq,w(lqex),nface,newrap,w(lw))
      if (newrap .eq. 1) goto 390
C
      nqsav = nq
      call onface(nq,q,q(ltg),ng,nface,ierr,w(lw))
      if (ierr .ne. 0) then
        job = -1
        return
      endif
      yi = phi(qi,nqsav,q(ltg),ng,w(lw))
      yf = phi(q,nq,q(ltg),ng,w(lw))
C
      eps390 = 1.0d-08
      if (yi .lt. (yf-eps390)) then
        newrap = 1
        goto 390
      endif
C
      if (info .gt. 1) call outl2(46,nq,nface,q,xx,yi,yf)
C
      newrap = 0
C
 390  if (newrap .eq. 1) then
        nq = nqsav
        k0 = kmax
        kmax = kmax + 1
        nbout = 1
        if(ti + 2*tpas.le.ti) then
           job=-1
           return
        endif
        tout = ti + 2*tpas
        if (info .gt. 1) call outl2(47,nq,nq,xx,qi,x,x)
        goto 314
      endif
C
      neq(1)=nq
      job = 1
      return
C
      end

