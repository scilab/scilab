      subroutine rlbd(indrl,n,simul,x,binf,bsup,f,hp,t,tmax,d,gn,
     &  tproj,amd,amf,imp,io,zero,nap,napmax,xn,izs,rzs,dzs)
c
c!but
c       subroutine de recherche lineaire pour des problemes avec
c       contraintes de borne (traitees par projection)
c       le critere de retour est une extension de celui de wolfe
c!origine
c       f.bonnans  inria juin 1988
c     Copyright INRIA
c!methode
c     pour chaque valeur du parametre t , sont calcules le critere
c     et son gradient.
c     une phase d extrapolation permet d obtenir un encadrement.
c     l intervalle est ensuite reduit suivant les cas par une methode
c     de dichotomie, d interpolation lineaire sur les derivees ou
c     d interpolation cubique.
c
c!impressions
c      si imp > 2 , rlbd fournit les impressions suivantes :
c
c      la premiere ligne indique :
c      t      premiere valeur de t fournie en liste d' appel
c      tproj  plus petit t > 0 pour lequel on bute sur une borne
c      dh/dt  derivee en zero de h(t)=f(x+t*d)-f(x)
c      tmax   valeur maximale de t fournie en liste d' appel
c
c      lignes suivantes :
c      chaine de caracteres en debut de ligne : indique comment sera calcule
c      le pas de la ligne suivante ;
c      ic : interpolation cubique
c      s  : saturation d une variable sur une borne
c      id : interpolation lineaire sur la derivee
c      e  : extrapolation
c      d  :interpolation cubique ayant echouee t est calcule par dichotomie
c      b  :sauvegarde de convergence active
c
c!subroutines utilisees
c     proj et satur (bibl. modulopt)
c!liste d appel
c
c      subroutine rlbd(indrl,n,simul,proj,x,binf,bsup,f,hp,t,tmax,d,gn,
c     &  tproj,amd,amf,imp,io,zero,nap,napmax,xn,izs,rzs,dzs)
c
c      e;s;e,s:parametres initialises en entree,en sortie,en entree et
c              en sortie
c      indrl<0:la recherche lineaire n a pas trouve de meilleur pas(e,s)
c           =0:arret demande par l'utilisateur dans simul
c           >0:meilleur pas fourni avec f et g
c           >9:meilleur pas fourni avec f et sans g
c           =14:deltat trop petit
c           =13:nap=napmax
c           =8:toutes les variables sont saturees
c           =4:deltat trop petit
c           =3:nap=napmax
c           =2:t=tmax
c           =1:descente serieuse avec t<tmax
c           =0:arret demande par l'utilisateur
c           =-3:nap=napmax
c           =-4:deltat trop petit
c           =-1000+indic:nap=napmax et indic<0
c      n:dimension de x                                    (e,s)
c      simul: subroutine fournissant le critere et le gradient (e)
c      x:valeur initiale de la variable a optimiser en entree;valeur a
c        l optimum en sortie.                                     (e,s)
c      binf,bsup:bornes inf et sup de dimension n                 (e,s)
c      f:valeur du critere en x                            (e,s)
c      hp:derivee de f(x+t*d) par rapport a t en 0         (e)
c      t:pas                                               (e)
c      tmax:valeur maximal du pas                          (e,s)
c      d:direction de descente                             (e)
c      gn: gradient de f en xn                             (e,s)
c      tproj:plus petit pas saturant une nouvelle contrainte(e,s)
c      amf,amd:constantes du test de wolfe                 (e)
c      imp<=2:pas d'impression                             (e)
c         >=3:une impression par calcul de simul           (e)
c      io:numero du fichier resultat                       (e)
c      zero:proche du zero machine                         (e)
c      nap:nombre d'appel a simul                          (e)
c      napmax:nombre maximum d'appel a simul               (e)
c      xn:tableau de travail de dimension n (=x+t*d)
c      izs,rzs,dzs:cf norme modulopt                       (e,s)
c!
c       parametres de l algorithme
c      eps1:sauvegarde de conv.dans l interpolation lineaire sur la derivee
c      eps:sauvegarde de conv.dans la l interpolation par saturation
c          d une contrainte.
c      epst:sauvegerde de conv.dans l interpolation cubique
c      extra,extrp:servent a calculer la limite sur la variation relative
c      de t dans l extrapolation et l interpolation lineaire sur la derivee
c      cofder: intervient dans le choix entre les methodes d' interpolation
c
c        variables de travail
c      fn:valeur du critere en xn
c      hpn:derivee de f(x+t*d) par rapport a t
c      hpd:valeur de hpn a droite
c      hpg:valeur de hpn a gauche
c      td:pas trop grand
c      tg:pas trop petit
c      tproj:plus petit pas saturant une contrainte
c      tmaxp:plus grand pas saturant une contraite
c      ftd:valeur de f en x+td*d
c      ftg:valeur de f en x+tg*d
c      hptd:valeur de hpn en x+td*d
c      hptg:valeur de hpn en x+tg*d
c      imax=1:tmax a ete atteint
c          =0:tmax n a pas ete atteint
c      icos:indice de la variable saturee par la borne superieure
c      icoi:indice de la variable saturee par la borne inferieure
c      ico1:indice de la variable saturee en tmaxp
c      icop:indice de la variable saturee en tproj
c
      implicit double precision(a-h,o-z)
      external simul,proj
      character var2*3
      dimension x(n),xn(n),gn(n),d(n),binf(n),bsup(n),izs(*)
      double precision dzs(*)
      real rzs(*)
