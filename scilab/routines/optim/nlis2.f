      subroutine nlis2 (simul,prosca,n,xn,fn,fpn,t,tmin,tmax,d,d2,g,gd,
     1     amd,amf,imp,io,logic,nap,napmax,x,tol,a,tps,tnc,gg,izs,rzs
     $     ,dzs)
c cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     Copyright INRIA
c cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c subroutine effectuant une recherche lineaire sur 0 tmax
c partant du point xn dans la direction d.
c sous l'hypothese d'hemiderivabilite, donne
c un pas serieux, bloque, nul ou semi serieux-nul (2 gradients).
c necessite fpn < 0 estimant la derivee a l'origine.
c appelle simul systematiquement avec indic = 4
c
c  logic
c        0          descente serieuse
c        1          descente bloquee
c        2          pas semiserieux-nul
c        3          pas nul, enrichissement du faiseau
c        4          nap > napmax
c        5          retour a l'utilisateur
c        6          non hemi-derivable (au-dela de dx)
c        < 0        contrainte implicite active
c
c        imp
c                   =0 pas d'impressions
c                   >0 message en cas de fin anormale
c                   >3 informations pour chaque essai de t
c            ----------------------------------------
c fait appel aux subroutines:
c -------simul(indic,n,x,f,g,izs,rzs,dzs)
c -------prosca(n,u,v,ps,izs,rzs,dzs)
c cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      implicit double precision (a-h,o-z)
      external simul,prosca
      dimension xn(n),d(n),g(n),x(n),izs(*),dzs(*),gg(n),gd(n)
      real rzs(*)
      dimension d3(1),d4(1),i5(1)
c
c     initialisations
c
      tesf=amf*fpn
      tesd=amd*fpn
      td=0.d0
      tg=0.d0
      fg=fn
      fpg=fpn
      ta=0.d0
      fa=fn
      fpa=fpn
      indica=1
      logic=0
