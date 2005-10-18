C/MEMBR ADD NAME=DADD,SSI=0
      subroutine  mdadd(m,n,dx,incx,dy,incy)
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
c      subroutine  dmpadd(n,dx,incx,dy,incy)
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
      double precision dx(*),dy(*)
      integer i,incx,incy,ix,iy,n,m
c      write(*,*)'mdadd.f'
c
c
c      write(*,*)m
      if(m.eq.1) then
      call mpdadd(n,dx,incx,dy,incy)
      else
      call mndadd(n,dx,incx,dy,incy)
      endif
      return
      end