c
      indrl=1
      eps1=.90d+0
      eps=.10d+0
      epst=.10d+0
      extrp=100.0d+0
      extra=10.0d+0
      cofder=100.
      var2='   '
c
      ta1=0.0d+0
      f0=f
      fa1=f
      hpta1=hp
      imax=0
      hptg=hp
      ftg=f
      tg=0.0d+0
      td=0.0d+0
      icos=0
      icoi=0
      icop=0
c
c     calcul de tproj:plus petit point de discontinuite de h'(t)
      tproj=0.0d+0
      do 7 i=1,n
      CRES=d(i)
      if (CRES .lt. 0) then
         goto 4
      elseif (CRES .eq. 0) then 
         goto 7
      else
         goto 5
      endif
4     t2=(binf(i)-x(i))/d(i)
      go to 6
5     t2=(bsup(i)-x(i))/d(i)
6     if (t2.le.0.0d+0) go to 7
      if (tproj.eq.0.0d+0) tproj=t2
      if (t2.gt.tproj) go to 7
      tproj=t2
      icop=i
7     continue
c
      if (imp.ge.3) write (io,14050) tproj,tmax,hp
14050 format (' rlbd tp=',e11.4,
     &        ' tmax=',e11.4,' dh0/dt=',e11.4 )
15000 format (a3,' t=',e11.4,' h=',e11.4,' dh/dt=',e11.4,
     & ' dfh/dt=', e11.4,' dt',e8.1)
15020 format (3x,' t=',e11.4,' h=',e11.4,' dh/dt=',e11.4,
     & ' dfh/dt=', e11.4,' dt',e8.1)
16000 format (' rlbd : sortie du domaine : indic=',i2,'  t=',e11.4)
c
c              boucle
c
c     calcul de xn,de fn et de gn
200   if (nap.ge.napmax) then
        k=3
        goto 1000
      end if
      do 230 i=1,n
230   xn(i)=x(i)+t*d(i)
      call proj (n,binf,bsup,xn)
      if (icos.gt.0) xn(icos)=bsup(icos)
      if (icoi.gt.0) xn(icoi)=binf(icoi)
      indic=4
      call simul (indic,n,xn,fn,gn,izs,rzs,dzs)
      nap=nap+1
      if (indic.lt.0) then
         if (imp.ge.3) write (io,16000) indic,t
         if (nap.ge.napmax) go to 1000
         t=tg+(t-tg)/4.0d+0
         tmax=t
         imax=1
         icoi=0
         icos=0
         var2='dd '
         go to 800
      endif
      if(indic.eq.0) then
         indrl=0
         go to 1010
      endif
c
c      calcul de hpg et hpd
      hpg=0.0d+0
      do 242 i=1,n
242   xn(i)=x(i)+t*d(i)
      if (icoi.gt.0) xn(icoi)=bsup(icoi)
      if (icos.gt.0) xn(icos)=bsup(icos)
      call proj(n,binf,bsup,xn)
      do 244 i=1,n
      xni=xn(i)
244   if(binf(i).lt.xni.and.xni.lt.bsup(i)) hpg=hpg + d(i)*gn(i)
      hpd=hpg
      if(icoi.gt.0) hpg=hpg + d(icoi)*gn(icoi)
      if(icos.gt.0) hpg=hpg + d(icos)*gn(icos)