c          elimination d'un t initial ridiculement petit
      if (t.gt.tmin) go to 20
      t=tmin
      if (t.le.tmax) go to 20
      if (imp.gt.0) call n1fc1o(io,35,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      tmin=tmax
   20 if (fn+t*fpn.lt.fn+0.9d0*t*fpn) go to 30
      t=2.d0*t
      go to 20
c
   30 if(t.lt.tmax) go to 40
      t=tmax
      logic=1
   40 if (imp.ge.4) call n1fc1o(io,36,i1,i2,i3,i4,i5,fpn,d2,tmin,tmax)
      do 50 i=1,n
   50 x(i)=xn(i)+t*d(i)
c
c                           boucle
c
  100 nap=nap+1
      if(nap.le.napmax) go to 150
c                sortie par maximum de simulations
      logic=4
      if(imp.ge.4) call n1fc1o(io,37,nap,i2,i3,i4,i5,d1,d2,d3,d4)
      if (tg.eq.0.d0) go to 999
      fn=fg
      do 120 i=1,n
      g(i)=gg(i)
  120 xn(i)=xn(i)+tg*d(i)
      go to 999
  150 indic=4
      call simul(indic,n,x,f,g,izs,rzs,dzs)
      if(indic.ne.0) go to 200
c
c                arret demande par l'utilisateur
      logic=5
      fn=f
      do 170 i=1,n
  170 xn(i)=x(i)
      if(imp.ge.4)call n1fc1o(io,38,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      go to 999
c
c                les tests elementaires sont faits, on y va
c                tout d'abord, ou en sommes nous ?
c
  200 if(indic.gt.0) go to 210
      td=t
      indicd=indic
      logic=0
      if (imp.ge.4) call n1fc1o(io,39,indic,i2,i3,i4,i5,t,d2,d3,d4)
      t=tg+0.1d0*(td-tg)
      go to 905
c
c                calcul de la derivee directionnelle h'(t)
  210 call prosca(n,g,d,fp,izs,rzs,dzs)
c
c         test de descente (premiere inegalite pour un pas serieux)
      ffn=f-fn
      if(ffn.lt.t*tesf) go to 300
      td=t
      fd=f
      fpd=fp
      do 230 i=1,n
  230 gd(i)=g(i)
      indicd=indic
      logic=0
      if(imp.ge.4) call n1fc1o(io,40,i1,i2,i3,i4,i5,t,ffn,fp,d4)
      if(tg.ne.0.) go to 500
c                tests pour un pas nul (si tg=0)
      if(fpd.lt.tesd) go to 500
      tps=(fn-f)+td*fpd
      tnc=d2*td*td
      p=max(a*tnc,tps)
      if(p.gt.tol) go to 500
      logic=3
      go to 999
c
c                    descente
  300 if(imp.ge.4) call n1fc1o(io,41,i1,i2,i3,i4,i5,t,ffn,fp,d4)
c
c         test de derivee (deuxieme inegalite pour un pas serieux)
      if(fp.lt.tesd) go to 320
c
c                sortie, le pas est serieux
      logic=0
      fn=f
      fpn=fp
      do 310 i=1,n
  310 xn(i)=x(i)
      go to 999
c
  320 if (logic.eq.0) go to 350
c
c                sortie par descente bloquee
      fn=f
      fpn=fp
      do 330 i=1,n
  330 xn(i)=x(i)
      go to 999
c
c                on a une descente
  350 tg=t
      fg=f
      fpg=fp
      do 360 i=1,n
  360 gg(i)=g(i)
c
      if(td.ne.0.d0) go to 500
c                extrapolation
      ta=t
      t=9.d0*tg
      z=fpn+3.d0*fp-4.d0*ffn/tg
      if(z.gt.0.d0) t=dmin1(t,tg*dmax1(1.d0,-fp/z))
      t=tg+t
      if(t.lt.tmax) go to 900
      logic=1
      t=tmax
      go to 900
c
c                interpolation
c
  500 if(indica.gt.0 .and. indicd.gt.0) go to 510
      ta=t
      t=0.9d0*tg+0.1d0*td
      go to 900
  510 test=0.1d0*(td-tg)
c                approximation cubique
      ps=fp+fpa-3.d0*(fa-f)/(ta-t)
      z1=ps*ps-fp*fpa
      if (z1.ge.0.d0) go to 520
      if (fp.lt.0.d0) tc=td
      if (fp.ge.0.d0) tc=tg
      go to 600
  520 z1=dsqrt(z1)
      if (t-ta.lt.0.d0) z1=-z1
      sign=(t-ta)/dabs(t-ta)
      if ((ps+fp)*sign.gt.0.d0) go to 550
      den=2.d0*ps+fp+fpa
      anum=z1-fp-ps
      if (dabs((t-ta)*anum).ge.(td-tg)*dabs(den)) go to 530
      tc=t+anum*(ta-t)/den
      go to 600
  530 tc=td
      go to 600
  550 tc=t+fp*(ta-t)/(ps+fp+z1)
  600 mc=0
      if (tc.lt.tg) mc=-1
      if (tc.gt.td) mc=1
      tc=max(tc,tg+test)
      tc=min(tc,td-test)
c                approximation polyhedrique
      ps=fpd-fpg
      if (ps.ne.0.d0) go to 620
      tp=0.5d0*(td+tg)
      go to 650
  620 tp=((fg-fpg*tg)-(fd-fpd*td))/ps
  650 mp=0
      if (tp.lt.tg) mp=-1
      if (tp.gt.td) mp=1
      tp=max(tp,tg+test)
      tp=min(tp,td-test)
c                nouveau t par approximation cp complete securisee
      ta=t
      if (mc.eq.0  .and.  mp.eq.0) t=dmin1(tc,tp)
      if (mc.eq.0  .and.  mp.ne.0) t=tc
      if (mc.ne.0  .and.  mp.eq.0) t=tp
      if (mc.eq.1  .and.  mp.eq.1) t=td-test
      if (mc.eq.-1 .and. mp.eq.-1) t=tg+test
      if (mc*mp.eq.-1) t=0.5d0*(tg+td)
c
c                 fin de boucle
c
  900 fa=f
      fpa=fp
  905 indica=indic
c                 peut-on faire logic=2 ?
      if (td.eq.0.d0) go to 920
      if (indicd.lt.0) go to 920
      if (td-tg.gt.10.d0*tmin) go to 920
      if (fpd.lt.tesd) go to 920
      tps=(fg-fd)+(td-tg)*fpd
      tnc=d2*(td-tg)*(td-tg)
      p=max(a*tnc,tps)
      if(p.gt.tol) go to 920
c               sortie par pas semiserieux-nul
      logic=2
      fn=fg
      fpn=fpg
      t=tg
      do 910 i=1,n
      xn(i)=xn(i)+tg*d(i)
  910 g(i)=gg(i)
      go to 999
c
c                test d'arret sur la proximite de tg et td
c
  920 if (td.eq.0.d0) go to 990
      if (td-tg.le.tmin) go to 950
      do 930 i=1,n
      z=xn(i)+t*d(i)
      if (z.ne.x(i) .and. z.ne.xn(i)) go to 990
  930 continue
c                arret sur dx ou de secours
  950 logic=6
      if (indicd.lt.0) logic=indicd
      if (tg.eq.0.d0) go to 970
      fn=fg
      do 960 i=1,n
      xn(i)=xn(i)+tg*d(i)
  960 g(i)=gg(i)
  970 if (imp.le.0) go to 999
      if (logic.lt.0) call n1fc1o(io,42,logic,i2,i3,i4,i5,d1,d2,d3,d4)
      if (logic.eq.6) call n1fc1o(io,42,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      go to 999
c
c                recopiage de x et boucle
  990 do 995 i=1,n
  995 x(i)=xn(i)+t*d(i)
      go to 100
c
  999 return
      end
