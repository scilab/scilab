      subroutine storl2(neq,tq,tg,ng,imin,tabc,iback,ntback,tback,
     &     nch,mxsol,w,ierr)
C!but
C     Lorsque un minimum local vient d'etre determine, cette
C     procedure est appelee afin de verifier son originalite,
C     et si elle est effective, de le stocker dans le tableau
C     en construction, correspondant au degre de la recherche
C     en cours. S'il n'est pas de ce degre, il est alors range
C     dans le tableau 'tback' qui contient tout minimum origi-
C     nal obtenu apres une sortie de face.
C!liste d'appel
C     entrees :
C     - neq. est le degre du minimum nouvellement obtenu.
C     - tq. est le tableau contenant ses coefficients
C     - imin. est le nombre des minimums de meme degre,
C        deja reveles.
C     - tabc. etant le tableau contenant ces minimums.
C     - iback. est le nombre de minimums de degre
C        quelconque, reveles apres une sortie de face.
C     - ntback. est un tableau entier unicolonne contenant
C        les degres de ces polynomes.
C     - tback. est le tableau ou sont stockes ces polynomes.
C        Ainsi, le ieme polynome, de degre ntback(i), a
C        ses coeff dans la ieme ligne, c-a-d de tback(i,0)
C        a tback(i,ntback(i)-1).
C     - nch. est un parametre entier indiquant s'il s'agit
C        d'un minimum de meme degre que celui de la recherche
C        en cours, ou bien d'une sortie de face.
C
C     sorties :
C     - peuvent etre modifies: imin, tabc, iback, ntback,
C        tback, suivant le tableau ou a ete stocke le minimum tq
c
c     Copyright INRIA
c
C!
      implicit double precision (a-h,o-y)
      dimension tq(0:*), tabc(mxsol,0:*), ntback(iback),
     &          tback(mxsol,0:*), xx(1),tg(ng+1),w(*)
C
      common /sortie/ io,info,ll
C
      ierr = 0
      if (nch .lt. -2) goto 200
      if (imin .eq. 0) goto 400
C
C     ---- test sur l'originalite du nouveau min -----------------------
C
C     ---- par rapport a tabc.
C
      do 120 im = 1,imin
C
        diff0 = 0.0d+0
        do 110 ij = 0,neq-1
          diff0 = diff0 + (tq(ij)-tabc(im,ij))**2
 110    continue
        diff0 = sqrt(diff0)
C
        if (diff0 .lt. 1.0d-03) then
          if (info .gt. 0) call outl2(80,0,0,xx,xx,x,x)
          return
        endif
C
 120  continue
C
C     ---- par rapport a tback.
C
C     - Situation des polynomes de meme degre. -
C
 200  if (nch.lt.0 .and. iback.gt.0) then
        jsup = iback + 1
        jinf = 0
C
        do 210 j = iback,1,-1
          if (jsup.gt.j .and. ntback(j).gt.neq) jsup = j
 210    continue
        do 220 j = 1,iback
          if (jinf.lt.j .and. ntback(j).lt.neq) jinf = j
 220    continue
C
C     - Controle de l'originalite. -
C
        if ((jsup-jinf) .gt. 1) then
C
          do 240 j = jinf+1,jsup-1
C
            diff0 = 0.0d+0
            do 230 i = 0,neq-1
              diff0 = diff0 + (tq(i)-tback(j,i))**2
 230        continue
            diff0 = sqrt(diff0)
C
            if (diff0 .lt. 1.0d-03) then
              if (info .gt. 0) call outl2(80,0,0,xx,xx,x,x)
              return
            endif
C
 240      continue
        endif
      endif
C
C     -------- classement du nouveau minimum -----
C     ---- dans tback.
C
      if (iback .eq. mxsol) then
        ierr = 7
        return
      endif
      if (nch .lt. 0) then
C
        if (iback .eq. 0) then
C
          do 310 i = 0,neq-1
            tback(1,i) = tq(i)
 310      continue
          ntback(1) = neq
C
        elseif (jsup .gt. iback) then
C
          do 330 i = 0,neq-1
            tback(jsup,i) = tq(i)
 330      continue
          ntback(iback+1) = neq
C
          
        else
C
          do 350 j = iback,jsup,-1
            do 340 i = 0,ntback(j)-1
              tback(j+1,i) = tback(j,i)
 340        continue
            ntback(j+1) = ntback(j)
 350      continue
C
          do 370 i = 0,neq-1
            tback(jsup,i) = tq(i)
 370      continue
          ntback(jsup) = neq
C
          
        endif
C
        iback = iback + 1
        if (info .gt. 1) call outl2(81,neq,neq,xx,xx,x,x)
        return
C
      endif
C
C     -------- dans tabc.
 400  continue
      if (imin .eq. mxsol) then
        ierr = 7
        return
      endif
      paux = phi(tq,neq,tg,ng,w)
C
      if (imin .eq. 0) then
C
        do 410 ij = 0,neq-1
          tabc(1,ij) = tq(ij)
 410    continue
        tabc(1,neq) = paux
        imin = imin + 1
C
        
      else
C
        do 490 im = imin,1,-1
C
          if (paux.gt.tabc(im,neq) .and. im.eq.imin) then
C
            do 420 ij = 0,neq-1
              tabc(imin+1,ij) = tq(ij)
 420        continue
            tabc(imin+1,neq) = paux
            imin = imin + 1
            return
C
          elseif (paux .gt. tabc(im,neq)) then
C
            do 440 in = imin,im+1,-1
              do 430 ij = 0,neq
                tabc(in+1,ij) = tabc(in,ij)
 430          continue
 440        continue
            do 450 ij = 0,neq-1
              tabc(im+1,ij) = tq(ij)
 450        continue
            tabc(im+1,neq) = paux
            imin = imin + 1
            return
C
          elseif (im .eq. 1) then
C
            do 470 in = imin,1,-1
              do 460 ij = 0,neq
                tabc(in+1,ij) = tabc(in,ij)
 460          continue
 470        continue
            do 480 ij = 0,neq-1
              tabc(1,ij) = tq(ij)
 480        continue
            tabc(1,neq) = paux
            imin = imin + 1
C
          endif
C
 490    continue
C
        
      endif
C
      return
      end

