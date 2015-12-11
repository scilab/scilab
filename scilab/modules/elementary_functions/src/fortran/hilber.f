c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=HILBER,SSI=0
c
      subroutine hilber(a,lda,n)
      double precision a(lda,n)
c!but
c     hilber genere l'inverse de la matrice de hilbert
c!liste d'appel
c      subroutine hilber(a,lda,n)
c     double precision a(lda,n)
c
c     a : tableau contenant apres execution l'inverse de la matrice
c         de hilbert de dimension n
c     lda : nombre de ligne de a dans le programme appelant
c     n : dimension de la matrice de hilbert
c!
      double precision p,r
      p = dble(n)
      do 20 i = 1, n
        if (i.ne.1) p = (dble(n-i+1)*p*dble(n+i-1))/dble(i-1)**2
        r = p*p
        a(i,i) = r/dble(2*i-1)
        if (i.eq.n) go to 20
        ip1 = i+1
        do 10 j = ip1, n
          r = -(dble(n-j+1)*r*(n+j-1))/dble(j-1)**2
          a(i,j) = r/dble(i+j-1)
          a(j,i) = a(i,j)
   10   continue
   20 continue
      return
      end
