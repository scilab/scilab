C/MEMBR ADD NAME=WPRXC,SSI=0
c     Copyright INRIA
      subroutine wprxc(n,rootr,rooti,coeffr,coeffi)
c!but
c     wprxc calcule les coefficients d'un polynome defini par ses
c     racines (complexes) et dont le coefficient de degre maximum est 1
c
c!liste d'appel
c      subroutine wprxc(n,rootr,rooti,coeffr,coeffi)
c     double precision rootr(n),rooti(n),coeffr(n+1),coeffi(n+1)
c     integer n
c
c     n     : degre du polynome
c     root : tableau contenant les racines
c     coeff : tableau contenant les coefficients du polynome, ranges
c             par odre croissant
c!sous programmes appeles
c     dset waxpy (blas)
c!origine
c     serge Steer INRIA
c!
      double precision rootr(n),rooti(n),coeffr(*),coeffi(*)
      integer n
c
      integer j,nj
c
      call dset (n,0.0d+0,coeffr,1)
      call dset (n+1,0.0d+0,coeffi,1)
      coeffr(n+1)=1.0d+0
c
      do 10 j=1,n
      nj=n+1-j
      call waxpy(j,-rootr(j),-rooti(j),coeffr(nj+1),coeffi(nj+1),1,
     & coeffr(nj),coeffi(nj),1)
   10 continue
c
      return
      end