c
      icoi=0
      icos=0
      if((hpd.ne.0.0d+0).or.(hpg.ne.0.0d+0)) go to 360
c
c      la derivee de h est nulle
c      calcul du pas saturant toutes les bornes:tmaxp
      tmaxp=0.0d+0
      ico1=0
      do 350 i=1,n
      CRES=d(i)
      if (CRES .lt. 0) then 
         goto 310
      elseif (CRES .eq. 0) then
         goto 350
      else
         goto 320
      endif
310   t2=(binf(i)-x(i))/d(i)
      go to 330
320   t2=(bsup(i)-x(i))/d(i)
330   if (t2.le.0.0d+0) go to 350
      if (tmaxp.eq.0.0d+0) tmaxp=t2
      if (tmaxp.gt.t2)go to 350
      tmaxp=t2
      ico1=i
350   continue
      if (t.lt.tmaxp) then
         if(fn.le.f+amf*hp*t) goto 1010
         t=t/10.0d+0
         var2='d  '
         goto 800
      end if
      icos=ico1
      icoi=0
      if (d(ico1).lt.0.0d+0) then
      icoi=ico1
      icos=0
      end if
c
c     toutes les variables sont saturees
      if (imp.ge.3) write (io,3330) tmaxp
3330   format ('toutes les variables sont saturees:tmaxp= ',e11.4)
      t=tmaxp
      if (fn.lt.f+amf*hp*tmaxp)  then
      indrl=8
      goto 1010
      end if
      hpg=d(ico1)*gn(ico1)
      if ((fn.lt.f).and.(hpg.lt.0.0d+0)) then
      indrl=8
      goto 1010
      end if
360    continue
c
c       test de wolfe
c
      a=f+amf*hp*t
      if (fn.gt.a) then
c      le pas est trop grand
c       (dans le cas quadratique changer eps1 et extra si td<tproj)
            td=t
            t1=t-ta1
            h1=(fn-fa1)/t1
            ftd=fn
            hptd=hpg
            ta=tg
            hpn=hptd
            hpa=hptg
            fa=ftg
      else
            if (hpd.ge.(amd*hp)) go to 1010
c      le pas est trop petit
            tg=t
            t1=t-ta1
            h1=(fn-fa1)/t1
            ftg=fn
            hptg=hpd
            ta=td
            hpn=hptg
            hpa=hptd
            fa=ftd
            if (td.eq.0.0d+0) go to 700
            a1=abs(hptd/hp)
            if ((a1.gt.cofder).and.(ftd.gt.f).and.(hptg.gt.(.99*hp)))
     &       then
                hpta1=hp
                fa1=f
                ta1=0.0d+0
                go to 700
             end if
      endif
      a1=abs(hpn/hp)
      if ((tg.ne.0.0d+0).or.(fn.le.f).or.(a1.le.cofder).or.
     & (hpn.lt.0.0d+0)) then
        if (td.le.tproj) go to 600
        go to 500
      end if
c
c       calcul du nouveau t
c
c      par interpolation lineaire sur la derivee
c
      ta1=t
      fa1=fn
      div=hp-hptd
      text=t/10.0d+0
      if(abs(div).gt.zero) text=t*(hp/div)
      if (text.gt.tproj) text=t/10.0d+0
      text=max(text,t/(extrp*extra))
      t=min(text,t*eps1)
      ttsup=1.50d+0*t
      extrp=10.
      if (tproj.gt.ta1) then
          var2='id '
          go to 800
      end if
      ttmin=0.70d+0*t
      tmi=t
      topt=0.0d+0
      iproj=0
      call satur(n,x,binf,bsup,d,ttmin,ttsup,topt,tg,td,tmi,
     &            icoi,icos,iproj)
      var2='id '
      if (topt.ne.0.0d+0) then
         t=topt
         var2='ids'
      end if
      go to 800
c
c      interpolation par saturation d une contrainte
c
500   if (td.le.tproj) go to 600
      topt=0.0d+0
      iproj=1
      ta1=t
      fa1=fn
      ttmin=tg+eps*(td-tg)
      ttsup=td-eps*(td-tg)
      tmi=(td+tg)/2.0d+0
      call satur(n,x,binf,bsup,d,ttmin,ttsup,topt,tg,td,tmi,
     &            icoi,icos,iproj)
      if (topt.eq.0.0d+0) go to 600
      t=topt
      var2='s  '
      if (t.eq.ttsup.or.t.eq.ttmin) var2='sb '
      go to 800
