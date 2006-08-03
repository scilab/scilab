c
c-----------------------------------------------------------------------
c
c     Copyright INRIA
      subroutine n1qn3a (simul,prosca,ctonb,ctcab,n,x,f,g,dxmin,df1,
     /                   epsg,impres,io,mode,niter,nsim,m,d,gg,diag,aux,
     /                   alpha,ybar,sbar,izs,rzs,dzs)
c----
c
c     Code d'optimisation proprement dit.
c
c----
c
c         arguments
c
      integer n,impres,io,mode,niter,nsim,m,izs(1)
      real rzs(1)
      double precision x(n),f,g(n),dxmin,df1,epsg,d(n),gg(n),diag(n),
     /    aux(n),alpha(m),ybar(n,m),sbar(n,m),dzs(1)
      external simul,prosca,ctonb,ctcab
c
c         variables locales
c
      integer i,iter,moderl,isim,jmin,jmax,indic
      double precision r1,t,tmin,tmax,gnorm,eps1,ff,preco,precos,ys,den,
     /    dk,dk1,ps,ps2,hp0
c
c         parametres
c
      double precision rm1,rm2
      parameter (rm1=0.1d-3,rm2=0.9d+0)
      double precision pi
      parameter (pi=3.1415927d+0)
c
c---- initialisation
c
      iter=0
      isim=1
c
      call prosca (n,g,g,ps,izs,rzs,dzs)
      gnorm=sqrt(ps)
      if (impres.ge.1) write (io,900) f,gnorm
900   format (5x,"f         = ",d15.8,
     /       /,5x,"norm of g = ",d15.8)
c
c     ---- mise a l'echelle de la premiere direction de descente
c
      precos=2.d0*df1/gnorm**2
      do 10 i=1,n
          d(i)=-g(i)*precos
10    continue
      if (impres.ge.5) write(io,899) precos
899   format (/," n1qn3a: descent direction -g: precon = ",d10.3)
      if (impres.eq.3) then
          write(io,901)
          write(io,9010)
      endif
      if (impres.eq.4) write(io,901)
c
c     ---- initialisation pour nlis0
c
      tmax=1.d+20
      call prosca (n,d,g,hp0,izs,rzs,dzs)
c
c     ---- initialisation pour dd
c
      jmin=1
      jmax=0
c
c---- debut de l'iteration. On cherche x(k+1) de la forme x(k) + t*d,
c     avec t > 0. On connait d.
c
c         debut de la boucle: etiquette 100,
c         sortie de la boucle: goto 1000.
c
100   iter=iter+1
      if (impres.lt.0) then
          if(mod(iter,-impres).eq.0) then
              indic=1
              call simul (indic,n,x,f,g,izs,rzs,dzs)
              goto 100
          endif
      endif
      if (impres.ge.5) write(io,901)
901   format (/,1x,79("-"))
      if (impres.ge.4) write(io,9010)
9010  format (1x)
      if (impres.ge.3) write (io,902) iter,isim,f,hp0
902   format (" n1qn3: iter ",i3,", simul ",i3,
     /        ", f=",d15.8,", h'(0)=",d12.5)
      do 101 i=1,n
          gg(i)=g(i)
101   continue
      ff=f
c
c     ---- recherche lineaire et nouveau point x(k+1)
c
      if (impres.ge.5) write (io,903)
903   format (/," n1qn3: line search")
c
c         ---- calcul de tmin
c
      tmin=0.d0
      do 200 i=1,n
          tmin=max(tmin,abs(d(i)))
200   continue
      tmin=dxmin/tmin
      t=1.d0
      r1=hp0
c
      call nlis0 (n,simul,prosca,x,f,r1,t,tmin,tmax,d,g,rm2,rm1,
     /           impres,io,moderl,isim,nsim,aux,izs,rzs,dzs)
c
c          ---- nlis0 renvoie les nouvelles valeurs de x, f et g
c
      if (moderl.ne.0) then
          if (moderl.lt.0) then
c
c             ---- calcul impossible
c                  t, g: ou les calculs sont impossible
c                  x, f: ceux du t_gauche (donc f <= ff)
c
              mode=moderl
          elseif (moderl.eq.1) then
