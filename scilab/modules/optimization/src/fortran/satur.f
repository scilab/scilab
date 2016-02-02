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
      subroutine satur (n,x,binf,bsup,d,ttmin,ttsup,topt,tg,td,
     &                   tmi,icoi,icos,iproj)
c
c      subroutine calculant ,dans un intervalle donne, un pas proche
c      de tmi saturant une contrainte
c         topt:pas calculer (=0 s'il n'existe pas un tel pas         (s)
c        ttmin,ttsup:bornes de l'intervalle dans lequel doit
c         etre topt                                                  (e)
c        tmi:pas au voisinnage duquel on calcul topt                 (e)
c        iproj:indicateur de projection                              (e)
c             =0:on cherche un pas saturant une contrainte dans
c                 l'intervalle ttmin,ttsup
c             =1:on cherche un pas dans l'intervalle tg,td et on
c                le ramene dans l'intervalle ttmin,ttsup
c                par projection
c       icos:indice de la variable saturee par la borne superieure
c       icoi:indice de la variable saturee par la borne inferieure
c       inf:indicateur pour l initialisation de icoi et icos
c            =0:la borne superieure est atteinte
c            =1:la borne superieure est atteinte
c            =2:le pas est obtenu par projection sur ttmin ttsup
c
      implicit double precision(a-h,o-z)
      integer iproj
      dimension x(n),binf(n),bsup(n),d(n)
c
      icoi=0
      icos=0
      ep=tmi
c
c        boucle
      do 70 i=1,n
      inf=0
c        calcul du pas saturant la ieme contrainte:tb
      CRES=d(i)
      if (CRES .lt. 0) then
         goto 61
      elseif (CRES .eq. 0) then
         goto 70
      else
         goto 62
      endif
61    tb=(binf(i)-x(i))/d(i)
      inf=1
      go to 63
62    tb=(bsup(i)-x(i))/d(i)
63    if ((tb.gt.ttsup).or.(tb.lt.ttmin))then
c        projection de tb sur l intervalle ttmin,ttsup
        if ((iproj.eq.0).or.(tb.lt.tg).or.(tb.gt.td)) go to 70
        tb=max(tb,ttmin)
        tb=min(tb,ttsup)
        inf=2
      end if
c        recherche du pas le plus proche de tmi
      e=abs(tb-tmi)
      if (e.ge.ep) go to  70
      topt=tb
      ep=e
c        mise a jour de icoi,icos
      icoi=0
      icos=0
      if (inf.eq.0) icos=i
      if (inf.eq.1) icoi=i
70    continue
      return
      end
