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

      subroutine degl2(tg,ng,neq,imina,iminb,iminc,ta,tb,tc,iback,
     &     ntback,tback,mxsol,w,iw,ierr)
C!but
C     Cette procedure a pour objectif de determiner le plus grand
C     nombre de minimums de degre "neq".
C!liste d'appel
C     subroutine degre (neq,imina,iminb,iminc,ta,tb,tc,
C    &     iback,ntback,tback)
C
C     Entree :
C     -neq. est le degre des polynomes parmi lesquels ont
C       recherche les minimums.
C     -imina. est le nombre de minimums de degre "neq-1"
C       contenus dans ta.
C     -iminb. est le nombre de minimums de degre "neq-2"
C       contenus dans tb.
C     -iminc. est le nombre de minimums de degre "neq-3"
C       contenus dans tc.
C     -ta. est le tableau contenant donc les minimums de degre
C       "neq-1"
C     -tb. est le tableau contenant donc les minimums de degre
C       "neq-2"
C     -tc. est le tableau contenant donc les minimums de degre
C       "neq-3"
C     -iback. est le nombre de minimums obtenus apres une
C       intersection avec la frontiere
C     -ntback est un tableau d'entier qui contient les degre
C       de ces minimums
C     -tback. est le tableau qui contient leurs coefficients,
C       ou ils sont ordonnes degre par degre.
C
C     Sortie :
C     -imina. est le nombre de minimums de degre neq que l'on
C       vient de determiner
C     -iminb. est le nombre de minimums de degre "neq-1"
C     -iminc. est le nombre de minimums de degre "neq-2"
C     -ta. contient les mins de degre neq, -tb. ceux de degre
C       neq-1 et tc ceux de degre neq-2
C     -iback,ntback,tback ont pu etre augmente des mins obtenus
C       apres intersection eventuelle avec la frontiere.
C
C     tableaux de travail
C      w : 33+33*neq+7*ng+neq*ng+neq**2*(ng+2)
C      iw :29+neq**2+4*neq
c


C!
      implicit double precision (a-h,o-y)
      dimension ta(mxsol,*), tb(mxsol,*), tc(mxsol,*),tg(ng+1),
     &          ntback(mxsol), tback(mxsol,*)
      dimension w(*), iw(*), xx(1)
C
      dimension tps(0:1), tms(0:1)
      double precision x,phi0,phi,gnrm
C
      external feq, jacl2
cDEC$ ATTRIBUTES DLLIMPORT:: /sortie/
cDEC$ ATTRIBUTES DLLIMPORT:: /no2f/
cDEC$ ATTRIBUTES DLLIMPORT:: /comall/
      common /comall/ nall1
      common /sortie/ io,info,ll
      common /no2f/ gnrm
C
      tps(0) = 1.0d+0
      tps(1) = 1.0d+0
      tms(0) = -1.0d+0
      tms(1) = 1.0d+0
C
C
C     -------- Reinitialisation des tableaux --------
C
      if (neq .eq. 1) goto 111
C
      do 110 j = 1,iminb
        call dcopy(neq,tb(j,1),mxsol,tc(j,1),mxsol)
 110  continue
      iminc = iminb
C
 111  do 120 j = 1,imina
        call dcopy(neq,ta(j,1),mxsol,tb(j,1),mxsol)
 120  continue
      iminb = imina
      imina = 0
      neq = neq + 1
      neqbac = neq
c
      lrw = neq**2 + 9*neq + 22  
      liw = 20+neq
C     decoupage du tableau de travail w
      ltq = 1
      lwopt = ltq+6+6*neq+6*ng+neq*ng+neq**2*(ng+1)
      ltr = lwopt +25+26*neq+ng+neq**2
      lfree=ltr+neq+1
c
c     les lrw elements de w suivant w(lwopt) ne doivent pas etre modifies 
c     d'un appel de optml2 a l'autre 
      lw=lwopt+lrw

      ltg=ltq+neq+1
      call dcopy(ng+1,tg,1,w(ltg),1)

C     decoupage du tableau de travail iw
      lneq=1
      liwopt=lneq+3+(neq+1)*(neq+2)
      lifree =liwopt + 20+neq
c
      iw(lneq)=neq
      iw(lneq+1)=ng
      iw(lneq+2)=neq


      if (info .gt. 0) call outl2(51,neq,neq,xx,xx,x,x)
C
C     -------- Boucles de calculs --------
C
      do 190 k = 1,iminb
C
        call dcopy(neq-1,tb(k,1),mxsol,w(ltr),1)
        w(ltr+neq-1) = 1.0d+0
C
        do 180 imult = 1,2
C
          if (imult .eq. 1) then
            call dpmul1(w(ltr),neq-1,tps,1,w(ltq))
          elseif (imult .eq. 2) then
            call dpmul1(w(ltr),neq-1,tms,1,w(ltq))
          endif
C
 140      continue
C
          nch = 1
          call optml2(feq,jacl2,iw(lneq),w(ltq),nch,w(lwopt),iw(liwopt))
          neq=iw(lneq)
          if(info.gt.1) call outl2(nch,iw(lneq),neqbac,w(ltq),xx,x,x)
          if (info .gt. 0) then
             call lq(neq,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(neq,x,w(lw),1)
             call outl2(nch,neq,neqbac,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),neq,w(ltg),ng,w(lw)))
             lqdot=lw
             call feq(iw(lneq),t,w(ltq),w(lqdot))
             call outl2(17,neq,neq,w(ltq),w(lqdot),phi0,x)
          endif
          if (nch.eq.15 .and. nall1.eq.0) then
            ierr = 4
            return
          endif
C
          if (nch .eq. -1) goto 140
          if (nch .eq. -2) goto 140
C
          nch = 2
          call optml2(feq,jacl2,iw(lneq),w(ltq),nch,w(lwopt),iw(liwopt))
          neq=iw(lneq)
          if (info .gt. 1) then
             call lq(neq,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(neq,x,w(lw),1)
             call outl2(nch,neq,neqbac,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),neq,w(ltg),ng,w(lw)))
             lqdot=lw
             call feq(iw(lneq),t,w(ltq),w(lqdot))
             call outl2(17,neq,neq,w(ltq),w(lqdot),phi0,x)
          endif

          if (nch.eq.15 .and. nall1.eq.0) then
            ierr = 4
            return
          endif
C
C
          if (nch .eq. -1) goto 140
          if (nch .eq. -2) goto 140
C
          if (nch .eq. 15) then
            if (info .gt. 0) call outl2(50,neq,neq,xx,xx,x,x)
            goto 170
          endif
C
          nch = neq - neqbac
          if (nch .eq. -2) then
            call storl2(neq,w(ltq),w(ltg),ng,iminc,tc,iback,ntback,
     &                  tback,nch,mxsol,w(lwopt),ierr)
          elseif (nch .eq. -1) then
            call storl2(neq,w(ltq),w(ltg),ng,iminb,tb,iback,ntback,
     &                  tback,nch,mxsol,w(lwopt),ierr)
          else
            call storl2(neq,w(ltq),w(ltg),ng,imina,ta,iback,ntback,
     &                  tback,nch,mxsol,w(lwopt),ierr)
          endif
C
 170      neq = neqbac
          iw(lneq)=neq
C
 180    continue
 190  continue
      if (info .gt. 0) then
        x = real(mxsol)
        call outl2(53,neq,imina,ta,xx,x,x)
      endif
      return
      end

