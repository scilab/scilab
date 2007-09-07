//                     lqv.bas : demo de icse
//  *************************************************************
//
//
// Copyright INRIA
//clear icse contexte
clear nu u uc uv itu dtu y0 tob binf bsup b fy fu obs don;
//
t0=0;    // instant initial
tf=20;    // instant final
dti=1;  // premier pas de temps
dtf=1;  // second pas de temps
ermx=1.d-6; // test d'arret absolu sur la valeur du second membre dans
            // la resolution de l'etat
iu=[0,0,1]; //  iu   :indications sur la structure du controle
    //    iu(1)=1 si l'etat initial depend du controle constant,0 sinon
    //    iu(2)=1 si l'etat initial depend du controle variable,0 sinon
    //    iu(3)=1 si le second membre depend du controle constant,0 sinon
nuc=5;      // nombre de parametres independants du temps
nuv=1;      // nombre de parametres dependants du temps
ilin=2;     // indicateur de linearite :
       // 0 pour un systeme non affine
       // 1 pour un systeme affine dont la partie lineaire n'est pas autonome
       // ilin=2 pour un systeme affine dont la partie lineaire est autonome
nti=10;    //nombre de pas de temps correspondant a dti (premier pas de temps)
ntf=10;    // nombre de pas de temps correspondant a dtf (second pas de temps)
            // si l'on utilise un seul pas de temps,on doit prendre ntf=0
ny=4;       // dimension de l'etat a un instant donne
nea=0;      // nombre d'equations algebriques (eventuellement nul)
itmx=10;    // nombre maximal d'iterations dans la resolution de
            // l'equation d'etat discrete a un pas de temps donne
nex=1;      // nombre d'experiences effectuees
nob=2;      // dimension du vecteur des mesures pour une experience donnee
            // en un instant donne
ntob=10;     // nombre d'instants de mesure pour une experience donnee
ntobi=5;    // nombre d'instants de mesure correspondant a dti (premier
            // pas de temps)
//
nu=nuc+nuv*(nti+ntf+1); // dimension du vecteur des parametres de controle
//
//  uc(1,nuc)          :controle constant
uc=0*ones(1,nuc);
//  uv(1,nuv*(nti+ntf+1)):controle variable
if nuv>0, uv(1,nuv*(nti+ntf+1))=0; end;
//  itu(1,nitu)        :tableau de travail entier reserve a
//                      l'utilisateur
itu=[0];
//  dtu(1,ndtu)        :tableau de travail double precision reserve
//                      a l'utilisateur
dtu=[0];
//
//  y0(ny)             :etat initial
//          (valeur arbitraire si iu(1) ou iu(2) est non nul)
y0=ones(1,ny);
//  tob(1,ntob)        :instants de mesure (compatibilite avec ntob
//                      et ntobi)
tob=2*(1:10);
binf=-10*ones(1,nu);//  borne inf des parametres
bsup= ones(1,nu);// borne sup des parametres
//
//  termes utiles pour une dynamique lineaire ou une observation quadratique
b(1,ny)=0;          // terme constant d'une dynamique lineaire
fy=0.1*ones(ny,ny);    // derivee de la dynamique par rapport a l'etat
fu=ones(ny,nuc+nuv); // derivee de la dynamique par rapport au controle
//
obs(nob,ny)=0; // matrice d'observation
obs=ones(nob,ny);
//
don=0*ones(1,nex*ntob*nob);

nap=20;     // nombre d'appels du simulateur
imp=2;      // niveau de debug pour optim
large=100;  // taille de nu au dela de laquelle on choisit un optimiseur
       // pour les problemes de grande taille (alg='gc' dans l'appel de optim)
//
exec('icseinit.sce');
[co,u,g,itv,dtv]=icse(u,'icsemc',nap,imp);