c
c      interpolation cubique
c
c      test de securite
600   if ((td-tg).lt.1.0d+2*zero) then
        k=4
        goto 1000
      end if
c
c      calcul du minimum
      b=1.0d+0
      p=hpn+hpa-3.0d+0*(fn-fa)/(t-ta)
      di=p*p-hpn*hpa
      if (di.lt.0.0d+0) go to 690
      if ((t-ta).lt.0.0d+0) b=-1
      div=hpn+p+b*sqrt(di)
      if (abs(div).le.zero) go to 690
      r=hpn/div
      topt=t-r*(t-ta)
      if ((topt.lt.tg).or.(topt.gt.td))go to 690
c
c      sauvegarde de convergence
      e=epst*(td-tg)
      var2='ic '
      if (topt.gt.(td-e)) then
        topt=td-e
        var2='icb'
      end if
      if (topt.lt.(tg+e)) then
         topt=tg+e
         var2='icb'
      end if
      ta1=t
      fa1=fn
      t=topt
      goto 800
690   ta1=t
      fa1=fn
      t=0.50d+0*(tg+td)
      var2='d  '
      go to 800
c
c      extrapolation
c
700   if (imax.ge.1) then
        k=2
        goto 1000
      end if
      text=10.0d+0*t
      difhp=hptg-hpta1
      if (difhp.gt.zero)then
        text=(amd*hp/3.0d+0-hptg)*((tg-ta1)/difhp)+tg
        if ((td.ne.0.0d+0).and.(text.ge.td)) go to 600
c        dans le cas quadratique prendre extrp plus grand
        text=min(text,extra*extrp*t)
        text=max(text,2.50d+0*t)
      else
        text=extra*extrp*t
      end if
      ta1=t
      fa1=fn
      hpta1=hpn
      extrp=10.
      if (text.ge.tmax/2.0d+0) then
        text=tmax
        imax=1
      end if
      if ((t.lt.tproj).and.(text.gt.tproj)) then
        t=max(tproj,2.50d+0*t)
        icoi=0
        icos=icop
        if(d(icop).lt.0.0d+0) then
          icoi=icop
          icos=0
        end if
        var2='es '
        goto 800
      end if
      ttsup=min(1.50d+0*text,tmax)
      if (ttsup.lt.tproj) go to 785
      ttmin=2*t
      iproj=0
      tmi=text
      topt=0.0d+0
      call satur(n,x,binf,bsup,d,ttmin,ttsup,topt,tg,td,tmi,
     &            icoi,icos,iproj)
      if (topt.gt.0.0d+0)then
          t=topt
          var2='es '
          go to 800
      endif
785   t=text
      var2='e  '
800   f11=fn-f
      if (imp.ge.3.and.indic.gt.0)
     &    write (io,15000)var2,ta1,f11,hpn,h1,t1
c
c      test sur deltat
      if(abs(ta1-t).ge.1.0d+2*zero) go to 200
      k=4
c      calcul de indrl
1000  if (indic.lt.0) then
        indrl=13
        if (tg.eq.0.0d+0) indrl=-1000+indic
        fn=ftg
        hpn=hptg
        t=tg
        go to 1010
      endif
      if (fn.le.ftg) then
      indrl=k
      t=tg
      go to 1010
      end if
      if (tg.eq.0.0d+0) then
       indrl=-1*k
      go to 1010
      end if
      indrl=10+k
      t=tg
      fn=ftg
      hpn=hptg
c
c      fin du programme
1010  f=fn
      do 810 i=1,n
810   x(i)=x(i)+t*d(i)
      call proj(n,binf,bsup,x)
      if (icos.gt.0) x(icos)=bsup(icos)
      if (icoi.gt.0) x(icoi)=binf(icoi)
c
      if (indrl.lt.0) then
        nap=nap+1
        indic=4
        call simul (indic,n,x,f,gn,izs,rzs,dzs)
      endif
c
      t1=t-ta1
      if (t1.eq.0.0d+0) then
      t1=1.
      end if
      h1=(fn-fa1)/t1
      hp=hpd
      f0=f-f0
      if (imp.ge.3) write (io,15020)t,f0,hpd,h1,t1
      return
      end
