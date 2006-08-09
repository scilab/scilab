C/MEMBR ADD NAME=DMP2PM,SSI=0
c     Copyright INRIA
      subroutine dmp2pm(mp,d,nl,pm,deg,m,n)
c!but
c Le sous programme dmptopm converti la  matrice de polynomes
c definie par mp,d,nl en un polynome matriciel pm
c!liste d'appel
c     double precision mp(*),pm(m,n*(deg+1))
c     integer d(nl*n+1),nl,m,n,deg
c
c     mp: tableau contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome mp(i,j) est range
c           dans mp( d(i + (j-1)*nl + k) )
c           mp doit etre de taille au moins d(nl*n+1)-d(1)
c     d : tableau entier de taille nl*n+1,  si k=i+(j-1)*nl alors
c          d(k)) contient  l'adresse dans pm du coeff de degre 0
c          du polynome mp(i,j). Le degre du polynome mp(i,j) vaut:
c          d(k+1)-d(k) -1
c     nl : entier definissant le rangement dans d
c     pm : tableau contenant les matrices coefficients du polynomes
c          matriciel :soit mp_i la matrice formee des coefficients
c          de degre i de mp, alors
c          pm=[mp_0,.....mp_deg]
c          si deg >max(degre(mp(i,j))) les termes non presents dans mp
c          sont supposes nuls
c     deg: degre maxi des coefficients du polynome matriciel a extraire
c     m : nombre de ligne de la matrice polynomiale
c     n : nombre de colonnes de la matrice polynomiale
c
c!origine
c Serge Steer Inria 1988
c!
      double precision mp(*),pm(*)
      integer d(*),nl,m,n,deg
c
      mn=m*n
      call dset(mn*(deg+1),0.0d+0,pm,1)
c
      imp=-nl
      ipm=-m
      do 20 k=1,n
      imp=imp+nl
      ipm=ipm+m
      do 10 l=1,m
      kij=min(deg+1,d(imp+l+1)-d(imp+l))
      call dcopy(kij,mp(d(imp+l)),1,pm(l+ipm),mn)
 10   continue
 20   continue
      return
      end
