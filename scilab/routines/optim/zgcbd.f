      subroutine zgcbd(simul,n,binf,bsup,x,f,g,zero,napmax,itmax,indgc
     &  ,ibloc,nfac,imp,io,epsx,epsf,epsg,dir,df0,diag,x2,
     &izs,rzs,dzs,y,s,z,ys,zs,nt,index,wk1,wk2,alg,ialg,nomf)
c     Copyright INRIA

c
      implicit double precision (a-h,o-z)
      real rzs(*)
      double precision dzs(*)
      dimension x2(n),dir(n),epsx(n)
      dimension binf(n),bsup(n),x(n),g(n),diag(n),ibloc(n),izs(*)
      dimension y(nt,n),s(nt,n),z(nt,n),ys(nt),zs(nt)
      dimension wk1(n),wk2(n),alg(15)
      character*6 nomf
      integer index(nt),ialg(15)
      external simul
c
      if(imp.ge.4) then
      write(io,10000)
10000 format (' dans gcbd. algorithme utilise: ')
      if(ialg(1).eq.1)write(io,10001)
10001 format ('        emploi correction de powell ')
      if(ialg(2).eq.1)write(io,10002)
10002 format ('  mise a jour de diag par la methode bfgs')
      if(ialg(3).eq.1)write(io,10003)
10003 format ('  mise a echelle de diag par methode de shanno-phua')
      if(ialg(3).eq.2)write(io,10004)
10004 format ('  mise a echelle de diag seulement a la 2e iter')
      if(ialg(4).eq.1)write(io,10005)
10005 format ('      memorisation pour choix iteration ')
      if(ialg(5).eq.1)write(io,10006)
10006 format ('      memorisation par variable')
      if(ialg(6).eq.1)write(io,10007)
10007 format ('      relachememt de variables a toutes les iteration')
      if(ialg(6).eq.2)write(io,10008)
10008 format ('      relachement de vars si decroissance g_norme')
      if(ialg(6).eq.10)write(io,10009)
10009 format ('      relachement de vars si dec f % iter_init du cycle')
      if(ialg(6).eq.11)write(io,10010)
10010 format ('      relachement de vars si dec f % dec du cycle')
      if(ialg(7).eq.1)write(io,10011)
10011 format ('      choix de vars a relacher par bertsekas modifiee')
      if(ialg(8).eq.1)write(io,10012)
10012 format ('      choix de dir descente par methode de gradient')
      if(ialg(8).eq.2)write(io,10013)
10013 format ('      choix de dir descente par methode qn')
      if(ialg(8).eq.3)write(io,10014)
10014 format ('      choix de dir descente par qn sans memoire.nt depl')
      if(ialg(8).eq.4)write(io,10015)
10015 format ('      choix de dir descente par qn -mem,redem,sans acc.')
      if(ialg(8).eq.5)write(io,10016)
10016 format ('     choix de dir descente par qn -mem,redem,avec acc.')
      if(ialg(9).eq.2)write(io,10017)
10017 format ('      redem si relachement de vars')
      if(ialg(9).eq.10)write(io,10018)
10018 format ('      redem si dec f % dec iter_init du cycle')
      if(ialg(9).eq.11)write(io,10019)
10019 format ('      redem si dec f % dec totale du cycle.')
      if(ialg(9).eq.12)write(io,10020)alg(9)
10020 format ('    redem si diminution du gradient des var libres d un',
     & 'facteur',d11.4)
      endif
c
c     section 1  initialisations
c     irl nombre de rech lin 'lentes'
c     nred nombre de redemarrage de la direction de descente
c     icycl nombre de cycles de minimisation
c
      epsgcp=1.0d-5
      indsim=4
      indrl=1
      irl=0
      irl=0
      nred=1
      icycl=1
      nap=0
