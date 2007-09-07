//                sero.bas : demo de icse
//  calcul coefficients optimaux du modele simplifie 5ht-plaquette
//  **************************************************************
//
// les doubles slash introduisent des commentaires
//
// contexte : tue les variables de nom reserve
// Copyright INRIA
libn=ilib_for_link('icsest','icsest.o',[],'f')
nlink=link('./'+libn,'icsest','f')

//clear icse contexte
clear nu u uc uv itu dtu y0 tob binf bsup b fy fu obs don;
t0=0.d0;    // instant initial
tf=18.d1;   // instant final
dti=1;  // premier pas de temps
dtf=2;   // second pas de temps
ermx=1.d-9; // test d'arret absolu sur la valeur du second membre dans
            // la resolution de l'etat
iu=[0,0,1]; //  iu   :indications sur la structure du controle
    //    iu(1)=1 si l'etat initial depend du controle constant,0 sinon
    //    iu(2)=1 si l'etat initial depend du controle variable,0 sinon
    //    iu(3)=1 si le second membre depend du controle constant,0 sinon
nuc=7;      // nombre de parametres independants du temps
nuv=0;      // nombre de parametres dependants du temps
ilin=2;     // indicateur de linearite :
       // 0 pour un systeme non affine
       // 1 pour un systeme affine dont la partie lineaire n'est pas autonome
       // ilin=2 pour un systeme affine dont la partie lineaire est autonome
nti=80;   //nombre de pas de temps correspondant a dti (premier pas de temps)
ntf=50;   // nombre de pas de temps correspondant a dtf (second pas de temps)
            // si l'on utilise un seul pas de temps,on doit prendre ntf=0
ny=4;       // dimension de l'etat a un instant donne
nea=0;      // nombre d'equations algebriques (eventuellement nul)
itmx=10;    // nombre maximal d'iterations dans la resolution de
            // l'equation d'etat discrete a un pas de temps donne
nex=8;      // nombre d'experiences effectuees
nob=2;      // dimension du vecteur des mesures pour une experience donnee
            // en un instant donne
ntob=9;     // nombre d'instants de mesure pour une experience donnee
ntobi=6;    // nombre d'instants de mesure correspondant a dti (premier
            // pas de temps)
// ne pas modifier l'instruction suivante
nu=nuc+nuv*(nti+ntf+1); // dimension du vecteur des parametres de controle
//
//  uc(1,nuc)          :controle constant
ucref=[2.d-4,1.d-3,1.d-2,5.d-3,2.d-2,1.5d-1,3.d-2];
uc=.1*ucref;
//  uv(1,nuv*(nti+ntf)):controle variable
//if nuv>0, uv(1,nuv*(nti+ntf))=0; end;
//  itu(1,nitu)        :tableau de travail entier reserve a
//                      l'utilisateur
itu=[0];
//  dtu(1,ndtu)        :tableau de travail double precision reserve
//                      a l'utilisateur
dtu=[0.d0];
//
//  y0(ny)              :etat initial
//  (inutile si iu(1) ou iu(2) est non nul)
y0=[4.d1,0.d0,0.d0,0.d0];
//  tob(1,ntob)        :instants de mesure (compatibilite avec ntob
//                      et ntobi)
tob=[1.d1,2.d1,3.d1,4.d1,6.d1,8.d1,1.1d2,1.6d2,1.8d2];
binf=1.d-17*ones(1,nu);//  borne inf des parametres
bsup=1.d1*ones(1,nu);// borne sup des parametres
//
//  termes utiles pour une dynamique lineaire ou une observation quadratique
// b(1,ny)=0;        // terme constant d'une dynamique lineaire
// fy(ny,ny)=0;      // derivee de la dynamique par rapport a l'etat
// fu(ny,nuc+nuv)=0; // derivee de la dynamique par rapport au controle
obs=[0,1,1,1;0,1,0,1]; // matrice d'observation obs(nob,ny)
//
//  don(nex*ntob*nob)  :mesures prealablement entrees dans le fichier
//                      sero.mes.Il s'agit de donnees simulees avec
//                      uc=[2.d-4,1.d-3,1.d-2,1.d-7,1.d-6,1.d-9,1.d-7]
don=read(p1+'sero.mes',1,nex*ntob*nob,'(5d15.7)');
//

nap=20;     // nombre d'appels du simulateur
imp=2;      // niveau de debug pour optim
large=100;  // taille de nu au dela de laquelle on choisit un optimiseur
       // pour les problemes de grande taille (alg='gc' dans l'appel de optim)
//
exec('icseinit.sce');
u0=u
[co,u,g,itv,dtv]=icse(u0,'icsest',nap,imp);
plot(u0)
plot(u)
ulink(nlink)
