      subroutine isbrk(l)
c ====================================================================
c
c       retourne la valeur de l'indicateur d'interruption
c
c ===================================== ( Inria    ) ==========
c
c     Copyright INRIA
      logical iflag,interruptible
      common /basbrk/ iflag,interruptible
      l=0
      if(iflag) l=1
      return
      end
