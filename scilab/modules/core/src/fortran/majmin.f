C/MEMBR ADD NAME=MAJMIN,SSI=0
c     Copyright INRIA
      subroutine majmin( nc, chain1, chain2)
c
c ====================================================================
c scilab . librairie system
c ====================================================================
c
c        conversion en minuscule d'une chaine en majuscule
c
c ====================================================================
c
c entree : nc      nombre de caracteres des chaines
c          chain1  chaine a convertir
c
c
c sortie : chain2  chaine convertie
c
c
c reference fortran : index len min
c
c ============================================ (   ) ===========
c
      integer        nc
      character*(*)  chain1,chain2
c
      integer        i,i1,n1
      character*26   minus,majus
      data minus / 'abcdefghijklmnopqrstuvwxyz' /
      data majus / 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' /
c
      n1 = min ( nc, len(chain1) )
      if ( n1.le.0 ) goto 100
      do 10 i=1,n1
         i1 = index ( majus , chain1(i:i) )
         if ( i1.eq.0 ) then
            chain2(i:i) = chain1(i:i)
         else
            chain2(i:i) = minus(i1:i1)
         endif
   10 continue
c
  100 continue
      end
