      subroutine nlis0 (n,simul,prosca,xn,fn,fpn,t,tmin,tmax,d,g,
     /                  amd,amf,imp,io,logic,nap,napmax,x,izs,rzs,dzs)
c     Copyright INRIA

c----
c
c     nlis0 + minuscules + commentaires
c     ---------------------------------
c
c        en sortie logic =
c
c        0          descente serieuse
c        1          descente bloquee
c        4          nap > napmax
c        5          retour a l'utilisateur
c        6          fonction et gradient pas d'accord
c        < 0        contrainte implicite active
c
c----
c
c --- arguments
c
      external simul,prosca
      integer n,imp,io,logic,nap,napmax,izs(*)
      real rzs(*)
      double precision xn(n),fn,fpn,t,tmin,tmax,d(n),g(n),amd,amf,x(n)
      double precision dzs(*)
c
c --- variables locales
c
      integer i,indic,indica,indicd
      double precision tesf,tesd,tg,fg,fpg,td,ta,fa,fpa,d2,f,fp,ffn,fd,
     /    fpd,z,z1,test
c
 1000 format (/4x,9h nlis0   ,4x,4hfpn=,d10.3,4h d2=,d9.2,
     1 7h  tmin=,d9.2,6h tmax=,d9.2)
 1001 format (/4x,6h nlis0,3x,12hfin sur tmin,8x,
     1 3hpas,12x,9hfonctions,5x,8hderivees)
 1002 format (4x,6h nlis0,37x,d10.3,2d11.3)
 1003 format (4x,6h nlis0,d14.3,2d11.3)
 1004 format (4x,6h nlis0,37x,d10.3,7h indic=,i3)
 1005 format (4x,6h nlis0,14x,2d18.8,d11.3)
 1006 format (4x,6h nlis0,14x,d18.8,12h      indic=,i3)
 1007 format (/4x,6h nlis0,10x,17htmin force a tmax)
 1008 format (/4x,6h nlis0,10x,16happel incoherent)
      if (n.gt.0 .and. fpn.lt.0.d+0 .and. t.gt.0.d+0
     1 .and. tmax.gt.0.d+0 .and. amf.gt.0.d+0
     1 .and. amd.gt.amf .and. amd.lt.1.d+0) go to 5
      logic=6
      go to 999
    5 tesf=amf*fpn
      tesd=amd*fpn
      td=0.d+0
      tg=0.d+0
      fg=fn
      fpg=fpn
      ta=0.d+0
      fa=fn
      fpa=fpn
      call prosca (n,d,d,d2,izs,rzs,dzs)
c
c               elimination d'un t initial ridiculement petit
c
      if (t.gt.tmin) go to 20
      t=tmin
      if (t.le.tmax) go to 20
      if (imp.gt.0) write (io,1007)
      tmin=tmax
   20 if (fn+t*fpn.lt.fn+0.9d+0*t*fpn) go to 30
      t=2.d+0*t
      go to 20
   30 indica=1
      logic=0
      if (t.gt.tmax) then
          t=tmax
          logic=1
      endif
      if (imp.ge.4) write (io,1000) fpn,d2,tmin,tmax
c
c     --- nouveau x
c
      do 50 i=1,n
          x(i)=xn(i)+t*d(i)
   50 continue
c
c --- boucle
c
  100 nap=nap+1
      if(nap.gt.napmax) then
          logic=4
          fn=fg
          do 120 i=1,n
              xn(i)=xn(i)+tg*d(i)
  120     continue
          go to 999
      endif
      indic=4
c
c     --- appel simulateur
c
      call simul(indic,n,x,f,g,izs,rzs,dzs)
      if(indic.eq.0) then
c
c         --- arret demande par l'utilisateur
c
          logic=5
          fn=f
          do 170 i=1,n
              xn(i)=x(i)
  170     continue
          go to 999
      endif
      if(indic.lt.0) then
c
c         --- les calculs n'ont pas pu etre effectues par le simulateur
c
          td=t
          indicd=indic
          logic=0
          if (imp.ge.4) write (io,1004) t,indic
          t=tg+0.1d+0*(td-tg)
          go to 905
      endif
c
c     --- les tests elementaires sont faits, on y va
c
      call prosca (n,d,g,fp,izs,rzs,dzs)
c
c     --- premier test de Wolfe
c
      ffn=f-fn
      if(ffn.gt.t*tesf) then
          td=t
          fd=f
          fpd=fp
          indicd=indic
          logic=0
          if(imp.ge.4) write (io,1002) t,ffn,fp
          go to 500
      endif
c
c     --- test 1 ok, donc deuxieme test de Wolfe
c
      if(imp.ge.4) write (io,1003) t,ffn,fp
      if(fp.gt.tesd) then
          logic=0
          go to 320
      endif
      if (logic.eq.0) go to 350
c
c     --- test 2 ok, donc pas serieux, on sort
c
  320 fn=f
      do 330 i=1,n
          xn(i)=x(i)
  330 continue
      go to 999
c
c
c
  350 tg=t
      fg=f
      fpg=fp
      if(td.ne.0.d+0) go to 500
c
c              extrapolation
c
      ta=t
      t=9.d+0*tg
      z=fpn+3.d+0*fp-4.d+0*ffn/tg
      if(z.gt.0.d+0) t=dmin1(t,tg*dmax1(1.d+0,-fp/z))
      t=tg+t
      if(t.lt.tmax) go to 900
      logic=1
      t=tmax
      go to 900
c
c              interpolation
c
  500 if(indica.le.0) then
          ta=t
          t=0.9d+0*tg+0.1d+0*td
          go to 900
      endif
      z=fp+fpa-3.d+0*(fa-f)/(ta-t)
      z1=z*z-fp*fpa
      if(z1.lt.0.d+0) then
          ta=t
          t=0.5d+0*(td+tg)
          go to 900
      endif
      if(t.lt.ta) z1=z-dsqrt(z1)
      if(t.gt.ta) z1=z+dsqrt(z1)
      z=fp/(fp+z1)
      z=t+z*(ta-t)
      ta=t
      test=0.1d+0*(td-tg)
      t=dmax1(z,tg+test)
      t=dmin1(t,td-test)
c
c --- fin de boucle
c     - t peut etre bloque sur tmax
c       (venant de l'extrapolation avec logic=1)
c
  900 fa=f
      fpa=fp
  905 indica=indic
c
c --- faut-il continuer ?
c
      if (td.eq.0.d+0) go to 950
      if (td-tg.lt.tmin) go to 920
c
c     --- limite de precision machine (arret de secours) ?
c
      do 910 i=1,n
          z=xn(i)+t*d(i)
          if (z.ne.xn(i).and.z.ne.x(i)) go to 950
  910 continue
c
c --- arret sur dxmin ou de secours
c
  920 logic=6
c
c     si indicd<0, les derniers calculs n'ont pas pu etre fait par simul
c
      if (indicd.lt.0) logic=indicd
c
c     si tg=0, xn = xn_depart,
c     sinon on prend xn=x_gauche qui fait decroitre f
c
      if (tg.eq.0.d+0) go to 940
      fn=fg
      do 930 i=1,n
  930 xn(i)=xn(i)+tg*d(i)
  940 if (imp.le.0) go to 999
      write (io,1001)
      write (io,1005) tg,fg,fpg
      if (logic.eq.6) write (io,1005) td,fd,fpd
      if (logic.eq.7) write (io,1006) td,indicd
      go to 999
c
c               recopiage de x et boucle
c
  950 do 960 i=1,n
  960 x(i)=xn(i)+t*d(i)
      go to 100
  999 return
      end
