C/MEMBR ADD NAME=FREQUE,SSI=0
      subroutine freque(fmin,fmax,atmin,atmax,ordre,fc,err)
c!
c sous-programme : frequen
c calcul de l'ordre et de la frequence de coupure du filtre
c calcul en double precision
c ecrit par gregoire de nanteuil le 08-10-1985
c
c
c                           parametres entrants
c       fmin : frequence de fin de bande passante [0.;0.5]
c
c       fmax : frequence de debut de bande affaiblie [0.;0.5]
c
c      atmin : amplitude crete a crete des ondulations
c                         en bande passante en db
c
c      atmax : amplitudes des ondulations en bande affaiblie en db
c
c                           parametres sortants
c      ordre : ordre du filtre
c
c         fc : frequence de coupure
c
c      err   : variable contenant le type d"erreur rencontree
c!
      double precision fmin,fmax
      double precision fc
      integer atmin,atmax,ordre,err
c
      ordre=nint(log10((10**(atmax/10.0d+0)-1)
     *  /(10**(atmin/10.0d+0)-1))/(2.0d+0*log10(fmax/fmin)))
      if (ordre.le.0) err=36
      if (err.gt.0) return
      fc=fmax/(10**((log10(10**(atmax/10.0d+0)-1))/(2*ordre)))
      if (fc.le.0) err=36
      if (err.gt.0) return
100      continue
      return
      end
