C/MEMBR ADD NAME=WRREF,SSI=0
c     Copyright INRIA
      subroutine wrref(ar,ai,lda,m,n,eps)
c!but
c     wrref calcule la forme echelon d'une matrice a coeff complexes
c!liste d'appel
c
c     subroutine wrref(ar,ai,lda,m,n,eps)
c     double precision ar(lda,n),ai(lda,n),eps
c     integer lda,m,n
c
c     ar,ai : tableaux contenant a l'appel les parties reelles et
c        complexes  de la matrice dont on veut determiner la forme
c        echelon, apres execution a contient la forme echelon
c     lda : nombre de ligne du tableau a dans le programme appelant
c     m : nombre de ligne de la matrice a
c     n : nombre de colonnes de a matrice a
c     eps : tolerance. les reels  inferieurs a 2*max(m,n)*eps*n1(a),
c           ou n1(a) est la norme 1 de a ,sont consideres comme nuls
c
c     si l'on veut la transformation appliquee,appeler wrref avec
c     la matrice obtenue en concatenant l'identite a la matrice a
c     en rajoutant des colonnes.
c!sous programmes appeles
c     iwamax wcopy wswap wscal wasum waxpy (blas.extensions)
c     dset (blas)
c     dble abs max (fortran)
c!
c!
      double precision ar(lda,*),ai(lda,*),eps,tol,tr,ti,wasum
      tol = 0.0d+0
      do 10 j = 1, n
         tol = max(tol,wasum(m,ar(1,j),ai(1,j),1))
   10 continue
      tol = eps*dble(2*max(m,n))*tol
      k = 1
      l = 1
   20 if (k.gt.m .or. l.gt.n) return
      i = iwamax(m-k+1,ar(k,l),ai(k,l),1) + k-1
      if (abs(ar(i,l))+abs(ai(i,l)) .gt. tol) go to 30
      call dset(m-k+1,0.0d+0,ar(k,l),1)
      call dset(m-k+1,0.0d+0,ai(k,l),1)
         l = l+1
         go to 20
   30 call wswap(n-l+1,ar(i,l),ai(i,l),lda,ar(k,l),ai(k,l),lda)
      call wdiv(1.0d+0,0.0d+0,ar(k,l),ai(k,l),tr,ti)
      call wscal(n-l+1,tr,ti,ar(k,l),ai(k,l),lda)
      ar(k,l) = 1.0d+0
      ai(k,l) = 0.0d+0
      do 40 i = 1, m
         tr = -ar(i,l)
         ti = -ai(i,l)
         if (i .ne. k) call waxpy(n-l+1,tr,ti,
     $                 ar(k,l),ai(k,l),lda,ar(i,l),ai(i,l),lda)
   40 continue
      k = k+1
      l = l+1
      go to 20
      end
