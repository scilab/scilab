C/MEMBR ADD NAME=SFACT2,SSI=0
c     Copyright INRIA
      subroutine sfact2(b,l,n,matg,maxit,ierr)
c
c!but
c      Etant donnee la matrice bloc :[B0, ...,  B(n-1), B(n)]
c     ou les Bi sont les coefficients (de degre i) du produit
c     de la matrice polynomiale A par A'(1/z)
c
c     alors cette subroutine produit les coefficients d'une
c     matrice polynomiale Hurwitz D, qui est le facteur
c     spectrale gauche associe a A, tel que A*A'(1/z)=D*D'(1/z)
c     (ou D=D0+D1*d+ ... +Dn*(d**n)).
c
c!methode
c     La methode de factorisation spectrale donnee ici est basee
c     dans la methode de factorisation de Cholesky. Elle est
c     iterative et asuure les convergences monotone et geometrique.
c     En plus elle peut etre employee naturellement pour des
c     polynomes scalaires, cependant que dans ce cas existent
c     des algorithmes plus surs et rapides.
c
c     Voir V. KUCERA.- Discrete linear Control (The polynomial
c     equation approach), J. Wiley & sons, 1979. Secs 2.10 et 7.13.
c
c!parametres d'appel
c
c     call sfact2(b,l,n,matg,maxit,ierr)
c
c     double precision b(l,(n+1)*l) ) , matg(n*n*l*l)
c     integer n,maxit,ierr
c
c     b : contient les coefficients bi : bi=b(1:l,1+(i-1)*l)
c         apres execution b contient les di.
c     l : nombre de lignes et de colonnes des bi
c     n: degre du polynome matriciel A, qui donne origine a b
c     matg : tableau de travail de taille q*(q+1)/2 avec q=(n+1)*l
c
c     maxit:entier, indique le nombre maximum d'maxitations admis
c
c     ierr:entier, si 0 fin normale,
c                  si -1 fin pour quantite maximum d'iterations
c                  si 1 probleme singulier ou non symetrique
c                  atteinte
c
c!auteur
c
c     Cette subroutine est la version fortran de l'algorithme
c     donne dans la section 7.13 du livre de vladimir kucera:
c     "discrete linear control", faite par
c     carlos klimann, inria, 16-xii-85.
c
c!
c
      double precision b(l,*),matg(*)
      integer n,l,ierr,maxit
c
      double precision sigma,acu,tr1,tr2
      integer i,j,k0,k,p,r,jj,kk,q,iter,q22
c
      iadr(i,j)=1+(i-j)+(q22-j)*(j-1)/2
c
c la matrice delta(i-1) est stockee dans matg sous forme compacte
c l'element (i,j) est stocke en iadr(i,j)
c l'element en haut a gauche est repere par id0,id0
c
      p=n*l
      q=p+l
      q22=2*q+2
c
      nel=q*(q+1)/2
      do 05 j=1,nel
   05 matg(j)=0.0d+0
      do 06 j=p+1,q
      do 06 r=j,q
06    matg(iadr(r,j))=b(r-p,j-p)
c
      id0=p+1
      k0=p
      iter=0
      j=p
c
c calcul de delta(0) - par choleski
      goto 20
c
10    continue
c
c calcul de x=[bi,...,b1]*delta(i-1)'**(-1)
c
      do 14 j=id0,p
      j1=(j-1)/l
      j2=j-j1*l
      jj=(n-j1)*l+j2
      if(matg(iadr(j,j)).eq.0.0d+0) goto 60
      do 13 r=p+1,q
      sigma=0.0d+0
      if(j.eq.id0)goto 12
      do 11 k=id0,j-1
      sigma=sigma+(matg(iadr(j,k))*matg(iadr(r,k)))
11    continue
12    matg(iadr(r,j))=(b(r-p,jj)-sigma)/matg(iadr(j,j))
13    continue
14    continue
c
c calcul de b0-x*x'
c
      do 18 j=p+1,q
       do 17 r=j,q
        sigma=0.0d+0
        do 16 k=id0,p
16      sigma=sigma+matg(iadr(r,k))*matg(iadr(j,k))
        matg(iadr(r,j))=b(r-p,j-p)-sigma
17     continue
18    continue
c
20    continue
c
c factorisation de cholesky du bloc en bas a droite
c
      do 26 j=p+1,q
      sigma=matg(iadr(j,j))
      if(j.eq.p+1)goto 22
      do 21 k=p+1,j-1
      sigma=sigma-(matg(iadr(j,k))*matg(iadr(j,k)))
21    continue
22    if(sigma.le.0.0d+0) goto 60
      matg(iadr(j,j))=sqrt(sigma)
      if(j.eq.q)goto 26
c
      do 25 r=j+1,q
      sigma=matg(iadr(r,j))
      if(j.eq.p+1)goto 24
      do 23 k=p+1,j-1
      sigma=sigma-(matg(iadr(j,k))*matg(iadr(r,k)))
23    continue
24    matg(iadr(r,j))=sigma/matg(iadr(j,j))
25    continue
26    continue
c
      if(n.eq.0)goto 50
c
c calcul de la trace du bloc en bas a droite
c
      tr2=0.0d+0
      do 30 jj=p+1,q
      tr2=tr2+matg(iadr(jj,jj))
30    continue
c
c test de convergence
c
      if(iter.eq.1) goto 40
      acu = abs(tr1-tr2)
      if(acu+abs(tr2).le.abs(tr2)) goto 50
      if(iter.ge.maxit)goto 50
c
c shift
c
40    id0=max(id0-l,1)
      do 41 jj=id0,p
      do 41 kk=id0,jj
      matg(iadr(jj,kk))=matg(iadr(jj+l,kk+l))
41    continue
      tr1=tr2
c
      iter=iter+1
      goto 10
c
50    continue
c
c fin
c
      do 53 r=1,l
      do 51 j=r,l
      b(r,j)=0.0d+0
51    b(j,r)=matg(iadr(p+j,p+r))
      if(n.eq.0) goto 53
      do 52 j=l+1,q
      j1=(j-1)/l
      j2=j-j1*l
      jj=(n-j1)*l+j2
52    b(r,j)=matg(iadr(p+r,jj))
53    continue
      ierr=0
      if(iter.ge.maxit) ierr=-1
      return
60    ierr=1
      return
c
      end
