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
      integer j,nj
c
      call dset (n,0.0d+0,coeff,1)
      coeff(n+1)=1.0d+0
c
      do 10 j=1,n
      nj=n+1-j
      call daxpy(j,-roots(j),coeff(nj+1),1,coeff(nj),1)
   10 continue
c
      return
      end
