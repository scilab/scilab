C/MEMBR ADD NAME=COSHIN,SSI=0
c
c fonction:  coshin
c fonction cosinus hyperbolique inverse de x
c en double precision
c acheve le 05/12/85
c ecrit par philippe touron
c
c
c sous programmes appeles: aucun
c
c
c     Copyright INRIA
      double precision function coshin(x)
      double precision x
      coshin=log(x+sqrt(x*x-1.0d+0))
      return
      end
