      subroutine icse(ind,nu,u,co,g,itv,rtv,dtv,icsef,icsec2,icsei)
c!but
c     Le logiciel ICSE est un outil de resolution de problemes de
c     CONTROLE OPTIMAL de systemes decrits par des equations
c     differentielles ou algebrico-differentielles, NON LINEAIRES.
c     Dans la mesure ou dans la methode d'integration qu'il utilise
c     est inconditionnellement stable,il peut aussi etre utilise pour
c     resoudre des problemes de controle d'EQUATIONS AUX DERIVEES
c     PARTIELLES DYNAMIQUES (reaction-diffusion, par exemple), ou
c     plus generalement pour controler des systemes raides.
c     Le controle se decompose en une partie independante du temps et
c     une partie dependante du temps.Cette structure permet a
c     l'utilisateur de resoudre facilement les problemes
c     d'IDENTIFICATION DE PARAMETRES d'un systeme dynamique par des
c     methodes de MOINDRES CARRES. Diverses facilites sont d'ailleurs
c     prevues pour ce cas.
c     Dans le cas d'un SYSTEME LINEAIRE,l'integration de l'etat et de
c     l'etat adjoint sont effectuees de maniere a tirer parti de la
c     linearite de l'equation.
c
c     Resolution de problemes de controle ou d'identification de
c      parametres de systemes dynamiques du type:
c          0=fi(t,y,u),i<=nea et dyj/dt=fj(t,y,u),nea<j<=ny pour t>=t0
c          et y(t0)=y0 , en notant ny la dimension de l'etat y et
c          fk la keme composante de la fonction f
c      On effectue un certain nombre (nex) d'experiences identiques
c      au cours desquelles certaines donnees sont mesurees.
c      Le critere a minimiser est de la forme c(tob,ytob,ob) avec:
c        tob(ntob)       :instants de mesure
c        ytob(ny,ntob)   :valeurs de l'etat aux instants de mesure
c        ob(nex,ntob,nob):mesures
c      L'equation d'etat est discretisee par la methode
c      de Crank-Nicolson.
c      Le gradient du cout est calcule en utilisant l'etat adjoint du
c      systeme discretise.
c
c!origine
c      F.Bonnans,G.Launay, INRIA, 1987
c
c! DESCRIPTION FORMELLE DES PROBLEMES DE CONTROLE CONSIDERES
c       L'equation du systeme est de la forme
c              0     =  fi(t,y(t),uc,uv(t)) ,  i<=nea,
c          dyi(t)/dt =  fi(t,y(t),uc,uv(t)) ,  i> nea,
c       et
c          t0<= t <=tf ,    y(t0)=y0 .
c     avec :
c       y(t )  : etat du systeme,
c       uc     : partie  du controle independante du temps
c               (controle constant),
c       uv     : partie  du controle dependante du temps
c                (controle variable)
c       t0, tf : instant initial et instant final,
c       y0     : etat initial. Il est soit fixe,soit fonction
c                du controle [y0=ei(uc,uv)]
c
c       Le critere a minimiser est de la forme  :
c
c                  tp
c                   ____
c                  \
c                   |    c2(ti,y(ti),uc) .
c                  /___
c                  ti =t1
c
c        Sont resolus, soit des problemes sans contraintes,  soit des
c     problemes comportant des contraintes de borne sur le controle.On
c     peut aussi utiliser ICSE pour resoudre des problemes comportant
c     des contraintes sur  l'etat, en traitant  ces contraintes par
c     penalisation ou lagrangien augmente [Ber,82].
c        Les fonctions  f,c1,c2,ei et leurs derivees  partielles sont
c     fournies par l'utilisateur sous forme de subroutines Fortran.
c
c
c
c! OUTILS POUR L'IDENTIFICATION DE PARAMETRES
c     Le probleme de l'identification de parametres (ou d'ajustement
c     de parametres a des mesures) a les caracteristiques suivantes :
c     le critere est fonction seulement de mesures faites a certains
c     instants et des valeurs de l'etat a ces instants.Seule la seconde
c     partie du cout intervient donc.Les mesures peuvent avoir ete
c     obtenues lors de plusieurs experiences.En general,le critere
c     est du type MOINDRES CARRES associe a une OBSERVATION LINEAIRE
c     .Autrement dit, il est de la forme
c
c                nex      ntob
c                ____     ____                              2
c           1    \        \      ||                       ||
c           -     |        |     || obs*y(ti) - z(iex,ti) || ,
c           2    /___     /___   ||                       ||
c                iex=1    ti =t1
c
c     ou obs est  la  matrice  d'observation et  z(iex,ti) represente
c     l'ensemble  des  mesures  faites  lors  de l'experience iex,  a
c     l'instant ti.Dans ce cas,l'utilisateur n'a aucune modification a
c     a apporter  a la subroutine de calcul  de cout  de l'exemple de
c     demonstration.
c!liste d'appel:
c      icse(ind,nu,u,co,g,itv,rtv,dtv)
c      en entree:
c
c      ind        entier egal a 2,3,ou4
c
c      nu         entier
c                 dimension du vecteur des parametres
c
c      u          double precision (nu)
c                 vecteur des parametres
c
c      en sortie:
c
c      co         double precision
c                 cout
c
c      g          double precision (nu)
c                 gradient de la fonction de cout c
c
c      itv        entier (nitv)
c                 tableau de travail entier
c
c      rtv        reel (nrtv)
c                 tableau de travail reel
c
c      dtv        double precision (ndtv)
c                 tableau de travail double precision
c
c!subroutines utilisees
c      Linpack    :dadd,daxpy,dcopy,dmmul,dnrm2,dscal,dset,
c                  dgefa,dgesl
c                 :icse0,icse1,icse2,icscof,icsef,icsei
c      common/nird/nitv,nrtv,ndtv
c!utilisation
c
c     Le probleme a traiter doit etre defini par 3 routines
c     fortran ecrites par l'utilisateur :
c
c      -Second membre de l'equation d'etat :
c       icsef(indf,t,y,uc,uv,f,fy,fu,b,itu,dtu,
c     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
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
c
c      -Cout ponctuel :
c       icsec2(indc,nu,tob,obs,cof,ytob,ob,u,c2,c2y,g,yob,d,itu,dtu,
c     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
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
c
c      -Etat initial (s'il est variable)
c       icsei(indi,nui,ui,y0,y0ui,itu,dtu,
c     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c       Parametres d'entree :
c        indi     : 1 si on desire calculer y0, 2 si on  desire
c                     calculer y0ui
c        nui      : dimension du tableau ui defini ci-dessous,
c        ui       : partie du controle intervenant dans l'etat initial,
c                   determinee par iu;vaut uc,uv,ou [uc,uv].
c
c       Parametres de sortie :
c        indc     : >0  si le calcul  s'est correctement effectue,<=0
c                   sinon,
c        y0       : etat initial,
c        y0ui     : derivee de l'etat initial par rapport au controle.
c
c
c
c!vue d'ensemble
c         Pour  utiliser la  subroutine icse, il faut  disposer d'un
c     optimiseur (code d'implementation d'un algorithme d'optimisation)
c     a  la norme  MODULOPT.Il  faut  ensuite  ecrire  le  programme
c     principal, constitue de quatre parties :
c       1. Initialisation des variables du common icsez,
c       2. Initialisation des tableaux itv et dtv et du common nird,
c       3. Appel de l'optimiseur,
c       4. Traitement des resultats.
c
c
c     1. INITIALISATION DU COMMON ICSEZ
c        common/icsez/t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c        itmx,nex,nob,ntob,ntobi,nitu,ndtu
c
c       Liste des variables a initialiser :
c       t0    : instant initial
c       tf    : instant final
c       dti   : premier pas de temps
c       dtf   : second pas de temps
c       ermx  : test d'arret absolu sur la valeur du  second membre
c               dans la resolution de l'equation d'etat
c       iu(5) : tableau  parametrant le  probleme : seuls iu(1:3)
c               sont utilises.
c            iu(1)=1 si l'etat initial depend du  controle constant
c                  0 sinon
c            iu(2)=1 si l'etat initial  depend du controle variable
c                  0 sinon
c            iu(3)=1 si le second membre depend du controle constant,
c                  0 sinon
c
c       nuc   : dimension du controle constant.
c       nuv   : dimension du controle variable a un instant donne.
c       ilin  : indicateur de linearite
c       nti   : nombre de pas de temps correspondant a dti (premier
c                 pas de temps)
c       ntf   : nombre de pas  de temps correspondant a dtf (second
c               pas de temps)
c       ny    : dimension de l'etat a un instant donne
c       nea   : nombre d'equations algebriques (eventuellement nul)
c       itmx  : nombre  maximal  d'iterations dans la resolution
c               de l'equation d'etat discrete a un pas de temps
c               donne
c       nex   : nombre d'experiences effectuees
c       nob   : dimension du  vecteur des mesures  pour une
c               experience donnee en un instant donne
c       ntob  : nombre d'instants de mesure pour une experience donnee
c       ntobi : nombre d'instants de mesure correspondant a dti
c               (premier pas de temps)
c       nitu  : longueur de  itu,tableau de  travail entier reserve
c               a l'utilisateur
c       ndtu  : longueur de dtu,  tableau de travail  double
c               precision reserve a l'utilisateur
c       u(nu)       : parametres initiaux
c       y0(ny)      : etat initial
c       tob(ntob)   : instants de mesure
c       binf(nu)    : borne inferieures sur les parametres
c       bsup(nu)    : borne superieures sur les parametres
c       obs(nob,ny) : matrice d'observation
c
c     Bien noter que
c         nu = nuc + nuv*(nti+ntf+1),
c         nui= iu(1)*nuc+ui(2)*nuv*(nti+ntf+1)
c     et que les dimensions suivantes peuvent etre nulles :
c         nuc,nuv,ntf,nea.
c
c
c     2 INITIALISATION DES TABLEAUX ENTIER ET DOUBLE PRECISION.
c       Le tableau itv (entier) contient le tableau :
c       itu    dimension nitu      : reserve a l'utilisateur,
c     le reste du tableau etant reserve au systeme ICSE.
c
c     Le tableau dtv (reel double precision) contient les tableaux :
c       dtu    dimension  ndtu     : reserve a l'utilisateur,
c       y0                ny       : etat initial,
c       tob               ntob     : instants d'observation,
c       obs               nob,ny   : matrice d'observation,
c       ob            nex,ntob,nob : observations (mesures),
c       ech               nu       : coefficients de mise a l'echelle de
c                                    u,
c       cof              nob,ntob  : coefficients de ponderation du
c                                    cout,
c
c       Les dimensions nitu et ndtu sont passees par le common icsez,
c                      nitv et ndtv sont passees par le common nird.
c
c
c!
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      real rtv
      dimension u(nu),g(nu),itv(*),rtv(*),dtv(*),iu(5)
      external icsef,icsec2,icsei
c
      common/icsez/ t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &itmx,nex,nob,ntob,ntobi,nitu,ndtu
      common/nird/nitv,nrtv,ndtv
c
c
c     lui et nui servent quand l'etat initial depend du controle
      if (iu(2).gt.0) lui=min(nu,1+nuc)
      if (iu(1).gt.0) lui=1
      nui=iu(1)*nuc+iu(2)*nuv*(nti+ntf+1)
c
c     decoupage de itv
c     nitu longueur de itu tableau de travail entier reserve
c     a l'utilisateur
c     nitvt longueur de itvt tableau de travail entier de
c     icse1 et icse2
c
      litu=1
      litvt=litu+nitu
c
c     decoupage de dtv
c     ndtu longueur de dtu tableau de travail double precision
c     reserve a l'utilisateur
c     ndtvt longueur de dtvt tableau de travail double precision
c     de icse1 et icse2
c
      ldtu=1
      ly0=ldtu+ndtu
      ltob=ly0+ny
      lobs=ltob+ntob
      lob=lobs+nob*ny
      lech=lob+nex*ntob*nob
      lcof=lech+nu
c       ********************** Modif 88
      lb=lcof+nob*ntob
      lfy=lb+ny
      lfu=lfy+ny*ny
      ludep=lfu+ny*(nuc+nuv)
      lytot=ludep+nu
      lf=lytot+ny*(nti+ntf)
      ldtvt=lf+ny
c
c     decoupage de itvt pour icse1
c
      lipv1=litvt
      mitv1=lipv1+ny-1
c
c     decoupage de itvt pour icse2
c
      litob=litvt
      lipv2=litob+ntob
      mitv2=lipv2+ny-1
c
      mitv=max(mitv1,mitv2)
c
c     decoupage de dtvt pour icse1
c
      ldm=ldtvt
      lyold=ldm+ny*ny
      lsmold=lyold+ny
      lyint=lsmold+ny
      lyerr=lyint+ny
      ldif1=lyerr+ny
      ldif2=ldif1+ny
      ldif3=ldif2+ny
      mdtv1=ldif3+ny-1
c
c     decoupage de dtvt pour icse2
c
      lytob=ldtvt
      lc2y=lytob+ny*ntob
      ly0u=lc2y+ny*ntob
      ldmy=ly0u+ny*nu
      lsmy=ldmy+ny*ny
      loldmu=lsmy+ny*ny
      ly=loldmu+ny*(nuc+nuv)
      loldp=ly+ny
      lp=loldp+ny
      lp0=lp+ny
      lgt=lp0+ny
      lyob=lgt+max(nuc+nuv,nui)
      ld=lyob+nob*ntob
      mdtv2=ld+nob-1
c
      mdtv=max(mdtv1,mdtv2)
      if (mitv.gt.nitv.or.mdtv.gt.ndtv) then
        if (nitv+ndtv.gt.0) write(*,8003) mitv,mdtv
        nitv=mitv
        ndtv=mdtv
        return
      endif
      do 10 i=1,nu
      dtv(ludep+i-1)=u(i)
      u(i)=dtv(lech+i-1)*u(i)
10    continue
c
c     etat initial dependant du controle
c
      if (iu(1).gt.0) then
        indi=1
        call icsei(indi,nui,u(lui),dtv(ly0),dtv(ly0u),
     &    itv(litu),dtv(ldtu),
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
        if (indi.le.0) then
           ind=indi
           return
        endif
      endif
c
c appel de icse1
c but
c     icse1 resout les systemes dynamiques du type:
c        0=fi(t,y,u),i<=nea et dyj/dt=fj(t,y,u),nea<j<=ny pour t>=t0
c        et y(t0)=y0,en notant ny la dimension de l'etat y et
c        fk la keme composante de la fonction f
c algorithme
c
c     on procede a pas de temps constant:dt suivant deux echelles
c     apres p pas,on a obtenu y_p valeur de l'etat a l'instant t_p
c     soit y_p= (y_p(1),....,y_p(ny))
c     on veut calculer d(y_p)=y_p+1-y_p
c     on note dt=t_(p+1)-t_p et I la matrice diagonale d'ordre ny
c     dont les nea premiers coefficients diagonaux valent 0 et les
c     autres 1.
c
c     prediction:
c     I*d(0,y_p)=dt f(t_p,y_p,u)
c
c     correction:
c     apres q corrections,on approche l'egalite souhaitee:
c     (1/dt)I*d(q+1,y_p)=(1/2)[I*f(t_p,y_p,u)+f(t_p+1,y_p+d(q+1,y_p),u)]
c     par:
c     (1/dt)I*d(q+1,y_p)=(1/2)dfy(t_p,y_p,u)dqp+
c            (1/2)[I*f(t_p,y_p,u)+f(t_p+1,y_p+d(q+1,y_p),u)]
c        en notant dqp=d(q+1,y_p)-d(q,y_p)
c     soit par:
c        ((1/dt)I-(1/2)dfy(t_p,y_p,u))dqp=
c        (1/2)[I*f(t_p,y_p,u)+f(t_p+1,y_p+d(q,y_p),u)]-(1/dt)d(q,y_p)
c
c     on retient d(y_p)=d(q0,y_p),ou q0 est le premier entier non nul
c     tel que la norme l2 de:(1/2)[I*f(t_p,y_p,u)+
c                            f(t_p+1,y_p+d(q0,y_p),u)]-(1/dt)d(q0,y_p)
c     est inferieure a ermx
c     de plus le nombre des corrections ne doit pas depasser:
c     itmx
c
c remarque:
c     L'erreur de discretisation est en O(dt**2) dans cette methode
c     car l'erreur e commise a chaque pas dt est en (dt**3),et si
c     l'on prend dt'=(1/s)*dt,l'erreur e' commise a chaque pas dt'
c     est e'=(1/s**3)*e;alors pour atteindre tf=nt*dt,il faut
c     nt pas dt,d'ou l'erreur E_nt=nt*O(dt**3),et il faut s*nt pas dt'
c     d'ou l'erreur E'_nt=s*nt*e'=(1/s**2)*E_nt
c
c liste d'appel:
c     icse1(ind,nu,u,icsef,y0,ytot,f,b,fy,fu,ipv1,dm,yold,smold,yint,
c     yerr,dif1,dif2,dif3,itu,dtu,
c     t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c     itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c     en entree:
c
c     ind,u,nu figurent dans la liste d'appel de icse.fortran
c
c     icsef    nom de subroutine appelee par icse1
c
c     y0         double precision (ny)
c                etat initial
c
c     b          double precision (ny)
c                terme constant dans le cas lineaire quadratique
c
c     en sortie:
c
c     ytot       double precision (ny,nti+ntf)
c                valeurs calculees de l'etat aux pas de temps
c
c     variables internes:
c
c     f          double precision (ny)
c                stockage d'un calcul inutile des seconds membres
c                (au cas ou l'on n'utiliserait pas indf)
c
c     fy         double precision (ny,ny)
c                stockage de la derivee des seconds membres
c                par rapport a l'etat
c
c     fu         double precision (ny,nuc+nuv)
c                stockage de la derivee des seconds membres
c                par rapport aux parametres
c
c     ipv1       entier (ny)
c                stockage du vecteur des indices des pivots donne par
c                dgefa a chaque factorisation du jacobien
c
c     dm         double precision (ny,ny)
c                matrices successives des systemes lineaires
c                donnant l'etat discretise
c                dm=(1/dt)I-(1/2)dfy
c
c     yold       double precision (ny)
c                valeurs calculees successives de l'etat
c
c     smold      double precision (ny)
c                stockage de I*f(yold)
c
c     yint       double precision (ny)
c                yint=yold+dif1,ou dif1 est l'ecart donne
c                par prediction
c
c     yerr       double precision (ny)
c                yerr=yold+dif3,ou dif3 est l'ecart donne
c                par correction
c
c     dif1       double precision (ny)
c                ecart donne par prediction
c
c     dif2       double precision (ny)
c                stockage des differences entre les ecarts
c                consecutifs et des erreurs apres correction
c
c     dif3       double precision (ny)
c                ecart donne par correction
c
c     itu        entier (nitu)
c                tableau de travail entier reserve a l'utilisateur
c
c     dtu        double precision (ndtu)
c                tableau de travail double precision reserve
c                a l'utilisateur
c
c     enfin:
c
c     kt         entier
c                indice de comptage des pas de temps
c
c     dt         double precision
c                pas de temps,egal a dti ou a dtf
c
c     dtinv      double precision
c                dtinv=1/dt
c
c     t          double precision
c                instant(a l'instant t on travaille sur [t-dt,t])
c
c     told       double precision
c                instant anterieur a t:told=t-dt
c
c     indf       entier
c                indicateur figurant dans la liste d'appel de icsef
c
c     it         entier
c                indice de comptage des corrections
c
c     err        double precision
c                norme l2 de dif2
c
      call icse1(ind,nu,u,icsef,dtv(ly0),dtv(lytot),dtv(lf),dtv(lb),
     &dtv(lfy),dtv(lfu),itv(lipv1),dtv(ldm),dtv(lyold),dtv(lsmold),
     &dtv(lyint),dtv(lyerr),dtv(ldif1),dtv(ldif2),dtv(ldif3),
     &itv(litu),dtv(ldtu),
     &t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
      if (ind.le.0) return
c
c appel de icse2
c but
c     icse2 calcule le gradient du cout en utilisant l'etat
c     adjoint du systeme discretise
c algorithme
c     On procede a pas de temps constant:dt suivant deux echelles
c     si nt est le nombre total de pas de temps,notons:
c     y_1,...,y_nt les valeurs de la variable d'etat y a chaque pas
c     p_1,...,p_nt l'etat adjoint discretise
c     avec pour tout l=1,...,nt
c     y_l=(y_l(1),...,y_l(ny)) et p_l=(p_l(1),...,p_l(ny))
c     c2 la fonction cout
c     Id la matrice identite d'ordre ny
c     I la matrice diagonale d'ordre ny dont les nea premiers
c       coefficients diagonaux valent 0 et les autres 1
c     (M)t la transposee de la matrice M
c     L'etat adjoint discretise est la solution du systeme:
c        dc2/dy_nt=(I-(dt/2)dfy(t_nt,y_nt,u))t*p_nt avec
c                  dt=t_nt-t_(nt-1)
c        dc2/dy_k+(Id+(dt/2)dfy(t_k,y_k,u))t*I*p_k+1=
c        (I-(dt2new)dfy(t_k,y_k,u))t*p_k pour k=1,...,nt-1
c        avec dc2/dy_l nul quand l n'est pas un indice d'instant de
c        mesure
c     A chaque pas,apres avoir calcule p_l,on calcule
c     la contribution au gradient au pas l+1:
c     (dt/2)(dfu(t_l,y_l,u)+dfu(t_l+1,y_l+1,u))t*p_(l+1)) avec
c     dt=t_(l+1)-t_l
c     qu'on ajoute pour obtenir finalement le gradient en prenant
c     si l=1 la contribution:
c     ((t_1-t0)/2)(dfu(t0,y0,u)+dfu(t_1,y_1,u))t*p_1.
c     L'etat adjoint n'est pas stocke.
c
c liste d'appel:
c     icse2(ind,u,nu,co,g,icsef,icsec2,icsei,y0,tob,obs,ob,ytot,f,b,
c     fy,fu,ipv2,itob,cof,ytob,c2y,y0u,dmy,smy,oldmu,y,oldp,p,p0,gt,
c     yob,d,itu,dtu,
c     t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
c     itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c     en entree:
c
c     ind,u,nu   figurent dans la liste d'appel de icse.fortran
c
c     icsef      nom de subroutine appelee par icse2
c
c     icsec2     nom de subroutine appelee par icse2
c
c     icsei      nom de subroutine appelee par icse2
c
c     y0         double precision (ny)
c                etat initial
c
c     tob        double precision (ntob)
c                instants de mesure
c
c     obs        double precision (nob,ny)
c                matrice d'observation
c                figure dans la liste d'appel de icsec2,qui calcule
c                le cout quadratique dans le cas d'un observateur
c                lineaire
c
c     ob         double precision (nex,ntob,nob)
c                mesures
c
c     ytot       double precision (ny,nti+ntf)
c                valeurs calculees de l'etat aux pas de temps
c
c     b          double precision (ny)
c                terme constant dans le cas lineaire quadratique
c
c     en sortie:
c
c     co,g       figurent dans la liste d'appel de icse.fortran
c
c     variables internes:
c
c     f          double precision (ny)
c                stockage d'un calcul inutile des seconds membres
c                (au cas ou l'on n'utiliserait pas indf)
c
c     fy         double precision (ny,ny)
c                stockage de la derivee des seconds membres
c                par rapport a l'etat
c
c     fu         double precision (ny,nuc+nuv)
c                stockage de la derivee des seconds membres
c                par rapport aux parametres
c
c     ipv2       entier (ny)
c                stockage du vecteur des indices des pivots donne par
c                dgefa a chaque factorisation du jacobien
c
c     itob       entier (ntob)
c                indices des instants de mesure
c
c     cof        double precision (nob,ntob)
c                coefficients de ponderation du cout
c
c     ytob       double precision (ny,ntob)
c                valeurs calculees de l'etat aux instants de mesure
c
c     y0u        double precision (ny,nui)
c                derivee de l'etat initial par rapport au controle
c
c     dmy        double precision (ny,ny)
c                matrices successives des systemes lineaires
c                donnant l'etat adjoint discretise
c                dmy=I-(dt/2)dfy
c
c     smy        double precision (ny,ny)
c                matrices successives conduisant aux seconds membres
c                des systemes lineaires de l'etat adjoint discretise
c
c     oldmu      double precision (ny,nuc+nuv)
c                stockage de df/du a l'instant posterieur
c
c     y          double precision (ny)
c                stockage de la valeur calculee de l'etat a un pas de
c                temps
c
c     oldp       double precision (ny)
c                stockage de la valeur calculee de l'etat adjoint au
c                pas de temps posterieur
c
c     p          double precision (ny)
c                stockage de la valeur calculee de l'etat adjoint a
c                un pas de temps
c
c     p0         double precision (ny)
c                etape dans le calcul des seconds membres des
c                systemes lineaires donnant l'etat adjoint dicretise
c
c     gt         double precision (nu)
c                stockage de la contribution au gradient a chaque
c                pas de temps
c
c     yob,d      figurent dans la liste d'appel de icsec2,qui calcule
c                le cout quadratique dans le cas d'un observateur
c                lineaire
c
c     itu        entier (nitu)
c                tableau de travail entier reserve a l'utlisateur
c
c     dtu        double precision (ndtu)
c                tableau de travail double precision reserve
c                a l'utilisateur
c
c     enfin:
c
c     kt         entier
c                indice de comptage des pas de temps
c
c     ktob       entier
c                indice de comptage des instants de mesure
c
c     dt         double precision
c                pas de temps,egal a dti ou a dtf
c
c     dt2        double precision
c                dt2=dt/2
c
c     dt2new      double precision
c                dt2 a l'instant posterieur
c
c     t          double precision
c                instant (a l'instant t on travaille sur [t,t+dt])
c
c     c2         double precision
c                stockage d'un calcul inutile du cout
c                (au cas ou l'on n'utiliserait pas indc)
c
c     indf       entier
c                indicateur figurant dans la liste d'appel de icsef
c
c     indi       entier
c                indicateur figurant dans la liste d'appel de icsei
c
c     nui        entier
c                nombre de parametres dont depend l'etat initial
c                (figure dans la liste d'appel de icsei)
c
      call icse2(ind,nu,u,co,g,icsef,icsec2,icsei,dtv(ly0),dtv(ltob),
     &dtv(lobs),dtv(lob),dtv(lytot),dtv(lf),dtv(lb),dtv(lfy),dtv(lfu),
     &itv(lipv2),itv(litob),dtv(lcof),dtv(lytob),dtv(lc2y),dtv(ly0u),
     &dtv(ldmy),dtv(lsmy),dtv(loldmu),dtv(ly),dtv(loldp),
     &dtv(lp),dtv(lp0),dtv(lgt),dtv(lyob),dtv(ld),itv(litu),dtv(ldtu),
     &t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &itmx,nex,nob,ntob,ntobi,nitu,ndtu)
      do 20 i=1,nu
      g(i)=dtv(lech+i-1)*g(i)
      u(i)=dtv(ludep+i-1)
20    continue
      return
c
c format
c
 8003 format(1x,'icse : taille des tableaux itv,dtv insuffisante',/,
     +       8x,'valeurs minimales ',i6,2x,i6)
c
c fin
c
      end
