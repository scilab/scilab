C/MEMBR ADD NAME=DPRXC,SSI=0
c     Copyright INRIA
      subroutine dprxc(n,roots,coeff)
c!but
c     dpxrc calcule les coefficients d'un polynome defini par ses
c     racines (reelles) et dont le coefficient de degre maximum est 1
c
c!liste d'appel
c      subroutine dpxrc(n,roots,coeff)
c     double precision roots(n),coeff(n+1)
c     integer n
c
c     n     : degre du polynome
c     roots : tableau contenant les racines
c     coeff : tableau contenant les coefficients du polynome, ranges
c             par odre croissant
c!sous programmes appeles
c     dset daxpy (blas)
c!origine
c     serge Steer INRIA
c!
      double precision roots(n),coeff(*)
      integer n
c
      integer j,nj, ninf
      double precision dlamch
c
      ninf=0
      call dset (n,0.0d+0,coeff,1)
      coeff(n+1)=1.0d+0
c
      do 10 j=1,n
         if(abs(roots(j)).gt.dlamch('o')) then
c     .    infinite roots gives zero high degree coeff
            ninf=ninf+1
         else
            nj=n+1-j
            call daxpy(j,-roots(j),coeff(nj+1),1,coeff(nj),1)
         endif
 10   continue
      if (ninf.gt.0) then
         call unsfdcopy(n-ninf+1,coeff(ninf+1),1,coeff(1),1)
         call dset(ninf,0.0d0,coeff(n-ninf+2),1)
      endif
c
      return
      end
