C/MEMBR ADD NAME=WVMUL,SSI=0
c     Copyright INRIA
      subroutine wvmul(n,dxr,dxi,incx,dyr,dyi,incy)
c!but
c
c     etant donne un vecteur dx et un vecteur dy complexes,
c      cette subroutine fait:
c                    dy = dy * dx
c     quand les deux increments sont egaux a un, cette
c     subroutine emploie des boucles "epanouis". dans le cas ou
c     les increments sont negatifs, cette subroutine prend
c     les composantes en ordre inverse.
c
c!liste d'appel
c
c     subroutine wvmul(n,dxr,dxi,incx,dyr,dyi,incy)
c
c     dy, dx: vecteurs double precision.
c
c     incx, incy: increments entre deux composantes succesives
c     des vecteurs.
c
c!auteur
c
c     serge steer inria
c!
      double precision dxr(*),dxi(*),dyr(*),dyi(*),sr
      integer i,incx,incy,n
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
        sr=dyr(iy)
        dyr(iy)=sr*dxr(ix) - dyi(iy)*dxi(ix)
        dyi(iy)=sr*dxi(ix) + dyi(iy)*dxr(ix)
        ix = ix + incx
        iy = iy + incy
   10 continue
      return
c
c code for both increments equal to 1
c
   20 continue
      do 30 i = 1,n
        sr=dyr(i)
        dyr(i)=sr*dxr(i) - dyi(i)*dxi(i)
        dyi(i)=sr*dxi(i) + dyi(i)*dxr(i)
   30 continue
c
      end