c
      iresul=1
      call proj(n,binf,bsup,x)
      indsim=4
      call simul(indsim,n,x,f,g,izs,rzs,dzs)
      nap=nap+1
      if(indsim.gt.0)go to 99
      indgc=-1
      if(indsim.eq.0)indgc=0
      if(imp.gt.0)write(io,123)indgc
      go to 900
99    continue
      ceps0=20.0d+0
      eps0=0.0d+0
      do 100 i=1,n
100   eps0=eps0+epsx(i)
      eps0=ceps0*eps0/n
c
c     calcul de zng
      znog0=rednor(n,binf,bsup,x,epsx,g)
      zng=znog0
      zngrit=znog0
      zngred=znog0
c
      do 130 i=1,n
130   ibloc(i)=0
      izag=3
      izag1=izag
      nap=0
      iter=0
      scal=1.0d+0
      nfac=n
      np=0
      lb=1
      nb=2
      if(ialg(8).eq.3) nb=1
      do 140 i=1,nt
140   index(i)=i
      tetaq=alg(9)
      condm=alg(2)
      param=alg(1)
      indgc1=indgc
c     si indgc=0 on init diag a k*ident puis scal a it=2
c
      if(indgc.eq.1.or.indgc.ge.100)go to 150
      if(indgc.eq.2)go to 180
      indgc=-13
      if(imp.gt.0) write(io,123) indgc
      go to 900
c
150   continue
c     on initialise diag par approximation quadratique
c     df0 decroissance prevue . si mod quad df0=((dh)-1g,g)/2
c     et on cherche dh diag de la forme cst/(dx)**2
c     donc cst=som((g(i)*(dx))**2))/(2*df0)
      sy=0.0d+0
      do 160 i=1,n
160    sy=sy+(g(i)*epsx(i))**2
      sy=sy/(2.0d+0*df0)
      do 170 i=1,n
170   diag(i)=(sy + zero)/(epsx(i)**2 + zero)
180   continue
c
c
c     bouclage
200   iter=iter +1
      indgc=1
      if(iter.gt.itmax)then
         indgc=5
         go to 900
      endif
201   continue
      if(imp.ge.2)write(io,1210)iter,f
1210  format(/' dans gcbd  iter=',i3,'  f=',d15.7)
      if(iter.eq.1)then
         irit=1
         goto 301
      endif
c
      call majysa(n,nt,np,y,s,ys,lb,g,x,wk2,wk1,index,ialg,nb)
      inp=index(np)
c
c
c     correction powell sur y si (y,s) trop petit
      if(ialg(1).ne.1) go to 290
      param1=1.-param
      bss=0.0d+0
      do 260 i=1,n
260   bss=bss + diag(i)*s(inp,i)**2
      bss2=param*bss
      if(ys(inp).gt.bss2)go to 290
      if(imp.gt.2)write(io,1270)ys(inp)
1270  format(' gcbd. emploi correction powell (y,s)=',d11.4)
      teta=param1*bss/(bss-ys(inp))
      teta1=1.0d+0-teta
      do 274 i=1,n
274   y(inp,i)=teta*y(inp,i)+teta1*diag(i)*s(inp,i)
      ys(inp)=bss2
c     verif correction powell (facultatif; faire go to 300)
      ys1=ddot(n,s(inp,1),1,y(inp,1),1)
      ys1=abs(bss2-ys1)/bss2
      if(imp.gt.2)write(io,1280)ys1
1280  format(' erreur relative correction powell =',d11.4)
c
c mise a jour de diag
290   continue
      if(ialg(2).eq.1)
     &  call bfgsd(diag,n,nt,np,y,s,ys,condm,param,zero,index)
c
      if(ialg(3).eq.1.or.(ialg(3).eq.2.and.iter.eq.2))
     &  call shanph(diag,n,nt,np,y,s,ys,scal,index,io,imp)
c
      call majz(n,np,nt,y,s,z,ys,zs,diag,index)
