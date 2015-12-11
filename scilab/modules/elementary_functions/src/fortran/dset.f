c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
