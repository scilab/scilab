C/MEMBR ADD NAME=FILBUT,SSI=0
      subroutine filbut(fmin,fmax,atmin,atmax,
     *  ordre,fc,gain,poler,polei,err)
c!
c sous-programme : filbut
c filtre a reponse impulsionnelle infinie
c calcul en double precision
c ecrit par gregoire de nanteuil le 08-10-1985
c
c
c                           parametres entrants
c    par l"utilisateur :
c
c      fmin : frequence de fin de bande passante.
c
c      fmax : frequence de debut de bande affaiblie.
c
c      atmin : amplitude crete a crete des ondulations
c            en bande passante en db .
c
c      atmax : amplitude des ondulations en bande affaiblie en db .
c
c
c                           parametres sortants
c      ordre : ordre du filtre.
c
c      fc    : frequence de coupure du filtre.
c
c      gain  : gain de la fonction de transfert.
c
c      poler  : tableau des parties reelles des poles
c                de la fonction de transfert.
c
c      polei  : tableau des parties imaginaires des poles
c                de la fonction de transfert.
c
c      err    : variable contenant le type d"erreur rencontree
c
c      subroutines appelees frequen, poles, coeft
c!
      double precision fmin,fmax
      double precision fc,gain
      double precision poler(*),polei(*)
      integer atmin,atmax,ordre,err
c
1      if (fmin.ge.fmax) err=36
      if (err.gt.0) return
      if ((fmin.le.0.).or.(fmax.ge.1.)) err=36
      if (err.gt.0) return
      if (atmin.ge.atmax) err=36
      if (err.gt.0) return
      if (atmin.le.0) err=36
      if (err.gt.0) return
10      continue
c
      call freque(fmin,fmax,atmin,atmax,ordre,fc,err)
      call poles(ordre,fc,poler,polei)
      call coeft(ordre,poler,polei,gain)
100     continue
      return
      end
