C/MEMBR ADD NAME=INO,SSI=0
      double precision function ino(x)
c!
c fonction:  ino
c fonction de bessel pour la fenetre de kaiser
c en double precision
c acheve 05/12/85
c ecrit par philippe touron
c
c
c                  paramatres entrants
c                 -------------------
c x, variable reelle
c
c                  parametres sortants
c                 -------------------
c ino(x), valeur de la fonction de bessel (reelle)
c
c                  variables internes
c                 ------------------
c y, variable reelle egale a x/2
c incr, increment reel de la fonction de bessel
c term, valeur reelle du terme general de la serie de bessel
c somm, fonction de bessel a la n-ieme iteration (reel)
c prec, si le rapport (term/somm) < 1.0**-8, le calcul est stoppe !!
c compt, compteur de boucle (entier)
c xcompt, valeur de compt en double precision
c
c sous programmes appele: aucun
c!
      double precision x,y,prec,somm,incr,term,xcompt
      integer compt
c
      y=x/2.0d+0
      prec=0.000000010d+0
      somm=1.0d+0
      incr=1.0d+0
      do 10 compt=1,25
      xcompt=dble(compt)
      incr=incr*y/xcompt
      term=incr*incr
      somm=somm+term
      if ((somm*prec-term) .le. 0) then
         goto 10
      else
         goto 20
      endif
10    continue
20    ino=somm
      return
      end
