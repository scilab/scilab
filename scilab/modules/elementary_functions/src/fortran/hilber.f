C/MEMBR ADD NAME=HILBER,SSI=0
c     Copyright INRIA
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
