C/MEMBR ADD NAME=WLSLV,SSI=0
c     Copyright INRIA
      subroutine wlslv(ar,ai,na,n,br,bi,nb,m,w,rcond,ierr,job)
c! purpose
c   la factorisation lu de la matrice a si job=0
c   la resolution du systeme a*x=b si job=1
c   la resolution du systeme x*a=b si job=2
c le calcul de l'inverse de a si job=3
c!calling sequence
c
c       subroutine wlslv(ar,ai,na,n,br,bi,nb,m,w,rcond,ierr,job)
c ar,ai:tableaux de taille na*n contenant la matrice a
c    apres execution a contient la factorisation lu
c na:dimensionnement de a dans le programme appelant
c n:dimensions de la matrice a
c br,bi:tableaux de taille nb*m contenant la matrice b et le resultat x
c nb:dimensionnement de b dans le programme appelant
c m:nombre de colonnes de b si job=1;ou nombre de ligne si job=2
c w:tableau de travail de taille 2*n+adr(n,1)
c rcond:reel contenant le conditionnement inverse de a
c ierr:indicateur de deroulement
c     ierr=0 ok
c     ierr=1 rcond=0
c     ierr=-1 rcond negligeable
c job:
c     si a et w n'ont pas ete modifies on peut reentrer dans le
c     sous programme avec une nouvelle matrice b (job=-1 ou job=-2)
c!sous programmes appeles
c     wgeco wgesl wgedi (linpack.extensions)
c     dcopy dscal (blas)
c     abs (fortran)
c!
      double precision ar(na,n),ai(na,n),br(*),bi(*),w(*)
      double precision rcond,dtr(2),dti(2)
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
      call wgeco(ar,ai,na,n,w,rcond,w(k1),w(k1+n))
      if(rcond.eq.0.0d+0) goto 70
      if(rcond+1.0d+0.eq.1.0d+0) ierr=-1
      if(job.eq.0) return
      if(job.eq.3) goto 60
c resolution
  20  if(abs(job).eq.2) goto 40
      jb=1
      do 30 j=1,m
      call wgesl(ar,ai,na,n,w,br(jb),bi(jb),0)
      jb=jb+nb
   30 continue
      return
   40 do 50 j=1,m
      call dcopy(n,br(j),nb,w(k1),1)
      call dcopy(n,bi(j),nb,w(k1+n),1)
      call dscal(n,-1.0d+0,w(k1+n),1)
      call wgesl(ar,ai,na,n,w,w(k1),w(k1+n),1)
      call dcopy(n,w(k1),1,br(j),nb)
      call dcopy(n,w(k1+n),1,bi(j),nb)
      call dscal(n,-1.0d+0,bi(j),nb)
   50 continue
      return
   60 call wgedi(ar,ai,na,n,w,dtr,dti,w(k1),w(k1+n),1)
       return
   70 ierr=1
      return
      end
