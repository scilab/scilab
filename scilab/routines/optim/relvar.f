      subroutine relvar(ind,n,x,binf,bsup,x2,g,diag,imp,io,ibloc,izag,
     &iter,nfac,irit)
c     Copyright INRIA

c     determination des variables a relacher par meth bertsekas
      implicit double precision (a-h,o-z)
      dimension x(n),binf(n),bsup(n),x2(n),g(n),ibloc(n),diag(n)
c     x2 vect de travail de dim n
c     ind: =1 si relachement des vars
c          =0 sinon
c
c     calcul eps1
      do 10 i=1,n
10    x2(i)=x(i)-abs(g(i))*g(i)/diag(i)
      call proj(n,binf,bsup,x2)
      eps1=0.
      do 20 i=1,n
20    eps1=eps1 + abs(x2(i)-x(i))
      if(imp.gt.2)write(io,322)eps1
322   format(' relvar1. valeur de eps1=',d15.7)
c     nfac nombre de lignes factorisees (nr pour ajournd)
      ifac=0
      idfac=0
      k=0
      frac=1./10.
      do 340 k=1,n
      bi=binf(k)
      bs=bsup(k)
      d1=x(k)-bi
      d2=bs-x(k)
      dd=(bs-bi)*frac
      ep=min(eps1,dd)
      if(d1.gt.ep)go to 324
      if(g(k).gt.0.)go to 330
      go to 335
324   if(d2.gt.ep)go to 335
      if(g(k).gt.0.)go to 335
      go to 330
c     on defactorise si necessaire
330   continue
      if(ibloc(k).gt.0)go to 340
      ibloc(k)=iter
      idfac=idfac+1
      nfac=nfac-1
      ind=1
      if(imp.ge.4)write(io,336)k,x(k)
336   format(' defactorisation de x(',i3,')=',d15.7)
      go to 340
c     on factorise
335   continue
      if(irit.eq.0) go to 340
      if(ibloc(k).le.0)go to 340
      izag1=iter-ibloc(k)
      if(izag.ge.izag1)go to 340
      ifac=ifac+1
      nfac=nfac+1
      ibloc(k)=-iter
      if(imp.ge.4)write(io,339)k
339   format(' on factorise l indice ',i3)
340   continue
      if(imp.ge.2.and.(ifac.gt.0.or.idfac.gt.0))
     &                       write(io,350)ifac,idfac,nfac
350   format(' relvar1 . nbre fact',i3,' nbre defact',i3,' nbre var
     &factorisees',i3)
      ind=1
      if(ifac.eq.0.and.idfac.eq.0)ind=0
      return
      end