c
c     section 3 determination des variables libres et bloquees
300   continue
c     -----decision de relachement a l'iteration courante
c          relachement si irit=1 (sinon irit=0)
      irit=0
      if(ialg(6).eq.1) irit=1
      if(ialg(6).eq.2.and.znglib.le.alg(6)*zngrit)irit=1
      if(ialg(6).eq.10.and.diff.le.dfrit1*alg(6))irit=1
      if(ialg(6).eq.11.and.diff.le.difrit*alg(6))irit=1
      if(irit.eq.1) nred=nred+1
c    ----choix des variables a relacher
      imp1=imp
301   if(ialg(7).eq.1)call relvar(ind,n,x,binf,bsup,x2,g,diag,
     &  imp,io,ibloc,izag,iter,nfac,irit)
c
c
c     section 4 expression de dir
      if (np.eq.0) then
         do 400 i=1,n
           dir(i)=-g(i)/diag(i)
400      continue
      else
         do 410 i=1,n
            dir(i)=-scal*g(i)
410      continue
         call gcp(n,index,ibloc,np,nt,y,s,z,ys,zs,diag,g,dir,wk1,
     &            wk2,epsgcp)
      endif
c
c     section 5  redemarrage
c
      if(ialg(8).eq.4.or.ialg(8).eq.5) then
         ired=0
         if(ialg(9).eq.2.and.ind.eq.1) ired=1
         if(ialg(9).eq.10.and.diff.lt.dfred1*tetaq) ired=1
         if(ialg(9).eq.11.and.diff.lt.difred*tetaq)  ired=1
         if(ialg(9).eq.12.and.znglib.le.tetaq*zngred) ired=1
         if(ired.eq.1) then
            icycl=icycl+1
            np=0
            lb=1
            if(imp.gt.2)write(io,1000)icycl
1000        format ('   redemarrage. icycl=',i5)
         endif
      endif
c
c     section 6 annulation de d(i) , i dans ib
      if(ialg(6).eq.1)go to 640
      do 630 i=1,n
630      if(ibloc(i).gt.0) dir(i)=0.0d+0
640   continue
c
c     recherche lineaire
c     conservation de x et g dans wk1 et wk2
      call dcopy(n,x,1,wk1,1)
      call dcopy(n,g,1,wk2,1)
c     calcul de la derivee dans la direction dir
      ifp=0
      fn=f
      znog0=zng
702   dfp=0.0d+0
      do 710 i=1,n
      epsxi=epsx(i)
      xi=x(i)
      diri=dir(i)
      if(xi-binf(i).le.epsxi.and.diri.lt.0.0d+0)dir(i)=0.0d+0
710   if(bsup(i)-xi.le.epsxi.and.diri.gt.0.0d+0)dir(i)=0.0d+0
      dfp=ddot(n,g,1,dir,1)
      if(-dfp.gt.0)go to 715
      if(ifp.eq.1) then
         indgc=6
         go to 900
      endif
c     restauration dir
      if(imp.ge.3)write(io,1712)dfp,zero
1712  format(' gcbd : restauration dir ; fp,zero',2d11.4)
      do 712 i=1,n
712   dir(i)=-scal*g(i)
      ifp=1
      go to 702
715   continue
c     pas initial suivant idee fletcher
      t=-2.0d+0*diff/dfp
      if(iter.eq.1)t=-2.0d+0*df0/dfp
      tmax=1.0d+10
      t=min(t,tmax)
      t=max(t,1.0d+10*zero)
      napm=15
      napm1=nap + napm
      if(napm1.gt.napmax) napm1=napmax
      napav=nap
      amd=0.70d+0
      amf=0.10d+0
c
      call rlbd(indrl,n,simul,x,binf,bsup,f,dfp,t,tmax,dir,g,tproj,
     &  amd,amf,imp,io,zero,nap,napm1,x2,izs,rzs,dzs)
      if(imp.gt.2)write(io,750)indrl,t,f
