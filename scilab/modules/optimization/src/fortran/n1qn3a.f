c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
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
      character bufstr*(4096)
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
      if (impres.ge.1) then

        write (bufstr,900) f
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))

        write (bufstr,990) gnorm
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
900   format (5x,"f         = ",d15.8)
990   format (5x,"norm of g = ",d15.8)
c
c     ---- mise a l'echelle de la premiere direction de descente
c
      precos=2.d0*df1/gnorm**2
      do 10 i=1,n
          d(i)=-g(i)*precos
10    continue
      if (impres.ge.4) then
        write(bufstr,899) precos
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
899   format (" n1qn3a: descent direction -g: precon = ",d10.3)
      if (impres.eq.3) then
          write(bufstr,901)
          call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
          write(bufstr,9010)
          call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      endif
      if (impres.eq.4) then
        write(bufstr,901)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
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
c     Si impres<0 et l'itération est un multiple de -impres,
c     alors on appelle la fonction fournie, avec indic=1.
c
100   iter=iter+1
      if (impres.lt.0) then
          if(mod(iter,-impres).eq.0) then
              indic=1
              call simul (indic,n,x,f,g,izs,rzs,dzs)
c             error in user function
              if(indic.eq.0) goto 1000
          endif
      endif
      if (impres.ge.4) then
        write(bufstr,901)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
901   format (1x,79("-"))
      if (impres.ge.3) then
        write(bufstr,9010)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
9010  format (1x,' ')
      if (impres.ge.2) then
        write (bufstr,902) iter,isim,f,hp0
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
902   format (" n1qn3: iter ",i3,", simul ",i3,
     /        ", f=",d15.8,", h'(0)=",d12.5)
      do 101 i=1,n
          gg(i)=g(i)
101   continue
      ff=f
c
c     ---- recherche lineaire et nouveau point x(k+1)
c
      if (impres.ge.4) then
        write (bufstr,903)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
903   format (" n1qn3: line search")
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
              if (impres.ge.1) then
                write(bufstr,904) iter
                call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
                endif
904           format(" >>> n1qn3 (iteration ",i3,
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
      if (impres.ge.4) then
        write (bufstr,905) eps1
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
905   format (" n1qn3: stopping criterion on g: ",d12.5)
      if (eps1.lt.epsg) then
          mode=1
          goto 1000
      endif
      if (iter.eq.niter) then
          mode=4
          if (impres.ge.1) then
            write (bufstr,906) iter
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
906       format (" >>> n1qn3 (iteration ",i3,
     /            "): maximal number of iterations")
          goto 1000
      endif
      if (isim.gt.nsim) then
          mode=5
          if (impres.ge.1) then
            write (bufstr,907) iter,isim
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
907       format (" >>> n1qn3 (iteration ",i3,"): ",i6,
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
          if (impres.ge.4) then
              call prosca (n,sbar(1,jmax),sbar(1,jmax),ps,izs,rzs,dzs)
              dk1=sqrt(ps)
              if (iter.gt.1) then
                write (bufstr,910) dk1/dk
                call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
                endif
910           format (" n1qn3: convergence rate, s(k)/s(k-1) = ",
     /                d12.5)
              dk=dk1
          endif
          call prosca (n,ybar(1,jmax),sbar(1,jmax),ps,izs,rzs,dzs)
          ys=ps
          if (ys.le.0.d0) then
              mode=7
              if (impres.ge.1) then
                write (bufstr,908) iter,ys
                call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
                endif
908           format (" >>> n1qn3 (iteration ",i2,
     & "): the scalar product (y,s) = ",
     & d12.5,27x,"is not positive")
              goto 1000
          endif
          if (impres.ge.4) then
            write(bufstr,909)
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
909       format (" n1qn3: matrix update:")
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
              if (impres.ge.4) then
                  write (bufstr,915) 1.d0/r1
                  call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
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
          if (impres.ge.4) then
              preco=0.d0
              do 406 i=1,n
                  preco=preco+diag(i)
406           continue
              preco=preco/n
              write (bufstr,912) precos,preco
              call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
912           format (5x,"Oren-Spedicato factor (not used) = ",
     /                d10.3,5x,"diagonal: average value = ",d10.3)
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
          if (impres.ge.1) then
            write (bufstr,913) iter
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            write (bufstr,9130) hp0
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
913       format (" >>> n1qn3 (iteration ",i2,"): ")
9130      format (5x," the search direction d is not a",
     /             "descent direction: (g,d) = ",d12.5)
          goto 1000
      endif
      if (impres.ge.4) then
          call prosca (n,g,g,ps,izs,rzs,dzs)
          ps=dsqrt(ps)
          call prosca (n,d,d,ps2,izs,rzs,dzs)
          ps2=dsqrt(ps2)
          ps=hp0/ps/ps2
          ps=dmin1(-ps,1.d+0)
          ps=dacos(ps)
          r1=ps*180.d0/pi
          write (bufstr,914) sngl(r1)
          call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
914       format (" n1qn3: descent direction d: ",
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
