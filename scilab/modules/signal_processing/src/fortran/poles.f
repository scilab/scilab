C/MEMBR ADD NAME=POLES,SSI=0
      subroutine poles(ordre,fc,poler,polei)
c!
c sous-programme : poles
c calcul des poles de la fonction de transfert
c calcul en double precision
c ecrit par gregoire de nanteuil le 08-10-1985
c
c
c                           parametres entrants
c       ordre : ordre du filtre
c
c          fc : frquence de coupure
c
c                           parametres sortants
c      poler  : tableau des parties reelles des poles
c                de la fonction de transfert.
c
c      polei  : tableau des parties imaginaires des poles
c                de la fonction de transfert.
c
c                           variables internes
c      i
c!
      double precision pi,fc
      double precision poler(*),polei(*)
      integer ordre,i
c
      pi=3.1415926535897930d+0
c
      do 30 i=0,ordre-1
            poler(i+1)=-fc*sin(pi*(2*i+1)/(2*ordre))
            polei(i+1)=fc*cos(pi*(2*i+1)/(2*ordre))
30      continue
      return
      end
