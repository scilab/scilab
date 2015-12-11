c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1986 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=DTILD,SSI=0
c
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
