C/MEMBR ADD NAME=HAMMIN,SSI=0
      subroutine hammin(ordr,demi,ieo,alph,win)
c!
c sous-programme:  hammin
c fenetre de hamming generalisee
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c                  parametres entrants
c                 -------------------
c demi, ordre de la demi-fenetre (entier)
c ordr, ordre du filtre (entier)
c ieo, indicateur de parite (entier)
c alph, 1ere constante de la fenetre de hamming (reelle)
c
c                  parametres sortants
c                 -------------------
c win, les valeurs de la demi-fenetre (tableau de reels qui doit
c    dans un programme appelant etre dimensionne a ordr)
c
c                  variables internes
c                 ------------------
c xcompt,ycompt,compt, compteur de boucle et indice de tableau en reel
c         (resp en entier)
c
c sous programmes appeles: aucun
c!
      double precision win(*),pi,twopi,xcompt,ycompt,alph,beta
      integer ordr,compt,demi,ieo
c
      pi=acos(-1.0d+0)
      twopi=2.0d+0*pi
      beta=1.0d+0-alph
      ycompt=dble(ordr)-1.0d+0
      do 10 compt=1,demi
      xcompt=dble(compt)-1.0d+0
      if(ieo.eq.0)xcompt=xcompt+0.50d+0
      win(compt)=alph+beta*cos((twopi*xcompt)/ycompt)
10    continue
      return
      end