750   format(' retour mlibd indrl=',i6,' pas= ',d11.4,' f= ',d11.4)
      if(nap-napav.ge.5) irl=irl+1
      if(indrl.ge.10)then
         indsim=4
         nap=nap + 1
         call simul(indsim,n,x,f,g,izs,rzs,dzs)
         if(indsim.le.0)then
            indgc=-3
            if(indsim.eq.0)indgc=0
            if(imp.gt.0)write(io,123)indgc
123         format(' gcbd : retour avec indgc=',i8)
            go to 900
         endif
      endif
      if(indrl.le.0)then
         indgc=10
         if(indrl.eq.0)indgc=0
         if(indrl.eq.-3)indgc=13
         if(indrl.eq.-4)indgc=12
         if(indrl.le.-1000)indgc=11
         if(imp.gt.0)write(io,123)indgc
         go to 900
      endif
      if(imp.ge.5) then
         do 760 i=1,n
760      if(imp.gt.2)write(io,777)i,x(i),g(i),dir(i)
777      format(' i=',i2,' xgd ',3f11.4)
      endif
c
      if(nap.lt.napmax)go to 758
      if(imp.gt.0)  write(io,755)
755   format(' gcbd max appels simul')
      indgc=4
      go to 900
758   continue
c
c     section 8 test de convergence
      do 805 i=1,n
      if(abs(x(i)-wk1(i)).gt.epsx(i))go to 806
805   continue
      if(imp.gt.0) write(io,1805)
1805  format(' gcbd. retour apres convergence sur x')
      indgc=3
      go to 900
c     calcul grad residuel,norme l2
806   continue
      difg=rednor(n,binf,bsup,x,epsx,g)
      diff=fn-f
      if(imp.ge.2)write(io,860)epsg,difg,epsf,diff,nap
860   format(' gcbd. epsg,difg=',2d11.4,'  epsf,diff=',2d11.4
     &,'  nap=',i3)
c
      if(diff.le.epsf) then
         indgc=2
         go to 900
      endif
      if(difg.le.epsg) then
         indgc=1
         go to 900
      endif
c
c     -----mise a jour de difrit,dfrit1,difred,dfred1
      if(irit.eq.1) then
         difrit=diff
         dfrit1=diff
       else
         difrit=difrit+diff
      endif
      if(ired.eq.1) then
         difred=diff
         dfred1=diff
       else
         difred=difred + diff
      endif
c
      znglib=0.0d+0
      do 884 i=1,n
         if(ibloc(i).gt.0)go to 884
         aa=g(i)
         if(x(i)-binf(i).le.epsx(i)) aa=min(0.0d+0,aa)
         if(bsup(i)-x(i).le.epsx(i)) aa=max(0.0d+0,aa)
         znglib=znglib+aa**2
884   continue
      znglib=sqrt(znglib)
      if(ired.eq.1)zngred=znglib
      if(irit.eq.1)zngrit=znglib
      go to 200
c
c      fin des calculs
900   if(indrl.eq.0)indgc=0
      if(indgc.eq.1.and.indrl.le.0)  indgc=indrl
      if(imp.gt.0)write(io,123)indgc
      if(imp.ge.1.and.indrl.le.zero)write(io,1910) indrl
1910  format(' arret impose par la recherche lineaire. cf notice rlbd',
     & /,' indicateur de rlbd=',i6)
      if(imp.ge.1)write(io,950)f,difg,nap,iter,indgc
950   format(' f,norme grad,nap,iter,indgc=',2e11.4,3i6)
c
c     autres impressions finales
      if(indgc1.lt.100) return
      zrl=0.
      if(iter.gt.0) zrl=dble(nap)/dble(iter)
2000  format('     nom    n       f        norm2g   nf   iter  rl/it ',
     &          ' irl   cpu  cycl   red')
      write(io,2001)nomf,f,difg,nap,iter,zrl,irl
2001  format(1x,a6,2e11.4,2i5,f6.2,i5)
      end
