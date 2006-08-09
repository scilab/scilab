C/MEMBR ADD NAME=WMPTLD,SSI=0
c     Copyright INRIA
      subroutine wmptld(pm1r,pm1i,d1,ld1,pm2r,pm2i,d2,m,n)
c!but
c     cette routine determine la matrice polynomiale :
c     z**n*pm(1/z)' ou n est le degre maximum des elements de pm
c     et z la variable formelle. pm a coefficients complexes
c!liste d'appel
c
c    subroutine wmptld(pm1r,pm1i,d1,ld1,pm2r,pm2i,d2,m,n)
c    double precision pm1r(*),pm1i(*),pm2r(*),pm2i(*)
c    integer d1(*),d2(n,m),m,n
c
c     pm1 : tableau  contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*ld1 + k) )
c           pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     ld1 : entier definissant le rangement dans d1
c
c     pm2,d2 : definitions similaires a celles de pm1,d1, ld2
c                 est suppose  egal a n
c     m : nombre de ligne de la matrice pm1
c     n : nombre de colonne de matrice pm1
c!origine
c    s Steer INRIA
c!
      double precision pm1r(*),pm1i(*),pm2r(*),pm2i(*),norm,wasum
      integer d1(*),d2(*),m,n
c
c determination du degre maxi
      d2(1)=1
      nmax=0
      i2=1
      do 12 i=1,m
      i1=i
      do 11 j=1,n
      l1=d1(i1)
      n1=d1(i1+1)-l1+1
      norm=wasum(n1-1,pm1r(l1),pm1i(l1),1)
   10 n1=n1-1
      if(abs(pm1r(l1+n1-1))+abs(pm1i(l1+n1-1))+norm.le.norm) goto 10
      i1=i1+ld1
      i2=i2+1
      d2(i2)=n1
      nmax=max(nmax,n1)
   11 continue
   12 continue
c
c     transcription
      d2(1)=1
      i2=1
      do 32 i=1,m
      i1=i
      do 31 j=1,n
      n1=d2(i2+1)
      l2=d2(i2)
      if(n1.ge.nmax) goto 30
      call dset(nmax-n1,0.0d+0,pm2r(l2),1)
      call dset(nmax-n1,0.0d+0,pm2i(l2),1)
   30 call dcopy(n1,pm1r(l1),1,pm2r(l2),-1)
      call dcopy(n1,pm1i(l1),1,pm2i(l2),-1)
      i1=i1+ld1
      i2=i2+1
      d2(i2)=l2+nmax
   31 continue
   32 continue
c
      call dscal(d2(1+m*n)-1,-1.0d+0,pm2i,1)
c
      return
      end
