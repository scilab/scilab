      subroutine  dadd(n,dx,incx,dy,incy)
c!but
c
c     cette subroutine ajoute le vecteur x, de taille n, au
c     vecteur y. (y=y+x)
c     dans le cas de deux increments egaux a 1, cette subroutine
c     emploie des boucles "epanouies".
c     dans le cas ou les increments sont negatifs cette
c     subroutine prend les composantes en ordre inverse.
c
c!liste d'appel
c
c      subroutine  dadd(n,dx,incx,dy,incy)
c
c     n: taille du vecteur x
c
c     dx: vecteur double precision contenant x
c
c     dy: vecteur double precision contenant y
c
c     incx, incy: increments entre les composantes des vecteurs.
c
c!
c
c     Copyright INRIA
      double precision dx(*),dy(*)
      integer i,incx,incy,ix,iy,n
c
      if(n.le.0)return
      if(incx.eq.1.and.incy.eq.1)go to 20
c
c code for unequal increments or equal increments not equal to 1
c
      ix = 1
      iy = 1
      if(incx.lt.0)ix = (-n+1)*incx + 1
      if(incy.lt.0)iy = (-n+1)*incy + 1
      do 10 i = 1,n
        dy(iy) = dx(ix)  + dy(iy)
        ix = ix + incx
        iy = iy + incy
   10 continue
      return
c
c code for both increments equal to 1
c
   20 continue
      do 30 i = 1,n
        dy(i) = dx(i) + dy(i)
   30 continue
c
      end
