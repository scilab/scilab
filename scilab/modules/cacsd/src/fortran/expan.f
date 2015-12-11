
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - F Delebecque
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

       subroutine expan(a,la,b,lb,c,nmax)
c! but
c calcul des nmax premiers coefficients de la longue division de
c  b par a .On suppose a(1) non nul.
c!liste d'appel
c       subroutine expan(a,la,b,lb,c,nmax)
c  a vecteur de longueur la des coeffs par puissances croissantes
c  b   "           "     lb        "                "          "
c  c                     nmax   des coeffs de  a/b
c
      dimension a(la),b(lb),c(nmax)
       double precision a,b,c,s,a0
c
       m=la
       n=lb
       a0=a(1)
       if(a0.eq.0.0d+0)  return
       k=1
   2   continue
       s=0.0d+0
       if(k.eq.1) goto 8
       j=1
   5   continue
       j=j+1
       if(j.gt.min(m,k)) goto 8
       s=s+a(j)*c(k-j+1)
       goto 05
   8   continue
       if(k.le.n) then
       c(k)=(b(k)-s)/a0
       else
       c(k)=-s/a0
       endif
       if(k.eq.nmax) return
       k=k+1
       goto 2
       end
