C/MEMBR ADD NAME=CHEBC,SSI=0
      subroutine chebc(ordr,dp,beta,x0,er)
c
c!
c sous-programme:  chebc
c sert a generer les parametres de la fenetre de chebyschev lorsqu'un de
c ceux-ci est manquant ( un et un seul ! )
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c
c                  parametres entrants ou sortants
c                 -------------------------------
c ordr, ordr du filtre (entier)
c beta, la largeur du lobe principal (reelle variant entre 0.et 1.)
c dp, attenuation en absolu calculee a partir de alph (reelle)
c ces trois parametres ne peuvent etre nuls simultanements
c
c                  parametre sortant
c                 -----------------
c x0, constante de la fenetre  de chebyschev (reelle)
c     =(3-c0)/(1+c0) avec c0=cos(2*pi*beta)
c er, flag erreur (entier)
c
c                  variables internes
c                 ------------------
c xn=ordr-1
c c0, c1 buffers de calcul (reels)
c sous programmes appele: coshin
c
c!
c
      double precision pi,dp,beta
      double precision x0,xn,c1,c0,xordr
      double precision coshin
      integer ordr,er
      pi=acos(-1.0d+0)
c
c dp et beta  sont specifies, on determine l'ordr
c
      if(ordr.ne.0)goto 10
      if(dp.eq.0.0d+0.or.beta.eq.0.0d+0)er=7
      if(er.gt.0)goto 40
      c1=coshin((1.0d+0+dp)/dp)
      c0=cos(pi*beta)
      xordr=1.0d+0+c1/coshin(1.0d+0/c0)
      ordr=int(xordr)+1
      goto 30
c
c ordr et dp sont specifies, on determine beta
c
10    if(beta.ne.0.0d+0)goto 20
      if(dp.eq.0)er=7
      if(er.gt.0)goto 40
      xn=dble(ordr)-1.0d+0
      c1=coshin((1.0d+0+dp)/dp)
      c0=cosh(c1/xn)
      beta=(acos(1.0d+0/c0))/pi
      goto 30
c
c ordr et beta sont specifies, on determine dp
c
20    xn=dble(ordr)-1.0d+0
      c0=cos(pi*beta)
      c1=xn*coshin(1.0d+0/c0)
      dp=1.0d+0/(cosh(c1)-1.0d+0)
30    x0=(3.0d+0-cos(2.0d+0*pi*beta))/(1.0d+0+cos(2.0d+0*pi*beta))
40    continue
      return
      end
