c     Copyright INRIA
      subroutine kronr(a,ia,ma,na,b,ib,mb,nb,pk,ik)
c!but
c ce sous programme genere le produit de kronecker de deux matrices
c  a et b  pk(i,j)=a(i,j)*b
c!liste d'appel
c      subroutine kronr(a,ia,ma,na,b,ib,mb,nb,pk,ik)
c     double precision a(ia,na),b(ib,nb),pk(ik,*)
c     integer ia,ma,na,ib,mb,nb,ik
c
c     a : tableau contenant la matrice a
c     ia : increment entre 2 elements consecutif d'une meme
c             ligne de a
c     ma : nombre de lignes de a
c     na : nombre de colonnes dea
c     b,ib,mb,nb : definitions similaires pour la matrice b
c     pk : tableau contenant la matrice resultat pk
c     ik : increment entre deux elements consecutifs d'une meme
c          ligne de pk
c!
      double precision a(*),b(*),pk(*)
      integer ia,ma,na,ib,mb,nb,ik,ka,kb,kk,ka1,kk1
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
               call dcopy(mb,b(kb),1,pk(kk),1)
               call dscal(mb,a(ka),pk(kk),1)
               kk=kk+mb
               ka=ka+1
 10         continue
            kb=kb+ib
 20      continue
 30   continue
      return
      end
