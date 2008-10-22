C/MEMBR ADD NAME=COEFT,SSI=0
      subroutine coeft(ordre,poler,polei,gain)
c!
c sous-programme : coeft
c calcul du gain de la fonction de transfert
c et des coefficients de la decomposition en elements simples
c en double precision
c ecrit par gregoire de nanteuil le 08-10-1985
c
c
c                           parametres entrants
c      poler  : tableau des parties reelles des poles
c                de la fonction de transfert.
c
c      polei  : tableau des parties imaginaires des poles
c                de la fonction de transfert.
c
c                           parametres sortants
c
c      gain : coefficient de la fonction de transfert (gain)
c
c                           variables internes
c      zr,zi,pre,pim,i,j,ordre
c!
      double precision gain
      double precision poler(*),polei(*)
      double precision zr,zi
      integer ordre,i
c
      zr=1.0d+0
      zi=0.0d+0
      do 40 i=1,ordre
            pre=zr*(-poler(i))-zi*(-polei(i))
            pim=zi*(-poler(i))+zr*(-polei(i))
            zr=pre
            zi=pim
40      continue
      gain=zr
50      continue
      return
      end
