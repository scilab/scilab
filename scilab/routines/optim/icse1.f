C/MEMBR ADD NAME=ICSE1,SSI=0
c     Copyright INRIA
      subroutine icse1(ind,nu,u,icsef,y0,ytot,f,b,fy,fu,ipv1,
     &dm,yold,smold,yint,yerr,dif1,dif2,dif3,itu,dtu,
     &t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c     sous programme de icse.fortran:calcul de l'etat
c
      implicit double precision (a-h,o-z)
      dimension u(nu),y0(ny),ytot(ny,nti+ntf),f(ny),b(ny),fy(ny,ny),
     &fu(ny,nuc+nuv),ipv1(ny),dm(ny,ny),yold(ny),smold(ny),yint(ny),
     &yerr(ny),dif1(ny),dif2(ny),dif3(ny),itu(nitu),dtu(ndtu),iu(5)
      external icsef
c
c     Initialisation
c
      t=t0
      call dcopy(ny,y0,1,yold,1)
c
c     Resolutions successives du systeme d'etat discretise a chaque
c     pas de temps
c
      do 100 kt=1,nti+ntf
c
c     *Calcul,puis factorisation de la matrice dm du systeme:
c     on a au depart yold=y_kt-1,etat au pas kt-1;alors
c     dm=(1/dt).I-(1/2).dfy(told,yold,u) avec dt=t_kt-t_(kt-1),
c     told=t_(kt-1),I designant la matrice diagonale d'ordre ny
c     dont les nea premiers coefficients diagonaux valent 0 et les
c     autres 1
c     si le systeme est affine avec partie lineaire autonome (ilin=2)
c     dm est calculee et factorisee seulement aux premiers pas de
c     temps pour dti et dtf,sinon (ilin=0 ou 1) dm est calculee et
c     factorisee a chaque pas de temps
c
c     stockage de l'instant au pas kt-1
c
      luv=min(nu,nuc+1+(kt-1)*nuv)
      told=t
c
c     calcul de t=t_kt,dt=t_kt-t_(kt-1),dtinv=1/dt
c
      if (kt.le.nti) then
        t=kt*dti+t0
        dt=dti
      else
        t=nti*dti+(kt-nti)*dtf+t0
        dt=dtf
      endif
      dtinv=1.0d+0/dt
c
c     calcul et factorisation de dm=dtinv.I-(1/2).dfy(told,yold,u)
c     I designant la matrice diagonale d'ordre ny dont les nea
c     premiers coefficients diagonaux valent 0 et les autres 1
c
c     fy=dfy(told,yold,u) n'est calcule que pour kt=1 lorsque
c     ilin>1
c     dm=dtinv.I-(1/2).fy n'est calcule que pour kt=1 ou kt=nti+1
c     lorsque ilin>1
c
      if (kt.eq.1.or.kt.eq.nti+1.or.ilin.le.1) then
        indf=2
        if (kt.eq.1.or.ilin.le.1)
     &    call icsef(indf,told,yold,u,u(luv),f,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu)
        if (indf.le.0) then
          ind=indf
          return
        endif
        do 10 i=1,ny
        do 10 j=1,ny
10      dm(i,j)=-fy(i,j)/2.0d+0
        do 20 i=1+nea,ny
20      dm(i,i)=dm(i,i)+dtinv
        call dgefa(dm,ny,ny,ipv1,info)
      endif
c
c     *Calcul de l'etat y_kt au pas kt:
c       Initialisation du nombre d'iterations dans l'algorithme
c
        it=1
c
c       Prediction du deplacement:dif1 (Euler explicite)
c       dif1=dt.(I*f(told,yold,u))
c       et stockage de I*f(told,yold,u) dans smold,I designant la
c       matrice diagonale d'ordre ny dont les nea premiers
c       coefficients diagonaux valent 0 et les autres 1
c
c       si kt=1,smold=f(told,yold,u)
c       sinon,smold=f(told,yold,u) a ete calcule au pas kt-1
c       sous le nom dif1=f(t,yerr,u)
c
        if (kt.eq.1) then
          indf=1
          call icsef(indf,told,yold,u,u(luv),smold,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu)
          if (indf.le.0) then
            ind=indf
            return
          endif
        endif
c
c       smold=I*smold,puis dif1=dt.smold
c
        if (nea.gt.0) then
          do 30 i=1,nea
30        smold(i)=0.0d+0
        endif
        call dcopy(ny,smold,1,dif1,1)
        call dscal(ny,dt,dif1,1)
c
c       Calcul de l'erreur:dif2=(1/2).(smold+f(t,yold+dif1,u))-dif1/dt
c         dif2=f(t,yint,u) ou yint=yold+dif1
c
          luv=min(nu,nuc+1+kt*nuv)
          call dcopy(ny,yold,1,yint,1)
          call dadd(ny,dif1,1,yint,1)
          indf=1
          call icsef(indf,t,yint,u,u(luv),dif2,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu)
          if (indf.le.0) then
            ind=indf
            return
          endif
c
c         dif2=(1/2).(smold+dif2)
c
          call dadd(ny,smold,1,dif2,1)
          call dscal(ny,0.50d+0,dif2,1)
c
c         dif2=dif2-dif1/dt
c
          call daxpy(ny,-dtinv,dif1,1,dif2,1)
c
c       Calcul du nouvel ecart:dif3
c         initialisation:dif3=dif1
c
          call dcopy(ny,dif1,1,dif3,1)
c
c         resolution de dm*X=dif2,la solution X s'appelant dif2
c
50        call dgesl(dm,ny,ny,ipv1,dif2,0)
c
c         dif3=dif3+dif2 est le nouvel ecart
c
          call dadd(ny,dif2,1,dif3,1)
c
c       Calcul de la norme err de l'erreur dif2 apres correction
c       dif2=(1/2).(smold+f(t,yold+dif3,u))-I*(dif3/dt)
c       I designant la matrice diagonale d'ordre ny dont les nea
c       premiers coefficients diagonaux valent 0 et les autres 1
c
c         dif1=f(t,yerr,u) ou yerr=yold+dif3
c
          call dcopy(ny,yold,1,yerr,1)
          call dadd(ny,dif3,1,yerr,1)
c
c         ermx<0 correspond au fait que l'utilisateur a choisi
c         de ne faire qu'une correction sans aucun test sur
c         l'erreur
          if (ermx.lt.0) go to 55
c
          indf=1
          call icsef(indf,t,yerr,u,u(luv),dif1,fy,fu,b,itu,dtu,
     &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu)
          if (indf.le.0) then
            ind=indf
            return
          endif
c
c         dif2=dif1
c
          call dcopy(ny,dif1,1,dif2,1)
c
c         dif2=(1/2).(smold+dif2)
c
          call dadd(ny,smold,1,dif2,1)
          call dscal(ny,.5d0,dif2,1)
c
c         dif2=dif2-I*(dif3/dt)
c
          call daxpy(ny-nea,-dtinv,dif3(1+nea),1,dif2(1+nea),1)
c
c         err=norme l2 de dif2
c
          err=dnrm2(ny,dif2,1)
c
c       Si err>ermx,on corrige a nouveau si possible,c'est a dire
c       si it<=itmx
c
        if (err.gt.ermx.and.ilin.eq.0) then
          it=it+1
          if (it.gt.itmx) then
            ind=-1
            print *,' icse : integration de l etat impossible'
            return
          endif
          go to 50
        endif
c
c       Si err<=ermx,yold=yerr est y_kt,etat au pas kt
c       (on avait calcule yerr=yold+dif3)
c       et ytot(.,kt)=yold
c
55      call dcopy(ny,yerr,1,yold,1)
        call dcopy(ny,yold,1,ytot(1,kt),1)
c
c       smold=dif1
c       (on avait calcule dif1=f(t,yerr,u) qui deviendra
c       f(told,yold,u) au pas kt+1)
c
        call dcopy(ny,dif1,1,smold,1)
c
c     *On passe au pas de temps suivant:kt+1
c
100   continue
      return
      end
