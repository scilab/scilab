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

      subroutine deg1l2(tg,ng,imin,ta,mxsol,w,iw,ierr)
C!but
C     Determiner la totalite des polynome de degre 1.
C!liste d'appel
C     sorties :
C     -imin. est le nombre de minimums obtenus.
C     -ta. est le tableau dans lequel sont conserves les
C        minimums.
C     tableaux de travail (dgmax=1)
C     - w :32+32*dgmax+7*ng+dgmax*ng+dgmax**2*(ng+2)+2*mxsol
C     -iw : 29+dgmax**2+4*dgmax+ mxsol
C!remarque
C     on notera que le neq ieme coeff de chaque colonne
C     devant contenir le coeff du plus au degre qui est
C     toujours 1. contient en fait la valeur du critere
C     pour ce polynome.
C!
      implicit double precision (a-h,o-y)
      dimension ta(mxsol,*),tg(ng+1)
      external feq, feqn, jacl2, jacl2n
C
      double precision x,phi0,phi,gnrm
      dimension w(*), iw(*), xx(1)
      integer dgmax
cDEC$ ATTRIBUTES DLLIMPORT:: /sortie/
cDEC$ ATTRIBUTES DLLIMPORT:: /no2f/
      common /sortie/ io,info,ll
      common /no2f/ gnrm
C
C
      dgmax=1

      ltq=1
      lwopt=ltq+6+6*dgmax+6*ng+dgmax*ng+dgmax**2*(ng+1)
      ltback=lwopt+25+26*dgmax+ng+dgmax**2
      lfree = ltback + 2*mxsol
c
c     les lrw elements de w suivant w(lwopt) ne doivent pas etre modifies 
c     d'un appel de optml2 a l'autre 
      lrw = dgmax**2 + 9*dgmax + 22  
      lw=lwopt+lrw
c

      lneq=1
      liwopt=lneq+3+(dgmax+1)*(dgmax+2)
      lntb =liwopt + 20+dgmax
      lifree=lntb+mxsol
C

      minmax = -1
      neq = 1
      neqbac = 1
      iback=0
c
      iw(lneq)=neq
      iw(lneq+1)=ng
      iw(lneq+2)=dgmax
c
      w(ltq)=0.99990d+0
      w(ltq+1)=1.0d+0
      ltg=ltq+2
      call dcopy(ng+1,tg,1,w(ltg),1)
C
      if (info .gt. 0) call outl2(51,neq,neq,xx,xx,x,x)
      do 120 icomp = 1,50
        if (minmax .eq. -1) then
          nch = 1
          call optml2(feq,jacl2,iw(lneq),w(ltq),nch,w(lwopt),
     $         iw(liwopt))
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

          nch = 2
          call optml2(feq,jacl2,iw(lneq),w(ltq),nch,w(lwopt),
     $         iw(liwopt))
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

          minmax = 1
        else
          nch = 1
          call optml2(feqn,jacl2n,iw(lneq),w(ltq),nch,w(lwopt),
     $         iw(liwopt))
          if (info .gt. 1) then
             call lq(neq,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(neq,x,w(lw),1)
             call outl2(nch,neq,neqbac,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),neq,w(ltg),ng,w(lw)))
             lqdot=lw
             call feqn(iw(lneq),t,w(ltq),w(lqdot))
             call outl2(17,neq,neq,w(ltq),w(lqdot),phi0,x)
          endif
          nch = 2
          call optml2(feqn,jacl2n,iw(lneq),w(ltq),nch,w(lwopt),
     $         iw(liwopt))
          if (info .gt. 0) then
             call lq(neq,w(ltq),w(lw),w(ltg),ng)
             x=sqrt(gnrm)
             call dscal(neq,x,w(lw),1)
             call outl2(nch,neq,neqbac,w(ltq),w(lw),x,x)

             phi0= abs(phi(w(ltq),neq,w(ltg),ng,w(lw)))
             lqdot=lw
             call feqn(iw(lneq),t,w(ltq),w(lqdot))
             call outl2(17,neq,neq,w(ltq),w(lqdot),phi0,x)
          endif

          minmax = -1
        endif
        if (abs(w(ltq)) .gt. 1.0d+0) goto 140
        if (minmax .eq. 1) then
          if (icomp .eq. 1) then
            imin = 1
            ta(imin,1) = w(ltq)
            ta(imin,2) = phi(w(ltq),neq,tg,ng,w(lwopt))
          else
            call storl2(neq,w(ltq),w(ltg),ng,imin,ta,iback,iw(lntb),
     &                  w(ltback),nch,mxsol,w(lwopt),ierr)
            if (ierr .gt. 0) return
          endif
        endif
        w(ltq) = w(ltq) - 0.000010d+0
 120  continue
C
 140  if (info .gt. 0) then
        x = real(mxsol)
        call outl2(52,neq,imin,ta,xx,x,x)
      endif
C
      return
      end

