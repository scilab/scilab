      subroutine dlslv(a,na,n,b,nb,m,w,rcond,ierr,job)
c!but
c      ce sous programme effectue:
c        la factorisation lu de la matrice a si job=0
c        la resolution du systeme a*x=b si job=1
c        la resolution du systeme x*a=b si job=2
c        l'inversion de a si job=3
c
c!liste d'appel
c           subroutine dlslv(a,na,n,b,nb,m,w,rcond,ierr,job)
c      a:tableau de taille na*n contenant la matrice a
c         apres execution a contient la factorisation lu
c      na:dimensionnement de a dans le programme appelant
c      n:dimensions de la matrice a
c      b:tableau de taille nb*m contenant la matrice b et le resultat x
c      nb:dimensionnement de b dans le programme appelant
c      m:nombre de colonnes de b si job=1 ;ou nombre de ligne si job=2
c      w:tableau de travail de taille n+adr(n,1)
c      rcond:reel contenant le conditionnement inverse de a
c      ierr:indicateur de deroulement
c          ierr=0 ok
c          ierr=1 rcond=0
c          ierr=-1 rcond negligeable
c      job:
c     si a et w n'ont pas ete modifies on peut reentrer dans le
c     sous programme avec une nouvelle matrice b (job=-1 ou job=-2)
c
c!sous programmes appeles
c     dgeco dgesl dgedi (linpack)
c     dcopy (blas)
c     abs (fortran)
c!Origine S Steer
c     Copyright INRIA
c!
      double precision a(na,n),b(*),w(*),rcond,dt(2)
      integer sadr
c
c     
c      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      k1=1+sadr(n)
      ierr=0
      if(job.lt.0) goto 20
c factorisation lu
      call dgeco(a,na,n,w,rcond,w(k1))
      if(rcond.eq.0.0d+0) goto 70
      if(rcond+1.0d+0.eq.1.0d+0) ierr=-1
      if(job.eq.0) return
      if(job.eq.3) goto 60
c resolution
  20  if(abs(job).eq.2) goto 40
      jb=1
      do 30 j=1,m
      call dgesl(a,na,n,w,b(jb),0)
      jb=jb+nb
   30 continue
      return
   40 do 50 j=1,m
      call dcopy(n,b(j),nb,w(k1),1)
      call dgesl(a,na,n,w,w(k1),1)
      call dcopy(n,w(k1),1,b(j),nb)
   50 continue
      return
   60 call dgedi(a,na,n,w,dt,w(k1),1)
      return
   70 ierr=1
      return
      end
