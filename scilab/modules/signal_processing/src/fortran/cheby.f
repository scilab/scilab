C/MEMBR ADD NAME=CHEBY,SSI=0
      subroutine cheby(ordr,demi,ieo,dp,x0,tam,win)
c!
c sous-programme: cheby
c fenetre de dolph chebyshev
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c                  parametres entrants
c                 -------------------
c ordr, l'ordre du filtre (entier)
c demi, l'ordre de la demi fenetre (entier)
c ieo, indicateur de parite vaut 0 si ordr pair et 1 sinon (entier)
c dp, attenuation en absolu (reelle)
c x0, constante de la fenetre de chebyshev fonction de
c      la largeur du lobe principale (reelle)
c tam, tableau de travail (reels) qui doit etre dans un programme
c    appelant dimensionne a 3 fois ordr)
c
c                  parametres sortants
c                 -------------------
c win, tableau de valeurs de la fenetre (reelles) qui doit dans
c    un programme appelant etre dimensionne a ordr
c
c                  variables internes
c                 ------------------
c c0,c1,c2, des buffers de calcul (reels)
c compt,indi,xcompt,xindi, compteurs de boucles (entiers et reels)
c teta,gama, coefficients de la fenetres de chebyshev (reels)
c
c sous programmes appele: coshin
c
c!
      double precision win(*),tam(*)
      double precision x0,dp,xordr,teta,gama,freq
      double precision pi,twopi,twn,c0,c1,c2,somm,xcompt,xindi
      double precision coshin
      integer ordr,demi,compt,indi,icompt,jcompt
c
      xordr=dble(ordr)
      teta=(x0+1.)/2.0d+0
      gama=(x0-1.)/2.0d+0
      pi=acos(-1.0d+0)
      twopi=2.0d+0*pi
      c0=(xordr-1.0d+0)/2.0d+0
      do 40 compt=1,ordr
      icompt=ordr+compt
      jcompt=icompt+ordr
      xcompt=dble(compt)-1.0d+0
      c1=xcompt/xordr
      freq=teta*cos(twopi*c1)+gama
      CRES=abs(freq)-1.0d+0
      if (CRES .le. 0) then
         goto 10
      else
         goto 20
      endif
10    tam(icompt)=dp*cos(c0*acos(freq))
      goto 30
20    tam(icompt)=dp*cosh(c0*coshin(freq))
30    tam(jcompt)=0.0d+0
c
c modification si filtre d'ordr pair
c
      if(ieo.eq.1)goto 40
      tam(jcompt)=-tam(icompt)*sin(pi*c1)
      tam(icompt)= tam(icompt)*cos(pi*c1)
      if(compt.gt.(ordr/2+1))then
      tam(icompt)=-tam(icompt)
      tam(jcompt)=-tam(jcompt)
      endif
40    continue
c
c transformee de fourrier inverse
c
      twn=twopi/xordr
      do 60 compt=1,demi
      xcompt=dble(compt)-1.0d+0
      somm=0.0d+0
      do 50 indi=1,ordr
      icompt=indi+ordr
      jcompt=icompt+ordr
      xindi=dble(indi)-1.0d+0
      somm=somm+tam(icompt)*cos(twn*xindi*xcompt)
     & +tam(jcompt)*sin(twn*xindi*xcompt)
50    continue
      win(compt)=somm
60    continue
      c2=win(1)
      do 70 compt=1,demi
      win(compt)=win(compt)/c2
70    continue
      return
      end
