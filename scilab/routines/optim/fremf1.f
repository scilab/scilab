      subroutine fremf1 (prosca,iflag,n,ntot,nta,mm1,p,alfa,
     1 e,a,r,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
      external prosca
      dimension p(*),alfa(ntot),izs(*),e(mm1),a(mm1),r(*),dzs(*)
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
   10 r(jj)=0.0d+0
      a(1)=1.0d+0
      e(1)=0.0d+0
      if (nta1.eq.1) go to 50
      do 30 j=2,nta1
      e(j)=1.0d+0
      nj=(j-2)*n
      mej=(j-1)*mm1
      do 30 i=2,j
      ni=(i-2)*n
c
c             produit scalaire de g(i-1) avec g(j-1)
c             pour j-1=1,nta et i-1=1,j-1
c             qui va dans r((j-1)*mm1+i)
c
      nij=mej+i
      call prosca (n,p(ni+1),p(nj+1),r(nij),izs,rzs,dzs)
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
      e(kk)=1.0d+0
      r(mekk+1)=0.0d+0
      nj=(kk-2)*n
      do 70 i=2,kk
      ni=(i-2)*n
c
c             produit scalaire de g(kk-1) avec g(i-1)
c             pour kk-1=nta+1,ntot et i-1=1,kk-1
c             qui va dans r((kk-1)*mm1+i)
c
      nij=mekk+i
      call prosca (n,p(ni+1),p(nj+1),r(nij),izs,rzs,dzs)
   70 continue
c
c          remplissage de la contrainte d'inegalite
c               (tout entiere sauf l'ecart)
c
      do 80 i=2,nt1
   80 a(i)=alfa(i-1)
  100 return
      end
