
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - F Delebecque
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

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
