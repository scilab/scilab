      subroutine icsest(ind,nu,u,co,g,itv,rtv,dtv)
c     Copyright INRIA
      external stsec,icsec2,icsei
c
c     identification de parametres d'un systeme lineaire
c     serotonine G.Launay
c
      call icse(ind,nu,u,co,g,itv,rtv,dtv,stsec,icsec2,icsei)
      end

      subroutine stsec(indf,t,y,uc,uv,f,fy,fu,b,itu,dtu,
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
c
      call seros(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
      end

      subroutine seros(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
c     sous programme appele par icse.fortran qui donne :
c     pour indf=1,les seconds membres du systeme:f(ny)
c     pour indf=2,la matrice derivee des seconds membres par
c                 rapport a l'etat:fy(ny,ny)
c     pour indf=3,la matrice derivee des seconds membres par
c                 rapport aux parametres:fu(ny,nu)
c     on prend ici un modele lineaire ,en supposant que toute la 5HT
c     excretee est degradee avant d'arriver dans le milieu exterieur
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,*),
     &itu(*),dtu(*)
c     seconds membres:
      if (indf.eq.1) then
        f(1)=-(uc(1)+uc(2))*y(1)
        f(2)=uc(1)*y(1)-(uc(5)+uc(3))*y(2)
        f(3)=uc(2)*y(1)+uc(3)*y(2)-(uc(4)+uc(6))*y(3)
        f(4)=uc(4)*y(3)-uc(7)*y(4)
        return
      endif
c     derivee des seconds membres par rapport a l'etat:
c     pour 1<=i<=ny et 1<=j<=ny,fy(i,j)=d(f(i))/dyj
      if (indf.eq.2) then
        do 10 i=1,ny
        do 10 j=1,ny
10      fy(i,j)=0.0d+0
        fy(1,1)=-uc(1)-uc(2)
        fy(2,1)=uc(1)
        fy(3,1)=uc(2)
        fy(2,2)=-uc(3)-uc(5)
        fy(3,2)=uc(3)
        fy(3,3)=-uc(4)-uc(6)
        fy(4,3)=uc(4)
        fy(4,4)=-uc(7)
        return
      endif
c     derivee des seconds membres par rapport aux parametres:
c     pour 1<=i<=ny et 1<=j<=nu,fu(i,j)=d(f(i))/duj
      if (indf.eq.3) then
        do 20 i=1,ny
        do 20 j=1,nuc+nuv
20      fu(i,j)=0.0d+0
        fu(1,1)=-y(1)
        fu(2,1)=y(1)
        fu(1,2)=-y(1)
        fu(3,2)=y(1)
        fu(2,3)=-y(2)
        fu(3,3)=y(2)
        fu(3,4)=-y(3)
        fu(4,4)=y(3)
        fu(2,5)=-y(2)
        fu(3,6)=-y(3)
        fu(4,7)=-y(4)
        return
      endif
      end

