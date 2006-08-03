      subroutine fremf2 (prosca,iflag,n,ntot,nta,mm1,p,alfa,e,a,r,
     1 izs,rzs,dzs)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      external prosca
      dimension p(*),alfa(ntot),izs(*),dzs(*),e(mm1),a(mm1),r(*)
      real rzs(*)


c
c          cette subroutine remplit les donnees pour fprf2
c          (produits scalaires et 2 contraintes lineaires)
c
c             de 1 a ntot +1  si iflag=0
c             de nta+1 +1 a ntot +1 sinon
c
c             (le +1 est du a l'ecart, place en premier)
c
c          p contient les opposes des gradients a la queue leu leu
c          -g(1), -g(2),..., -g(ntot) soit ntot*n coordonnees
c
      nt1=ntot+1
      nta1=nta+1
      if(iflag.gt.0) go to 50
c
c                remplissage des anciennes donnees
c          (produits scalaires, ecart et contrainte d'egalite)
c
      do 10 j=1,ntot
      jj=(j-1)*mm1+1
   10 r(jj)=0.d0
      a(1)=1.d0
      e(1)=0.d0
      if (nta1.eq.1) go to 50
      do 30 j=2,nta1
      e(j)=1.d0
      nj=(j-2)*n
      mej=(j-1)*mm1
      do 30 i=2,j
      ni=(i-2)*n
c
c             produit scalaire de g(i-1) avec g(j-1)
c             pour j-1=1,nta et i-1=1,j-1
c
      call prosca (n,p(ni+1),p(nj+1),ps,izs,rzs,dzs)
      nij=mej+i
c               le produit scalaire ci-dessus va dans r((j-1)*mm1+i)
      r(nij)=ps
   30 continue
c
c
   50 nta2=nta+2
c
c          remplissage des nouvelles donnees
c
      if (nta2.gt.nt1) go to 100
      do 70 kk=nta2,nt1
      mekk=(kk-1)*mm1
      e(kk)=1.d0
      r(mekk+1)=0.d0
      nj=(kk-2)*n
      do 70 i=2,kk
      ni=(i-2)*n
c
c             produit scalaire de g(kk-1) avec g(i-1)
c             pour kk-1=nta+1,ntot et i-1=1,kk-1
c
      call prosca (n,p(ni+1),p(nj+1),ps,izs,rzs,dzs)
      nij=mekk+i
c               le produit scalaire ci-dessus va dans r((kk-1)*mm1+i)
   70 r(nij)=ps
c
c          remplissage de la contrainte d'inegalite
c               (tout entiere sauf l'ecart)
c
      do 80 i=2,nt1
   80 a(i)=dble(alfa(i-1))
  100 return
      end
