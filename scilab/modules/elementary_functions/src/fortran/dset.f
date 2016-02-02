c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine dset(n,dx,dy,incy)
c!but
c     dset affecte un scalaire a tous les elements d'un vecteur
c!liste d'appel
c     subroutine dset(n,dx,dy,incy)
c     double precision dx,dy(n*incy)
c     integer n,incy
c
c     n : nombre d'elements du vecteur dy
c     dx : scalaire a affecter
c     dy : tableau contenant le vecteur
c     incy : increment entre deux elements consecutifs du vecteur y
c              dans le tableau dy
c
      double precision dx,dy(*)
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
