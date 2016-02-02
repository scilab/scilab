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
      subroutine n1qn2a (simul,prosca,n,x,f,g,dxmin,df1,epsg,
     /                   impres,io,mode,niter,nsim,m,
     /                   d,gg,aux,alpha,ybar,sbar,izs,rzs,dzs)
c
c----
c
c     code d'optimisation proprement dit
c
c----
c
c     arguments
c
      integer n,impres,io,mode,niter,nsim,m,izs(*)
      real rzs(*)
      double precision x(n),f,g(n),dxmin,df1,epsg,d(n),gg(n),aux(n),
     /     alpha(m),ybar(n,m),sbar(n,m)
      double precision dzs(*)
      external simul,prosca
c
c     variables locales
c
      integer i,iter,moderl,isim,jmin,jmax,indic
      real r
      double precision d1,t,tmin,tmax,gnorm,eps1,precon,ff
      double precision ps,ps2,hp0
      character bufstr*(4096)
c
c---- parametres
c
      double precision rm1,rm2
      parameter (rm1=0.90d+0,rm2=0.10d-3)
      double precision pi
      parameter (pi=3.14159270d+0)
c
c---- initialisation
c
      iter=0
      isim=1
      call prosca (n,g,g,ps,izs,rzs,dzs)
      gnorm=sqrt(ps)
      if (impres.ge.1) then
        write (bufstr,900) f,gnorm
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
900   format (5x,'f          = ',d15.8,
     /      /,5x,'norme de g = ',d15.8)
c
c     ---- direction de descente initiale
c          (avec mise a l'echelle)
c
      precon=2.0d+0*df1/gnorm**2
      do 10 i=1,n
          d(i)=-g(i)*precon
10    continue
      if (impres.ge.5) then
        write(bufstr,899) precon
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
899   format (' n1qn2a: direction de descente -g: precon = ',d10.3)
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
      tmax=1.0d+20
      call prosca (n,d,g,hp0,izs,rzs,dzs)
c
c     ---- initialisation pour strang
c
      jmin=1
      jmax=0
c
c---- debut de l'iteration. on cherche x(k+1) de la forme x(k) + t*d,
c     avec t > 0. on connait d.
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
      if (impres.ge.5) then
        write(bufstr,901)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
901   format (1x,79('-'))
      if (impres.ge.4) then
        write(bufstr,9010)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
9010  format (1x,' ')
      if (impres.ge.3) then
        write (bufstr,902) iter,isim
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))

        write (bufstr,9020) f,hp0
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))

        endif
902   format (' n1qn2: iter ',i3,', simul ',i3)
9020  format (', f=',d15.8,', h''(0)=',d12.5)
      do 101 i=1,n
          gg(i)=g(i)
101   continue
      ff=f
c
c     ---- recherche lineaire et nouveau point x(k+1)
c
      if (impres.ge.5) then
        write (bufstr,903)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
903   format (/,' n1qn2: recherche lineaire')
c
c         ---- calcul de tmin
c
      tmin=0.0d+0
      do 200 i=1,n
          tmin=max(tmin,abs(d(i)))
200   continue
      tmin=dxmin/tmin
      t=1.0d+0
      d1=hp0
c
      call nlis0 (n,simul,prosca,x,f,d1,t,tmin,tmax,d,g,rm1,rm2,
     /           impres,io,moderl,isim,nsim,aux,izs,rzs,dzs)
c
c         ---- nlis0 renvoie les nouvelles valeurs de x, f et g
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
904           format (/,' >>> n1qn2 (iteration ',i3,
     /                '): recherche lineaire bloquee sur tmax: ',
     /                'reduire l''echelle')
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
      if (impres.ge.5) then
        write (bufstr,905) eps1
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
905   format (/,' n1qn2: test d''arret sur g: ',d12.5)
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
906       format (/,' >>> n1qn2 (iteration ',i3,
     /            '): nombre maximal d''iterations atteint')
          goto 1000
      endif
      if (isim.ge.nsim) then
          mode=5
          if (impres.ge.1) then
            write (bufstr,907) iter,isim
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
907       format (/,' >>> n1qn2 (iteration ',i3,'): ',i6,
     /            ' appels a simul (nombre maximal atteint)')
          goto 1000
      endif
c
c     ---- mise a jour de la matrice
c
      jmax=jmax+1
      if (iter.gt.m) then
          jmin=jmin+1
          if (jmin.gt.m) jmin=jmin-m
          if (jmax.gt.m) jmax=jmax-m
      endif
c
c         ---- y, s et (y,s)
c
      do 400 i=1,n
          sbar(i,jmax)=t*d(i)
          ybar(i,jmax)=g(i)-gg(i)
400   continue
      call prosca (n,ybar(1,jmax),sbar(1,jmax),d1,izs,rzs,dzs)
      if (d1.le.0.0d+0) then
          mode=7
          if (impres.ge.1) then
            write (bufstr,908) iter,d1
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
908       format (/,' >>> n1qn2 (iteration ',i2,
     /            '): le produit scalaire (y,s) = ',d12.5,
     /            /,27x,'n''est pas positif')
          goto 1000
      endif
c
c         ---- precon: facteur de mise a l'echelle
c
      call prosca (n,ybar(1,jmax),ybar(1,jmax),ps,izs,rzs,dzs)
      precon=d1/ps
      if (impres.ge.5) then
        write (bufstr,909) d1,precon
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
909   format (/,' n1qn2: mise a jour: (y,s) = ',d10.3,
     /          ' Oren-Spedicato = ',d10.3)
c
c         ---- ybar, sbar
c
      d1=sqrt(1.0d+0/d1)
      do 410 i=1,n
          sbar(i,jmax)=d1*sbar(i,jmax)
          ybar(i,jmax)=d1*ybar(i,jmax)
410   continue
c
c     ---- calcul de la nouvelle direction de descente d = - h.g
c
      do 510 i=1,n
          d(i)=-g(i)
510   continue
      call strang(prosca,n,m,d,jmin,jmax,
     /           precon,alpha,ybar,sbar,izs,rzs,dzs)
c
c         ---- test: la direction d est-elle de descente ?
c             hp0 sera utilise par nlis0
c
      call prosca (n,d,g,hp0,izs,rzs,dzs)
      if (hp0.ge.0.0d+0) then
          mode=7
          if (impres.ge.1) then
            write (bufstr,910) iter,hp0
            call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
            endif
910       format (/,' >>> n1qn2 (iteration ',i2,'): ',
     /            /,5x,'la direction de recherche d n''est pas de ',
     /             'descente: (g,d) = ',d12.5)
          goto 1000
      endif
      if (impres.ge.5) then
          call prosca (n,g,g,ps,izs,rzs,dzs)
          ps=sqrt(ps)
          call prosca (n,d,d,ps2,izs,rzs,dzs)
          ps2=sqrt(ps2)
          ps=hp0/ps/ps2
          ps=min(-ps,1.0d+0)
          ps=acos(ps)
          r=real(ps*180./pi)
          write (bufstr,911) r
          call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
911       format (/,' n1qn2: direction de descente d: ',
     /              'angle(-g,d) = ',f5.1,' degres')
      endif
c
c---- on poursuit les iterations
c
      goto 100
c
c         retour
c
1000  epsg=eps1
      niter=iter
      nsim=isim
      return
      end
