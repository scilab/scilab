//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

//                navet.bas : demo de icse
//  calcul trajectoire optimale de rentree d'une navette spatiale
//  *************************************************************

libn  = ilib_for_link("icsenb","icsenb.o",[],"f")
nlink = link("./"+libn,["icsenb","icsenf"],"f")

exec("icse.contexte");

t0   = 0.d0;  // instant initial
tf   = 1.d0;  // instant final
dtf  = 0;
ermx = 1.d-6; // test d'arret absolu sur la valeur du second membre dans
// la resolution de l'etat
iu = [0,0,1]; //  iu   :indications sur la structure du controle
//    iu(1)=1 si l'etat initial depend du controle constant,0 sinon
//    iu(2)=1 si l'etat initial depend du controle variable,0 sinon
//    iu(3)=1 si le second membre depend du controle constant,0 sinon
nuc  = 1;     // nombre de parametres independants du temps
nuv  = 1;     // nombre de parametres dependants du temps
ilin = 0;     // indicateur de linearite :
// 0 pour un systeme non affine
// 1 pour un systeme affine dont la partie lineaire n'est pas autonome
// ilin=2 pour un systeme affine dont la partie lineaire est autonome
nti = 150;    //nombre de pas de temps correspondant a dti (premier pas de temps)
dti = tf/nti;
ntf = 00;     // nombre de pas de temps correspondant a dtf (second pas de temps)
// si l'on utilise un seul pas de temps,on doit prendre ntf=0
ny   = 4;     // dimension de l'etat a un instant donne
nea  = 0;     // nombre d'equations algebriques (eventuellement nul)
itmx = 10;    // nombre maximal d'iterations dans la resolution de
// l'equation d'etat discrete a un pas de temps donne
nex = 1;      // nombre d'experiences effectuees
nob = 3;      // dimension du vecteur des mesures pour une experience donnee
// en un instant donne
ntob  = 1;    // nombre d'instants de mesure pour une experience donnee
ntobi = 1;    // nombre d'instants de mesure correspondant a dti (premier
// pas de temps)

nu=nuc+nuv*(nti+ntf+1); // dimension du vecteur des parametres de controle

//  uc(1,nuc)          :controle constant
echtf = 2000;
uc    = [2500/echtf];

//  uv(1,nuv*(nti+ntf)):controle variable
//if nuv>0 then uv(1,nuv*(nti+ntf+1))=0; end;
alpha0 = .20704/.029244; legu = "alpha initial : ann. cz";     // annulation cz
alpha0 = 17.391;         legu = "alpha initial : finesse max"; // finesse maximum
legu = " navette americaine ."+legu;
if nuv>0 then uv=alpha0*ones(1,nuv*(nti+ntf+1)); end;

//  itu(1,nitu)        :tableau de travail entier reserve a
//                      l'utilisateur
itu = [0];

//  dtu(1,ndtu)        :tableau de travail double precision reserve
//                      a l'utilisateur
raddeg = %pi/180;

//dtu=[ 249.9,         ..//s      1
//      .078540,       ..//cx0    2
//     -.0061592,      ..//cx1    3
//      .621408e-3,    ..//cx2    4
//     -.207040,       ..//cz0    5
//      .029244,       ..//cz1    6
//       83388,        ..//zm     7
//     3.9860119e14,   ..//zmu    8
//     6378166,        ..//rt     9
//     1.2,            ..//ro0   10
//     6700,           ..//h     11
//     raddeg,         ..//      12
//     echtf,          ..//echtf 13
//     0,0,0,0,0,0,0,  ..// inutilises    14 a 20
//     1000,           ..//mise a echelle v    21
//     1,              ..//mise a echelle gam  22
//     1.e5,           ..//mise a echelle z    23
//     1,           ..//mise a echelle l       24
//     1.e6      ];      //cpenal              25

dtu=[ 249.9,         ..
.078540,       ..
-.0061592,      ..
.621408e-3,    ..
-.207040,       ..
.029244,       ..
83388,        ..
3.9860119e14,   ..
6378166,        ..
1.2,            ..
6700,           ..
raddeg,         ..
echtf,          ..
0,0,0,0,0,0,0,  ..
1000,           ..
1,              ..
1.e5,           ..
1,           ..
1.e6      ];      //cpenal              25

y0=[7803, -1*raddeg, 121920, 0]; // etat initial
//          (valeur arbitraire si iu(1) ou iu(2) est non nul)

y0=y0./dtu(1,21:24); // mise a l'echelle de y0

//  tob(1,ntob)        :instants de mesure (compatibilite avec ntob
//                      et ntobi)
tob       = [1];
binf      = -20*ones(1,nu); // borne inf des parametres
binf(1,1) = 2500/echtf;
bsup      = 40*ones(1,nu); // borne sup des parametres
bsup(1,1) = 4000/echtf;

obs(nob,ny) = 0; // matrice d'observation

//don=[762,            ..//vfin         1
//     -5*raddeg,      ..//gamma final  2
//     24384    ];     ..//zfin         3
don=[762,            ..
-5*raddeg,      ..
24384    ];
don   = don./dtu(1,21:23); // mise a l'echelle
nomf  = "icsenf";          // noms de subroutines de dynamique
legfb = " croissant ";

// changements pour calculer en temps retrograde
retro = 1;
if retro>0 then
    legfb     = " retrograde ";
    don1      = don;
    don       = y0(1,1:3);
    y0(1,1:3) = don1;
    nomf      ="icsenb";
end

nap    = 20;  // nombre d'appels du simulateur
imp    = 2;   // niveau de debug pour optim
large  = 100; // taille de nu au dela de laquelle on choisit un optimiseur

// pour les problemes de grande taille (alg='gc' dans l'appel de optim)

exec("icseinit.sce");

[co,u,g,itv,dtv]=icse(u,nomf,nap,imp);