c
c             ---- descente bloquee sur tmax
c                  [sortie rare (!!) d'apres le code de nlis0]
c
              mode=3
              if (impres.ge.1) write(io,904) iter
904           format(/," >>> n1qn3 (iteration ",i3,
     /                "): line search blocked on tmax: ",
     /                "decrease the scaling")
          elseif (moderl.eq.4) then
c
c             ---- nsim atteint
c                  x, f: ceux du t_gauche (donc f <= ff)
c
              mode=5
          elseif (moderl.eq.5) then
c
c             ---- arret demande par l'utilisateur (indic = 0)
c                  x, f: ceux en sortie du simulateur
c
              mode=0
          elseif (moderl.eq.6) then
c
c             ---- arret sur dxmin ou appel incoherent
c                  x, f: ceux du t_gauche (donc f <= ff)
c
              mode=6
          endif
          goto 1000
      endif
c
c     ---- tests d'arret
c
      call prosca(n,g,g,ps,izs,rzs,dzs)
      eps1=sqrt(ps)/gnorm
c
      if (impres.ge.5) write (io,905) eps1
905   format (/," n1qn3: stopping criterion on g: ",d12.5)
      if (eps1.lt.epsg) then
          mode=1
          goto 1000
      endif
      if (iter.eq.niter) then
          mode=4
          if (impres.ge.1) write (io,906) iter
906       format (/," >>> n1qn3 (iteration ",i3,
     /            "): maximal number of iterations")
          goto 1000
      endif
      if (isim.gt.nsim) then
          mode=5
          if (impres.ge.1) write (io,907) iter,isim
907       format (/," >>> n1qn3 (iteration ",i3,"): ",i6,
     /            " simulations (maximal number reached)")
          goto 1000
      endif
c
c     ---- mise a jour de la matrice
c
      if (m.gt.0) then
          jmax=jmax+1
          if (iter.gt.m) then
              jmin=jmin+1
              if (jmin.gt.m) jmin=jmin-m
              if (jmax.gt.m) jmax=jmax-m
          endif
c
c          ---- y, s et (y,s)
c
          do 400 i=1,n
              sbar(i,jmax)=t*d(i)
              ybar(i,jmax)=g(i)-gg(i)
400       continue
          if (impres.ge.5) then
              call prosca (n,sbar(1,jmax),sbar(1,jmax),ps,izs,rzs,dzs)
              dk1=sqrt(ps)
              if (iter.gt.1) write (io,910) dk1/dk
910           format (/," n1qn3: convergence rate, s(k)/s(k-1) = ",
     /                d12.5)
              dk=dk1
          endif
          call prosca (n,ybar(1,jmax),sbar(1,jmax),ps,izs,rzs,dzs)
          ys=ps
          if (ys.le.0.d0) then
              mode=7
              if (impres.ge.1) write (io,908) iter,ys
908           format (/," >>> n1qn3 (iteration ",i2,
     /                "): the scalar product (y,s) = ",d12.5
     /                /27x,"is not positive")
              goto 1000
          endif
          if (impres.ge.5) write(io,909)
909       format (/," n1qn3: matrix update:")
c
c          ---- ybar et sbar
c
          r1=sqrt(1.d0/ys)
          do 410 i=1,n
              sbar(i,jmax)=r1*sbar(i,jmax)
              ybar(i,jmax)=r1*ybar(i,jmax)
410       continue
c
c          ---- calcul de la diagonale de preconditionnement
c
          call prosca (n,ybar(1,jmax),ybar(1,jmax),ps,izs,rzs,dzs)
          precos=1.d0/ps
          if (iter.eq.1) then
              do 401 i=1,n
                  diag(i)=precos
401           continue
          else
c
c             ---- ajustememt de la diagonale a l'ellipsoide de Rayleigh
c
              call ctonb (n,ybar(1,jmax),aux,izs,rzs,dzs)
              r1=0.d0
              do 398 i=1,n
                  r1=r1+diag(i)*aux(i)**2
398           continue
              if (impres.ge.5) then
                  write (io,915) 1.d0/r1
915               format(5x,"fitting the ellipsoid: factor ",d10.3)
              endif
              do 399 i=1,n
                  diag(i)=diag(i)/r1
399           continue
c
c             ---- mise a jour diagonale
c                  gg utilise comme vecteur auxiliaire
c
              call ctonb (n,sbar(1,jmax),gg,izs,rzs,dzs)
              den = 0.d0
              do 402 i=1,n
                  den = den + gg(i)**2/diag(i)
402           continue
              do 403 i=1,n
                  diag(i)=1.d0/(1.d0/diag(i)+aux(i)**2
     /                          -(gg(i)/diag(i))**2/den)
403           continue
          endif
          if (impres.ge.5) then
              preco=0.d0
              do 406 i=1,n
                  preco=preco+diag(i)
406           continue
              preco=preco/n
              write (io,912) precos,preco
912           format (5x,"Oren-Spedicato factor (not used) = ",
     /                d10.3/5x,"diagonal: average value = ",d10.3)
          endif
      endif
c
c     ---- calcul de la nouvelle direction de descente d = - h.g
c
      if (m.eq.0) then
          preco=2.d0*(ff-f)/(eps1*gnorm)**2
          do 500 i=1,n
              d(i)=-g(i)*preco
500       continue
      else
          do 510 i=1,n
              d(i)=-g(i)
510       continue
          call ddd2 (prosca,ctonb,ctcab,n,m,d,aux,jmin,jmax,
     /               diag,alpha,ybar,sbar,izs,rzs,dzs)
      endif
c
c          ---- test: la direction d est-elle de descente ?
c               hp0 sera utilise par nlis0
c
      call prosca (n,d,g,hp0,izs,rzs,dzs)
      if (hp0.ge.0.d+0) then
          mode=7
          if (impres.ge.1) write (io,913) iter,hp0
913       format (/," >>> n1qn3 (iteration ",i2,"): "
     /            /5x," the search direction d is not a",
     /             "descent direction: (g,d) = ",d12.5)
          goto 1000
      endif
      if (impres.ge.5) then
          call prosca (n,g,g,ps,izs,rzs,dzs)
          ps=dsqrt(ps)
          call prosca (n,d,d,ps2,izs,rzs,dzs)
          ps2=dsqrt(ps2)
          ps=hp0/ps/ps2
          ps=dmin1(-ps,1.d+0)
          ps=dacos(ps)
          r1=ps*180.d0/pi
          write (io,914) sngl(r1)
914       format (/," n1qn3: descent direction d: ",
     /            "angle(-g,d) = ",f5.1," degrees")
      endif
c
c---- on poursuit les iterations
c
      goto 100
c
c---- retour
c
1000  niter=iter
      nsim=isim
      epsg=eps1
      return
      end
