C/MEMBR ADD NAME=KAISER,SSI=0
      subroutine kaiser(ordr,demi,ieo,alph,win)
c!
c sous-programme:  kaiser
c fenetre de kaiser
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c                  parametres entrants
c                 -------------------
c ordr, ordre du filtre (entier)
c demi, ordre de la demi-fenetre (entier)
c ieo, indicateur de parite (entier)
c alph, attenuation absolue en db (reelle)
c
c                  parametres sortants
c                 -------------------
c win, les valeurs de la demi-fenetre (tableau de reels qui doit
c    dans un programme appelant etre dimensionne a ordr)
c
c                  variables internes
c                 ------------------
c xcomp,compt, compteur de boucle et indice de tableau en reel
c         (resp en entier)
c beta, calculee a partir de alph (reelle)
c xbuf, buffer de calcul
c bes, valeur (reelle)prise par la fonction de bessel au point beta
c
c sous programmes appele: ino
c!
      double precision win(*),ino,xcompt,xbuf,bes,beta,alph
      integer ordr,demi,ieo,compt
c
c calcul de beta=f(alph)
c
      if(alph.gt.50.0d+0)beta=0.11020d+0*(alph-8.70d+0)
      if(alph.ge.20.960d+0.and.alph.le.50.0d+0)beta=0.584170d+0
     &    *(alph-20.960d+0)**0.40d+0+0.078860d+0*(alph-20.960d+0)
      if(alph.lt.20.960d+0)beta=0.0d+0
      bes=ino(beta)
      xbuf=dble(ordr-1)*dble(ordr-1)
      do 10 compt=1,demi
      xcompt=dble(compt)-1.0d+0
      if(ieo.eq.0)xcompt=xcompt+0.50d+0
      xcompt=4.0d+0*xcompt*xcompt
      win(compt)=ino(beta*sqrt(1.0d+0-xcompt/xbuf))
      win(compt)=win(compt)/bes
10    continue
      return
      end
