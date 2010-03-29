      subroutine icsenb(ind,nu,u,co,g,itv,rtv,dtv)
c     Copyright INRIA
      external nbsec,nvcout,icsei
c     controle de la rentree de la navette en backward
      call icse(ind,nu,u,co,g,itv,rtv,dtv,nbsec,nvcout,icsei)
      end
      subroutine icsenf(ind,nu,u,co,g,itv,rtv,dtv)
      external nfsec,nvcout,icsei
c     controle de la rentree de la navette en forward
      call icse(ind,nu,u,co,g,itv,rtv,dtv,nfsec,nvcout,icsei)
      end

      subroutine nbsec(indf,t,y,uc,uv,f,fy,fu,b,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c      Second membre de l'equation d'etat :
c       Parametres d'entree :
c        indf     : vaut 1,2,3 suivant qu'on veut calculer f,fy,fu
c        t        : instant courant
c        y(ny)    : etat a un instant donne
c        uc(nuc)  : controle independant du temps
c        uv(nuv)  : controle dependant du temps, a l'instant t
c        b(ny)    : terme constant dans le cas lineaire quadratique
c       Parametres de sortie :
c         indf    : >0 si  le calcul s'est  correctement effectue,<=0
c                   sinon
c        f(ny)    : second membre
c        fy(ny,ny): jacobien de f par rapport a y
c        fu(ny,nuc+nuv) : derivee de f par rapport au controle
c       Tableaux de travail reserves a l'utilisateur :
c        itu(nitu): tableau entier
c        dtu(ndtu): tableau double precision
c       (nitu et ndtu sont initialises par le common icsez).
c!
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,*),
     &     b(ny),itu(*),dtu(*),iu(5)
      call navetb(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
      end

      subroutine nfsec(indf,t,y,uc,uv,f,fy,fu,b,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c      Second membre de l'equation d'etat :
c       Parametres d'entree :
c        indf     : vaut 1,2,3 suivant qu'on veut calculer f,fy,fu
c        t        : instant courant
c        y(ny)    : etat a un instant donne
c        uc(nuc)  : controle independant du temps
c        uv(nuv)  : controle dependant du temps, a l'instant t
c        b(ny)    : terme constant dans le cas lineaire quadratique
c       Parametres de sortie :
c         indf    : >0 si  le calcul s'est  correctement effectue,<=0
c                   sinon
c        f(ny)    : second membre
c        fy(ny,ny): jacobien de f par rapport a y
c        fu(ny,nuc+nuv) : derivee de f par rapport au controle
c       Tableaux de travail reserves a l'utilisateur :
c        itu(nitu): tableau entier
c        dtu(ndtu): tableau double precision
c       (nitu et ndtu sont initialises par le common icsez).
c!
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,*),
     &     b(ny),itu(*),dtu(*),iu(5)
      call navetf(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
      end

      subroutine nvcout(indc,nu,tob,obs,cof,ytob,ob,u,
     & c,cy,g,yob,d,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c
c     critere standard des moindres carres
c
c
c      Cout ponctuel :
c       Parametres d'entree :
c        indc     : 1 si on desire calculer c2,2 si on desire
c                   calculer c2y,c2u
c        tob      : instants de mesure
c        obs      : matrice d'observation
c        cof      : coefficients de ponderation du cout
c        ytob     : valeur de l'etat aux instants d'observation
c        ob       : mesures
c        u(nu)    : controle.Le controle variable est stocke a la
c                   suite du controle suite du constant.
c       Parametres de sortie :
c        indc     : comme pour icsec1
c        c2       : cout
c        c2y(ny,ntob) : derivee de c2 par rapport a y
c        g(nu)  : derivee de c2 par rapport a u
c!
      implicit double precision (a-h,o-z)
      dimension tob(ntob),obs(nob,ny),cof(nob,ntob),ytob(ny,ntob),
     &ob(nex,ntob,nob),u(nu),cy(ny,ntob),g(nu),yob(nob,ntob),
     &d(nob),itu(nitu),dtu(ndtu),iu(5)
c
      call navetc(indc,nu,tob,obs,cof,ytob,ob,u,
     &     c,cy,g,yob,d,itu,dtu)
c
      end
      subroutine navetb(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
C        test de icse : navette
C        fb,inria
C!
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,nuc+nuv),itu(nit
     &u),dtu(ndtu)
      f(1)=-uc(1)*dtu(13)*(-dtu(8)*sin(y(2)*dtu(22))/(y(3)*dtu(23)+dtu(9
     &))**2-0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu(2))*dtu
     &(10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(21)
      f(2)=-uc(1)*dtu(13)*(y(1)*dtu(21)*cos(y(2)*dtu(22))/(y(3)*dtu(23)+
     &dtu(9))-dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21)*(y(3)*dtu(23)+dtu(9
     &))**2)+0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*dtu(21)*exp(-
     &y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(22)
      f(3)=-uc(1)*y(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
      f(4)=-uc(1)*y(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+
     &dtu(9))*dtu(24))
      if(indf.eq.2) then
        fy(1,1)=dtu(1)*uc(1)*y(1)*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu(2))*
     &  dtu(10)*dtu(13)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/dtu(7)
        fy(1,2)=uc(1)*dtu(8)*dtu(13)*dtu(22)*cos(y(2)*dtu(22))/(dtu(21)*
     &  (y(3)*dtu(23)+dtu(9))**2)
        fy(1,3)=-uc(1)*dtu(13)*dtu(23)*(2*dtu(8)*sin(y(2)*dtu(22))/(y(3)
     &  *dtu(23)+dtu(9))**3+0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dt
     &  u(3)+dtu(2))*dtu(10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/(dtu(
     &  7)*dtu(11)))/dtu(21)
        fy(1,4)=0
        fy(2,1)=-uc(1)*dtu(13)*dtu(21)*(cos(y(2)*dtu(22))/(y(3)*dtu(23)+
     &  dtu(9))+dtu(8)*cos(y(2)*dtu(22))/(y(1)**2*dtu(21)**2*(y(3)*dtu(2
     &  3)+dtu(9))**2)+0.5*dtu(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*exp(-y(3
     &  )*dtu(23)/dtu(11))/dtu(7))/dtu(22)
        fy(2,2)=-uc(1)*dtu(13)*(dtu(8)*sin(y(2)*dtu(22))/(y(1)*dtu(21)*(
     &  y(3)*dtu(23)+dtu(9))**2)-y(1)*dtu(21)*sin(y(2)*dtu(22))/(y(3)*dt
     &  u(23)+dtu(9)))
        fy(2,3)=-uc(1)*dtu(13)*dtu(23)*(-y(1)*dtu(21)*cos(y(2)*dtu(22))/
     &  (y(3)*dtu(23)+dtu(9))**2+2*dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21
     &  )*(y(3)*dtu(23)+dtu(9))**3)-0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5)
     &  )*dtu(10)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/(dtu(7)*dtu(11)))/d
     &  tu(22)
        fy(2,4)=0
        fy(3,1)=-uc(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
        fy(3,2)=-uc(1)*y(1)*dtu(13)*dtu(21)*dtu(22)*cos(y(2)*dtu(22))/dt
     &  u(23)
        fy(3,3)=0
        fy(3,4)=0
        fy(4,1)=-uc(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+
     &  dtu(9))*dtu(24))
        fy(4,2)=uc(1)*y(1)*dtu(13)*dtu(21)*dtu(22)*sin(y(2)*dtu(22))/((y
     &  (3)*dtu(23)+dtu(9))*dtu(24))
        fy(4,3)=uc(1)*y(1)*dtu(13)*dtu(21)*dtu(23)*cos(y(2)*dtu(22))/((y
     &  (3)*dtu(23)+dtu(9))**2*dtu(24))
        fy(4,4)=0
      end if
      if(indf.eq.3) then
        fu(1,1)=-dtu(13)*(-dtu(8)*sin(y(2)*dtu(22))/(y(3)*dtu(23)+dtu(9)
     &  )**2-0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu(2))*dt
     &  u(10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(21)
        fu(1,2)=0.5*dtu(1)*uc(1)*y(1)**2*(2*uv(1)*dtu(4)+dtu(3))*dtu(10)
     &  *dtu(13)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/dtu(7)
        fu(2,1)=-dtu(13)*(y(1)*dtu(21)*cos(y(2)*dtu(22))/(y(3)*dtu(23)+d
     &  tu(9))-dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21)*(y(3)*dtu(23)+dtu(
     &  9))**2)+0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*dtu(21)*ex
     &  p(-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(22)
        fu(2,2)=-0.5*dtu(1)*uc(1)*y(1)*dtu(6)*dtu(10)*dtu(13)*dtu(21)*ex
     &  p(-y(3)*dtu(23)/dtu(11))/(dtu(7)*dtu(22))
        fu(3,1)=-y(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
        fu(3,2)=0
        fu(4,1)=-y(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+d
     &  tu(9))*dtu(24))
        fu(4,2)=0
      end if
      end


      subroutine navetf(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
C        test de icse : navette
C        fb,inria
C        [" "," "]
C
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,nuc+nuv),itu(nit
     &u),dtu(ndtu)
      f(1)=uc(1)*dtu(13)*(-dtu(8)*sin(y(2)*dtu(22))/(y(3)*dtu(23)+dtu(9)
     &)**2-0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu(2))*dtu(
     &10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(21)
      f(2)=uc(1)*dtu(13)*(y(1)*dtu(21)*cos(y(2)*dtu(22))/(y(3)*dtu(23)+d
     &tu(9))-dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21)*(y(3)*dtu(23)+dtu(9)
     &)**2)+0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*dtu(21)*exp(-y
     &(3)*dtu(23)/dtu(11))/dtu(7))/dtu(22)
      f(3)=uc(1)*y(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
      f(4)=uc(1)*y(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+d
     &tu(9))*dtu(24))
      if(indf.eq.2) then
        fy(1,1)=-1.0*dtu(1)*uc(1)*y(1)*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu
     &  (2))*dtu(10)*dtu(13)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/dtu(7)
        fy(1,2)=-uc(1)*dtu(8)*dtu(13)*dtu(22)*cos(y(2)*dtu(22))/(dtu(21)
     &  *(y(3)*dtu(23)+dtu(9))**2)
        fy(1,3)=uc(1)*dtu(13)*dtu(23)*(2*dtu(8)*sin(y(2)*dtu(22))/(y(3)*
     &  dtu(23)+dtu(9))**3+0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dtu
     &  (3)+dtu(2))*dtu(10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/(dtu(7
     &  )*dtu(11)))/dtu(21)
        fy(1,4)=0
        fy(2,1)=uc(1)*dtu(13)*dtu(21)*(cos(y(2)*dtu(22))/(y(3)*dtu(23)+d
     &  tu(9))+dtu(8)*cos(y(2)*dtu(22))/(y(1)**2*dtu(21)**2*(y(3)*dtu(23
     &  )+dtu(9))**2)+0.5*dtu(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*exp(-y(3)
     &  *dtu(23)/dtu(11))/dtu(7))/dtu(22)
        fy(2,2)=uc(1)*dtu(13)*(dtu(8)*sin(y(2)*dtu(22))/(y(1)*dtu(21)*(y
     &  (3)*dtu(23)+dtu(9))**2)-y(1)*dtu(21)*sin(y(2)*dtu(22))/(y(3)*dtu
     &  (23)+dtu(9)))
        fy(2,3)=uc(1)*dtu(13)*dtu(23)*(-y(1)*dtu(21)*cos(y(2)*dtu(22))/(
     &  y(3)*dtu(23)+dtu(9))**2+2*dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21)
     &  *(y(3)*dtu(23)+dtu(9))**3)-0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5))
     &  *dtu(10)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/(dtu(7)*dtu(11)))/dt
     &  u(22)
        fy(2,4)=0
        fy(3,1)=uc(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
        fy(3,2)=uc(1)*y(1)*dtu(13)*dtu(21)*dtu(22)*cos(y(2)*dtu(22))/dtu
     &  (23)
        fy(3,3)=0
        fy(3,4)=0
        fy(4,1)=uc(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+d
     &  tu(9))*dtu(24))
        fy(4,2)=-uc(1)*y(1)*dtu(13)*dtu(21)*dtu(22)*sin(y(2)*dtu(22))/((
     &  y(3)*dtu(23)+dtu(9))*dtu(24))
        fy(4,3)=-uc(1)*y(1)*dtu(13)*dtu(21)*dtu(23)*cos(y(2)*dtu(22))/((
     &  y(3)*dtu(23)+dtu(9))**2*dtu(24))
        fy(4,4)=0
      end if
      if(indf.eq.3) then
        fu(1,1)=dtu(13)*(-dtu(8)*sin(y(2)*dtu(22))/(y(3)*dtu(23)+dtu(9))
     &  **2-0.5*dtu(1)*y(1)**2*(uv(1)**2*dtu(4)+uv(1)*dtu(3)+dtu(2))*dtu
     &  (10)*dtu(21)**2*exp(-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(21)
        fu(1,2)=-0.5*dtu(1)*uc(1)*y(1)**2*(2*uv(1)*dtu(4)+dtu(3))*dtu(10
     &  )*dtu(13)*dtu(21)*exp(-y(3)*dtu(23)/dtu(11))/dtu(7)
        fu(2,1)=dtu(13)*(y(1)*dtu(21)*cos(y(2)*dtu(22))/(y(3)*dtu(23)+dt
     &  u(9))-dtu(8)*cos(y(2)*dtu(22))/(y(1)*dtu(21)*(y(3)*dtu(23)+dtu(9
     &  ))**2)+0.5*dtu(1)*y(1)*(uv(1)*dtu(6)+dtu(5))*dtu(10)*dtu(21)*exp
     &  (-y(3)*dtu(23)/dtu(11))/dtu(7))/dtu(22)
        fu(2,2)=0.5*dtu(1)*uc(1)*y(1)*dtu(6)*dtu(10)*dtu(13)*dtu(21)*exp
     &  (-y(3)*dtu(23)/dtu(11))/(dtu(7)*dtu(22))
        fu(3,1)=y(1)*dtu(13)*dtu(21)*sin(y(2)*dtu(22))/dtu(23)
        fu(3,2)=0
        fu(4,1)=y(1)*dtu(13)*dtu(21)*cos(y(2)*dtu(22))/((y(3)*dtu(23)+dt
     &  u(9))*dtu(24))
        fu(4,2)=0
      end if
      end

      subroutine navetc(indc,nu,tob,obs,cof,ytob,ob,u,
     & c,cy,g,yob,d,itu,dtu)
c
c     test probleme navette
c
c     sous programme appele par icse.f qui donne :
c     pour indc=1,le cout:c
c     pour indc=2,la matrice derivee du cout par rapport a l'etat
c                 calcule aux instants de mesure:cy(ny,ntob)
c     sorties :
c
c     c        double precision
c              cout
c     cy       double precision (ny,ntob)
c              derivee du cout par rapport a l'etat calcule aux
c              instants de mesure
c     g        double precision (nu)
c              le gradient est initialise a la derivee partielle du
c              cout par rapport au controle
c
c     variables internes:     yob,d
c
      implicit double precision (a-h,o-z)
      dimension ytob(4),ob(3),u(nu),cy(4),g(nu),dtu(*)
c
      cpen=dtu(25)
      cy(1)=ytob(1)-ob(1)
      cy(2)=ytob(2)-ob(2)
      cy(3)=ytob(3)-ob(3)
      cy(4)=1.0d+0/cpen
      c=ytob(4)/cpen + ( cy(1)**2 + cy(2)**2 + cy(3)**2 )/2.0d+0
      do 10 k=1,nu
10    g(k)=0.0d+0
      end
