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
