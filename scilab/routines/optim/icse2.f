C/MEMBR ADD NAME=ICSE2,SSI=0
c     Copyright INRIA
      subroutine icse2(ind,nu,u,co,g,icsef,icsec2,icsei,y0,tob,obs,
     &ob,ytot,f,b,fy,fu,ipv2,itob,cof,ytob,c2y,y0u,dmy,smy,oldmu,
     &y,oldp,p,p0,gt,yob,d,itu,dtu,
     &t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
c
c     sous programme de icse.fortran:calcul du gradient par
c     integration du systeme adjoint
c
      implicit double precision (a-h,o-z)
      dimension iu(5), u(nu),g(nu),y0(ny),tob(ntob),obs(nob,ny),
     &ob(nex,ntob,nob),ytot(ny,nti+ntf),f(ny),b(ny),fy(ny,ny),
     &fu(ny,nuc+nuv),ipv2(ny),itob(ntob),cof(nob,ntob),ytob(ny,ntob),
     &c2y(ny,ntob),y0u(ny,nu),dmy(ny,ny),smy(ny,ny),
     &oldmu(ny,nuc+nuv),y(ny),oldp(ny),p(ny),p0(ny),
     &gt(nu),yob(nob,ntob),d(nob),itu(nitu),dtu(ndtu)
      external icsef,icsec2,icsei
c
      character*6 nomf,nomc,nomi
c
c     Initialisation
c
      call dset(nu,0.0d+0,g,1)
      call dset(ny,0.0d+0,p,1)
      kt=nti+ntf
      ktob=ntob
c     lui et nui servent quand l'etat initial depend du controle
      if (iu(2).gt.0) lui=min(nu,1+nuc)
      if (iu(1).gt.0) lui=1
      nui=iu(1)*nuc+iu(2)*nuv*(nti+ntf+1)
c
c
c     Calcul de itob,vecteur des indices des instants de mesure
c     a partir de tob,vecteur des instants de mesure
c     itob(j) est l'entier le plus proche de tob(j)/dt
c
      do 1 j=1,ntobi
1     itob(j)=int(((tob(j)-t0)/dti)+0.50d+0)
      if (ntobi.lt.ntob) then
        itob(ntobi+1)=nti+int(0.50d+0+(tob(ntobi+1)-t0-nti*dti)/dtf)
      endif
      if (ntobi+1.lt.ntob) then
        do 2 j=ntobi+2,ntob
2       itob(j)=itob(ntobi+1)+int(0.50d+0+(tob(j)-tob(ntobi+1))/dtf)
      endif
c
c     Ecriture de ytob tableau des valeurs de l'etat
c     aux instants de mesure
c
      do 10 j=1,ntob
      do 10 i=1,ny
10    call dcopy(ny,ytot(1,itob(j)),1,ytob(1,j),1)
c
c     Si ind=2,on calcule seulement le cout
c     Si ind=3,on calcule seulement le gradient
c     Si ind=4,on calcule le cout et le gradient
c
      if (ind.ne.3) then
        indc=1
        call icsec2(indc,nu,tob,obs,cof,ytob,ob,u,
     &  co,c2y,g,yob,d,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
        if (indc.le.0) then
          ind=indc
          return
        endif
      endif
      if (ind.eq.2) return
c
c     Calcul du gradient du cout en utilisant l'etat adjoint
c     discretise:
c     calcul de la derivee partielle c2y du cout par rapport
c     a l'etat
c
      indc=2
      call icsec2(indc,nu,tob,obs,cof,ytob,ob,u,co,c2y,g,yob,d,itu,
     &dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
      if (indc.le.0) then
        ind=indc
        return
      endif
c
c     +Evaluations successives de la contribution au gradient
c     a chaque pas de temps a l'aide de l'etat adjoint(non stocke)
c
      do 100 kt=nti+ntf,1,-1
c
c     *Calcul de l'etat adjoint p_kt au pas kt
c       Calcul du second membre p
c       Initialisation:
c       nt=nti+ntf
c       si kt=nt,p est nul
c       si kt<nt,on a au depart p=p_kt+1,etat adjoint au pas kt+1;
c       on prend p=p0,ou p0=(smy)t*I*p avec smy=Id+(dt/2).dfy(t,y,u)
c       avec dt=t_(kt+1)-t_kt,y=y_kt,t=t_kt,I designant la matrice
c       diagonale d'ordre ny dont les nea premiers coefficients
c       valent 0 et les autres 1 et Id designant la matrice
c       identite d'ordre ny;
c       si le systeme est affine avec partie lineaire autonome
c       (ilin=2) smy est calculee seulement aux premiers pas de temps
c       pour dti et dtf,sinon (ilin=0 ou 1) smy est calculee a chaque
c       pas de temps
c
c       stockage de l'etat adjoint et de dt/2 au pas kt+1
c
        call dcopy(ny,p,1,oldp,1)
        luv=min(nu,1+nuc+kt*nuv)
c
c       calcul de y=y_kt,dt=t_(kt+1)-t_kt,dt2=dt/2,
c                 dt2new=(t_kt-t_(kt-1))/2
c
        call dcopy(ny,ytot(1,kt),1,y,1)
c
        if (kt.lt.nti) then
          t=kt*dti+t0
          dt=dti
        else
          t=nti*dti+(kt-nti)*dtf+t0
          dt=dtf
        endif
        dt2=dt/2.0d+0
        if (kt.ne.nti) then
          dt2new=dt2
        else
          dt2new=dti/2.0d+0
        endif
c
c       Dans le cas ilin<=1,
c         calcul de fy=dfy(t,y,u) puis de smy=Id+(dt/2).dmy
c         lorsque kt<(nti+ntf)
c       Sinon (ilin>1),fy=dfy a ete calcule dans icse1
c
c
        if (ilin.le.1) then
          indf=2
          call icsef(indf,t,y,u,u(luv),f,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
          if (indf.le.0) then
            ind=indf
            return
          endif
        endif
c
        if (kt.ne.nti+ntf) then
          if (ilin.le.1.or.kt.eq.nti+ntf-1.or.kt.eq.nti-1) then
            do 30 i=1,ny
            do 30 j=1,ny
30          smy(i,j)=dt2*fy(i,j)
            do 35 i=1,ny
35          smy(i,i)=1.0d+0+smy(i,i)
          endif
c
c         calcul de p0=(smy)t*I*p puis p=p0
c
          if (nea.gt.0) then
            do 40 i=1,nea
40          p(i)=0.0d+0
          endif
          call dmmul(p,1,smy,ny,p0,1,1,ny,ny)
c
          call dcopy(ny,p0,1,p,1)
        endif
c
c       Fin du calcul du second membre p
c         si kt=itob(ktob),on ajoute c2y(.,ktob) au second membre p
c
        if (ktob.gt.0) then
          if (kt.eq.itob(ktob)) then
            do 50 i=1,ny
50          p(i)=p(i)+c2y(i,ktob)
            ktob=ktob-1
          endif
        endif
c
c       Calcul et factorisation de la matrice dmy du systeme
c       de l'etat adjoint
c       dmy=I-dt2new.dfy(t,y,u) avec dt2new=(t_kt-t_(kt-1))/2,
c       y=y_kt,t=t_kt,Idesignant la matrice diagonale d'ordre
c       ny dont les nea premiers coefficients valent 0 et les
c       autres 1;
c       si le systeme est affine avec partie lineaire autonome
c       (ilin=2) dmy est calculee et factorisee aux premiers
c       pas de temps pour dti et dtf,sinon (ilin=0 ou 1) dmy est
c       calculee et factorisee a chaque pas de temps
c
        if (ilin.le.1.or.kt.eq.nti+ntf.or.kt.eq.nti) then
          do 60 i=1,ny
          do 60 j=1,ny
60        dmy(i,j)=-dt2new*fy(i,j)
          do 65 i=1+nea,ny
65        dmy(i,i)=1.0d+0+dmy(i,i)
          call dgefa(dmy,ny,ny,ipv2,info)
        endif
c
c       Resolution de (dmy)t*X=p,la solution s'appelant p
c       p est alors p_kt,etat adjoint au pas kt
c
        call dgesl(dmy,ny,ny,ipv2,p,1)
c
c     *Calcul du gradient g au pas kt+1
c       calcul de la contribution gt au gradient au pas kt+1:
c       gt=(dt/2).(I*dfu(t_kt,y_kt,u)+dfu(t_kt+1,y_kt+1,u))t*p_kt+1
c       avec dt=t_(kt+1)-t_kt,I designant la matrice diagonale
c       d'ordre ny dont les nea premiers coefficients valent 0
c       et les autres 1
c
        if (nuv.gt.0.or.iu(3).eq.1) then
          indf=3
          call icsef(indf,t,y,u,u(luv),f,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
          if (indf.le.0) then
            ind=indf
            return
          endif
          if (kt.lt.nti+ntf) then
            call dmmul(oldp,1,oldmu,ny,gt,1,1,ny,nuc+nuv)
            call dscal(nuc+nuv,dt2,gt,1)
c           le gradient g est la somme des contributions
            if(iu(3).gt.0) then
              call dadd(nuc,gt,1,g,1)
            endif
            if(nuv.gt.0) then
              luv=min(nu,1+nuc+(kt+1)*nuv)
              call dadd(nuv,gt(nuc+1),1,g(luv),1)
            endif
            if (nea.gt.0) then
              do 70 i=1,nea
70            oldp(i)=0.0d+0
            endif
            call dmmul(oldp,1,fu,ny,gt,1,1,ny,nuc+nuv)
            call dscal(nuc+nuv,dt2,gt,1)
c           le gradient g est la somme des contributions
            if(iu(3).gt.0) then
              call dadd(nuc,gt,1,g,1)
            endif
            if(nuv.gt.0) then
              luv=min(nu,1+nuc+kt*nuv)
              call dadd(nuv,gt(nuc+1),1,g(luv),1)
            endif
          endif
c
c         stockage de dfu(t_kt,y_kt,u) dans oldmu
c
        call dcopy(ny*(nuc+nuv),fu,1,oldmu,1)
c
c     *On passe au pas de temps suivant:kt-1,sauf si kt=1,auquel cas
c       on calcule la contribution gt au gradient au pas kt=1 et
c       on l'ajoute a g;on a:
c       gt=(dt/2).(I*dfu(t0,y0,u)+dfu(t_1,y_1,u))t*p_1,avec
c       dt=t_1-t0=dti car nti n'est jamais nul par convention
c
          if (kt.eq.1) then
            t=t0
            dt2=dti/2.0d+0
            call dcopy(ny,y0,1,y,1)
            indf=3
            call icsef(indf,t,y,u,u(luv),f,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
            if (indf.le.0) then
              ind=indf
              return
            endif
            call dmmul(p,1,oldmu,ny,gt,1,1,ny,nuc+nuv)
            call dscal(nuc+nuv,dt2,gt,1)
c           le gradient g est la somme des contributions
            if(iu(3).gt.0) then
              call dadd(nuc,gt,1,g,1)
            endif
            if(nuv.gt.0) then
              luv=min(nu,1+nuc+nuv)
              call dadd(nuv,gt(nuc+1),1,g(luv),1)
            endif
            if (nea.gt.0) then
              do 90 i=1,nea
90            p(i)=0.0d+0
            endif
            call dmmul(p,1,fu,ny,gt,1,1,ny,nuc+nuv)
            call dscal(nuc+nuv,dt2,gt,1)
c           le gradient g est la somme des contributions
            if(iu(3).gt.0) then
              call dadd(nuc,gt,1,g,1)
            endif
            if(nuv.gt.0) then
              luv=min(nu,1+nuc)
              call dadd(nuv,gt(nuc+1),1,g(luv),1)
            endif
          endif
        endif
100   continue
c
c     gradient par rapport au controle initial
c
      if(max(iu(1),iu(2)) .gt.0) then
c
c     calcul de l'etat adjoint initial p0
c
        indf=2
        call icsef(indf,t,y,u,u(luv),f,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
        if (indf.eq.0) then
          ind=indf
          return
        endif
        do 120 i=1,ny
        do 120 j=1,ny
120     smy(i,j)=dt2*fy(i,j)
        do 125 i=1,ny
125     smy(i,i)=1.0d+0+smy(i,i)
        if (nea.gt.0) then
          do 130 i=1,nea
130       p(i)=0.0d+0
        endif
        call dmmul(p,1,smy,ny,p0,1,1,ny,ny)
c     incrementation du gradient
        indi=2
        call icsei(indi,nui,u(lui),y0,y0u,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
        if (indi.le.0) then
           ind=indi
           return
        endif
        call dmmul(p0,1,y0u,ny,gt,1,1,nui,nui)
        do 150 i=1,nui
150     g(lui+i-1)=g(lui+i-1)+gt(i)
c
      endif
      end
