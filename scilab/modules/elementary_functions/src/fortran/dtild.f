C/MEMBR ADD NAME=DTILD,SSI=0
c     Copyright INRIA
      subroutine  dtild(n,x,incx)
c!but
c
c     cette subroutine inverse l'ordre des elements d'un
c     vecteur x
c
c!liste d'appel
c
c      subroutine  dtild(n,x,incx)
c
c     n: taille du vecteur dx
c
c     x: double precision, vecteur
c
c     incx: increment entre les composantes du vecteur.
c
c!auteur
c
c     serge Steer Inria 1986
c
c!
c
      double precision x(*),xx
      integer i,incx,i1,i2,n
c
      if(n.le.1)return
      i1=1
      i2=n*incx
      do 10 i=1,n/2
      xx=x(i1)
      x(i1)=x(i2)
      x(i2)=xx
      i1=i1+incx
      i2=i2-incx
   10 continue
      return
      end
