      subroutine drref(a,lda,m,n,eps)
c!but
c     drref calcule la forme echelon d'une matrice
c!liste d'appel
c
c      subroutine drref(a,lda,m,n,eps)
c     double precision a(lda,n),eps
c     integer lda,m,n
c
c     a: tableau contenant a l'appel la matrice dont on veut determiner
c        la forme echelon, apres execution a contient la forme echelon
c     lda : nombre de ligne du tableau a dans le programme appelant
c     m : nombre de ligne de la matrice a
c     n : nombre de colonnes de a matrice a
c     eps : tolerance. les reels  inferieurs a 2*max(m,n)*eps*n1(a),
c           ou n1(a) est la norme 1 de a ,sont consideres comme nuls
c
c     si l'on veut la transformation appliquee,appeler drref avec
c     la matrice obtenue en concatenant l'identite a la matrice a
c     en rajoutant des colonnes.
c!sous programmes appeles
c     idamax dcopy dswap dscal dasum daxpy (blas)
c     dble (fortran)
c!
c!
c     Copyright INRIA
      double precision a(lda,*),eps,tol,t,dasum
      tol = 0.0d+0
      do 10 j = 1, n
         tol = max(tol,dasum(m,a(1,j),1))
   10 continue
      tol = eps*dble(2*max(m,n))*tol
      k = 1
      l = 1
   20 if (k.gt.m .or. l.gt.n) return
      i = idamax(m-k+1,a(k,l),1) + k-1
      if (abs(a(i,l)) .gt. tol) go to 30
      call dcopy(m-k+1,0.0d+0,0,a(k,l),1)
         l = l+1
         go to 20
   30 call dswap(n-l+1,a(i,l),lda,a(k,l),lda)
      t=1.0d+0/a(k,l)
      call dscal(n-l+1,t,a(k,l),lda)
      a(k,l) = 1.0d+0
      do 40 i = 1, m
         t = -a(i,l)
         if (i .ne. k) call daxpy(n-l+1,t,a(k,l),lda,a(i,l),lda)
   40 continue
      k = k+1
      l = l+1
      go to 20
      end
