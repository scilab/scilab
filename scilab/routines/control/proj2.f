C/MEMBR ADD NAME=PROJ2,SSI=0
c     Copyright INRIA
      subroutine proj2(f,nn,am,n,np1,np2,pf,w)
c ce sous programme calcule les produits scalaires:
c <f,(z**(n-v)/am>=pf(np+1-v)   v=np1...np2  ;np=np2-np1+1
c les pf(v) sont les dernieres valeurs de la filtree de f(nn+1-i) par
c (z**n-1)/am,am est un polynome de degre n range par puissance
c croissante
c
c w:tableau de travail de taille n
c
      implicit double precision (a-h,o-z)
      dimension f(nn),am(*),pf(*),w(n)
c
c w contient l'etat du filtre
c
      np=np2
      if(np1.gt.1) np=np2-np1+1
c
      nf=0
      nb=nn-np
      an=am(n+1)
      if(n.eq.1) goto 50
      call dset(n,0.0d+0,w,1)
      do 20 i=1,nb
      wn=w(n)/an
      nf=nf+1
      iw=n+1
      do 10 j=2,n
      iw=iw-1
      w(iw)=w(iw-1)-am(iw)*wn
   10 continue
      w(1)=-am(1)*wn
      w(n)=w(n)+f(nf)
   20 continue
c les n valeurs suivantes de la sortie du filtre donnent les produits
c scalaires
      do 31 i=1,np
      wn=w(n)/an
      iw=n+1
      nf=nf+1
      do 30 j=2,n
      iw=iw-1
      w(iw)=w(iw-1)-am(iw)*wn
   30 continue
      w(1)=-am(1)*wn
      w(n)=w(n)+f(nf)
      pf(i)=w(n)/an
   31 continue
      if(np1.ge.1) return
      do 41 i=np+1,np2-np1+1
      wn=w(n)/an
      iw=n+1
      do 40 j=2,n
      iw=iw-1
      w(iw)=w(iw-1)-am(iw)*wn
   40 continue
      w(1)=-am(1)*wn
      pf(i)=w(n)/an
   41 continue
      return
c cas  particulier n=1
   50 wn=0.0d+0
      do 60 i=1,nb
      nf=nf+1
      wn=-am(1)/an*wn+f(nf)
   60 continue
      do 70 i=1,np
      nf=nf+1
      wn=-am(1)/an*wn+f(nf)
      pf(i)=wn/an
   70 continue
      if(np1.ge.1) return
      do 71 i=np+1,np2-np1+1
      nf=nf+1
      wn=-am(1)/an*wn
      pf(i)=wn/an
   71 continue
      return
      end
