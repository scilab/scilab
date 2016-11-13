c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - M Cardelli L Baratchart INRIA Sophia-Antipolis 1989
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine arl2a(f,nf,ta,mxsol,imina,nall,inf,ierr,ilog,w,iw)
C!but
C     Cette procedure a pour but de rechercher le plus
C     grand nombre d'approximants pour chaque degre en partant
C     du degre 1 jusqu'a l'ordre nall.
C!liste d'appel
C     subroutine arl2a(f,nf,ta,nta,nall,info,ierr,io)
C     double precision ta(mxsol,0:nall),f(nf),w(*)
C     integer iw(*)
C
C     entrees
C      f : vecteur des coefficients de Fourier
C      nf : nombre de coefficients de Fourrier maxi 200
C      nall: degre des polynomes minimums que l'on veut  atteindre.
C      inf : impression de la progression de l'algorithme:
C            0 = rien
C            1 = resultats intermediaires et messages d'erreur
C            2 = suivi detaille
C      ilog : etiquette logique du fichier ou sont ecrite ces informations
C
C      sorties
C       ta :tableau contenant les minimums  locaux a l'ordre nall
C       imina : nombre de minimums trouves
C       ierr. contient l'information sur le deroulement du programme
C          ierr=0 : ok
C          ierr=1 : trop de coefficients de fourrier (maxi 200)
C          ierr=2 : ordre d'approximation trop eleve
C          ierr=3 : boucle indesirable sur 2 ordres
C          ierr=4 : plantage lsode
C          ierr=5 : plantage dans recherche de l'intersection avec une face
C          ierr=7 : trop de solutions
C
C      tableaux de travail
C      w: 34+34*nall+7*ng+nall*ng+nall**2*(ng+2)+4*(nall+1)*mxsol
C      iw :29+nall**2+4*nall+2*mxsol
      implicit double precision (a-h,o-y)
      dimension ta(mxsol,*), f(nf), w(*), iw(*), x(1)
      integer dgmax
C
cDEC$ ATTRIBUTES DLLIMPORT:: /sortie/
cDEC$ ATTRIBUTES DLLIMPORT:: /no2f/
cDEC$ ATTRIBUTES DLLIMPORT:: /comall/
      common /sortie/ io,info,ll
      common /no2f/ gnrm
      common /comall/ nall1

C     decoupage du tableau de travail w
      dgmax=nall
      ncoeff=nf
      ng=nf-1
      ldeg  =1
      ltb   = ldeg + 33+33*dgmax+7*ng+dgmax*ng+dgmax**2*(ng+2)
      ltc = ltb + (nall+1)*mxsol
      ltback = ltc + (nall+1)*mxsol
      lter = ltback + (nall+1)*mxsol
      ltq = ltback + (nall+1)*mxsol
      lfree = ltq + nall + 1
C
C     decoupage du tableau de travail iw
      ildeg = 1
      ilntb = ildeg +29+dgmax**2+4*dgmax
      ilnter = ilntb + mxsol
      ilfree = ilnter + mxsol
C     initialisations
      io = ilog
      ll = 80
      info = inf
      nall1 = nall
C
C test validite des arguments
C
      ng = nf - 1
      gnrm = dnrm2(nf,f,1)
      call dscal(nf,1.0d+0/gnrm,f,1)
      gnrm = gnrm**2
C
C
      iback = 0
C
      call deg1l2(f,ng,imina,ta,mxsol,w(ldeg),iw(ildeg),ierr)
      if (ierr .gt. 0) return
      if (nall .eq. 1) goto 400
      neq = 1
C
      do 200 ideg = 2,nall
        call degl2(f,ng,neq,imina,iminb,iminc,ta,w(ltb),w(ltc),iback,
     &             iw(ilntb),w(ltback),mxsol,w(ldeg),iw(ildeg),ierr)
        if (ierr .gt. 0) return
C
        if (imina .eq. 0) goto 201
C
 200  continue
C
 201  if (info .gt. 1) call outl2(23,neq,iback,x,x,tt,tt)
C
      if (iback .gt. 0) then
        imina = 0
        neq = iw(ilntb)
        inf = 1
        do 300 ideg = neq,nall-1
C
          do 250 j = inf,iback
            ntbj = iw(ilntb+j-1)
            if (ntbj .eq. neq) then
              call dcopy(ntbj,w(ltback-1+j),mxsol,w(ltq),1)
              w(ltq+ntbj) = 1.0d+0
C
              nch = 1
C	remplacement de tq par w(ltq) tq n'est pas defini
              call storl2(neq,w(ltq),f,ng,imina,ta,iback,iw(ilnter),
     &             w(lter),nch,mxsol,w(ldeg),ierr)
            else
              inf = j
              goto 260
            endif
 250      continue
C
 260      continue
          call degl2(f,ng,neq,imina,iminb,iminc,ta,w(ltb),w(ltc),iback,
     &         iw(ilnter),w(lter),mxsol,w(ldeg),iw(ildeg),ierr)
          if (ierr .gt. 0) return
C
 300    continue
      endif
C
 400  continue
C
      return
      end

