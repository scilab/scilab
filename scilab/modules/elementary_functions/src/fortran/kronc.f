C/MEMBR ADD NAME=KRONC,SSI=0
c     Copyright INRIA
      subroutine kronc(ar,ai,ia,ma,na,br,bi,ib,mb,nb,pkr,pki,ik)
c!but
c ce sous programme genere le produit de kronecker de deux matrices
c  a et b complexes  pk(i,j)=a(i,j)*b
c!liste d'appel
c      subroutine kronc(ar,ai,ia,ma,na,br,bi,ib,mb,nb,pkr,pki,ik)
c     double precision ar(*),ai(*),br(*),bi(*),pkr(*),pki(*)
c     integer ia,ma,na,ib,mb,nb,ik
c
c     ar,ai : tableaux contenant les parties reelles et imaginaires
c          de la matrice a
c     ia : increment entre 2 elements consecutif d'une meme
c             ligne de a
c     ma : nombre de lignes de a
c     na : nombre de colonnes dea
c     br,bi,ib,mb,nb : definitions similaires pour la matrice b
c     pkr,pki : tableaux contenant les parties reelles et imaginaires
c               du resultat
c     ik : increment entre deux elements consecutifs d'une meme
c          ligne de pk
c!
      integer ia,ma,na,ib,mb,nb,ik,ka,kb,kk,ka1,kk1,l1
      double precision ar(*),ai(*),br(*),bi(*),pkr(*),pki(*)
c
      ka1=1-ia
      kk1=-nb
      do 30 ja=1,na
      kb=1
      ka1=ka1+ia
      kk1=kk1+nb
      do 20 jb=1,nb
      ka=ka1
      kk=1+(jb-1+kk1)*ik
      do 10 i=1,ma
      do 5 l=1,mb
      l1=l-1
      pkr(kk+l1)=ar(ka)*br(kb+l1)-ai(ka)*bi(kb+l1)
      pki(kk+l1)=ar(ka)*bi(kb+l1)+ai(ka)*br(kb+l1)
   5  continue
      kk=kk+mb
      ka=ka+1
   10 continue
      kb=kb+ib
   20 continue
   30 continue
      return
      end
