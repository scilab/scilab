C/MEMBR ADD NAME=ISET,SSI=0
c     Copyright INRIA
      subroutine iset(n,dx,dy,incy)
c!but
c     iset affecte un entier a tous les elements d'un vecteur
c!liste d'appel
c     subroutine iset(n,dx,dy,incy)
c     integer dx,dy(n*incy)
c     integer n,incy
c
c     n : nombre d'elements du vecteur dy
c     dx : scalaire a affecter
c     dy : tableau contenant le vecteur
c     incy : increment entre deux elements consecutifs du vecteur y
c              dans le tableau dy
c!
      integer dx,dy(*)
c
      if (n.le.0) return
      iy = 1
      if (incy.lt.0) iy = (-n+1)*incy + 1
      do 10 i = 1,n
        dy(iy) = dx
        iy = iy + incy
   10 continue
      return
      end
