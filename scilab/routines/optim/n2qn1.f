      subroutine n2qn1 (simul,n,x,f,g,dxmin,df1,epsabs,
c     Copyright INRIA
      & imp,io,mode,iter,nsim,binf,bsup,iz,rz,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
      dimension x(n),g(n),dxmin(n),bsup(n),binf(n)
      dimension iz(*),rz(*),izs(*),dzs(*)
      real rzs(*)
      external simul
 1000 format (41h entree dans n2qn1. dimension du probleme,i4/
     & 6h mode=,i2,9h   niter=,i4,6h nsim=,i5,5h imp=,i3/
     & 5h df1=,d9.2,9h  epsabs=,d9.2/
     & 22h  dimensions minimales,2x,3hiz(,i4,8h)    rz(,i6,1h)/)
 1001 format (25h n2qn1   appel incoherent)
 1002 format(43h sortie de n2qn1. norme du gradient projete,d10.3/
     & 6h mode=,i2,9h   niter=,i4,7h  nsim=,i5)
      if (imp.eq.0) go to 10
      nw=n*(9+n)/2
      ni=2*n+1
      write (io,1000) n,mode,iter,nsim,imp,df1,epsabs,ni,nw
   10 if (n.gt.0 .and. (df1.gt.0.d0 .or. mode.ne.1)
     & .and. epsabs.ge.0.d0
     & .and. mode.ge.1 .and. mode.le.4
     & .and. iter.gt.0 .and. nsim.gt.0) go to 100
      write (io,1001)
      mode=2
      go to 999
  100 continue
      nd=1+(n*(n+1))/2
      nww=nd+n
      nww1=nww+n
      nga=nww1+n
      nindi=1
      nibloc=nindi+n
      ni=nibloc+n
c
c      calcul du test d arret
c
      s=0.d0
      do 110 i=1,n
  110 s=s+dxmin(i)*dxmin(i)
      epsabs=epsabs*dsqrt(s/dble(float(n)))
      call n2qn1a (simul,n,x,f,g,dxmin,epsabs,df1,mode,
     & iter,nsim,imp,io,rz,rz(nd),rz(nww),rz(nww1),
     & rz(nga),binf,bsup,iz(nindi),iz(nibloc),iz(ni),
     & izs,rzs,dzs)
      if(imp.gt.0) write(io,1002) epsabs,mode,iter,nsim
  999 return
      end
