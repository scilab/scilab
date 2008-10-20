//  PROGRAMME DA4-11 : Structure dynamique A : division sur place de quasi-carrés sans effet d'échelle
//  PROGRAM   DA4-11 : Dynamical stucture A :  division on place of quasi-squares without scale effects
//  Version 4.3 du 3 novembre 2005
//  Copyright Jacques-Deric Rouault
//  Designed for Scilab 3.1.1 & 4.0 under Windows XP & Linux
// Modified by Jean-Baptiste Silvy for the 17/11/2005 demo.

function demo_cellule()
demo_help demo_cellule

clear;
mprintf ("\nPROGRAMME DA4-11 : Structure dynamique A sur quasi-carrés sans adresse\n");
mprintf ("PROGRAM DA4-11 : Dynamical stucture A on quasi-squares without adresses\n");
racine_prog = "DA4-11";
ndivi=13; // Nombre de divisions
long =5000; // Longueur du rectangle initial
haut =3700; // Largeur du rectangle initial
ncoulmax = 32;

// ETAPE/STEP 01 : REPERTOIRE DE TRAVAIL / WORKING DIRECTORY
//[fsci,erreur] = mopen (racine_prog + ".sci","r");
//repertoire_prog = getcwd ();
//if erreur<>0 then
//  repertoire_prog=tk_getdir(title="REPERTOIRE DE L''APPLICATION\nAPPLICATION DIRECTORY");
//  chdir (repertoire_prog);
//  [fsci,erreur] = mopen (racine_prog + ".sci","r");
//  if erreur<>0 then 
//    mprintf ("\nLE REPERTOIRE COURANT NE CONTIENT PAS LE FICHIER " + racine_prog + ".sci\n");
//    mprintf ("THE CURRENT DIRECTORY DOES NOT CONTAIN THE FILE " + racine_prog + ".sci\n\n");
//    abort; 
//  end;  
//end;
//mclose (fsci);

long2 = long ;
haut2 = haut ;

nrects = 1 ;
rectd = [] ; // Niveau de divisions
rectc = [] ; // Coordonnées
rectp = [] ; // Pattern = couleur

rectd (1)   = 0 ;     //  Niveau de division
rectc (1,1) = 0 ;     // x coin supérieur gauche
rectc (2,1) = haut ;  // y coin supérieur gauche
rectc (3,1) = long ;  //  longueur
rectc (4,1) = haut ;  //  hauteur
rectp (1)   = 2 ;     //  Couleur

//  representation
nd = 0;
hf = scf (0);
nomfenetre = sprintf ("DA4-11 : Division numéro %d/%d",nd,ndivi);
hf.figure_name = nomfenetre;

ha = hf.children;
ha.axes_visible = "off";
ha.isoview = "on";
ha.margins=[0.01, 0.01, 0.01, 0.01];
ha.data_bounds = [0,0; long,haut];

//xsetech (frect=[0,0,long,haut]) ;
xrects (rectc,rectp);

for nd=1:ndivi do
  drawlater();
  //  division
  xpause (1000000);
  couleur = 1;
  if rectc(3,1)>rectc(4,1) then  // division en longueur est-ouest
    long2=long2/2;
  else
    haut2=haut2/2;
  end;  
  for nr=1:nrects do
    nrects=nrects+1;
    if rectc(3,nr)>rectc(4,nr) then  // division en longueur est-ouest
      // Ouest = nouveau rectangle nrects
      rectc (1:4,nrects)=rectc(1:4,nr);
      rectp (nrects)=rectp(nr);
      rectd (nrects)=nd;
      rectc (3,nrects)=long2;
      couleur=couleur+1;
      if couleur==ncoulmax then couleur=1; end;
      rectp (nrects)=couleur;

      // Est = ancien rectangle nr
      rectd (nr)=nd;
      rectc (1,nr)=rectc (1,nr)+long2;
      rectc (3,nr)=long2;
      couleur=couleur+1;
      if couleur==ncoulmax then couleur=1; end;
      rectp (nr)=couleur;
      // nord = inchangé
      // est = inchangé
      // sud = inchangé

    else  // division en hauteur nord-sud
      // nord = nouveau rectangle
      rectc (1:4,nrects)=rectc(1:4,nr);
      rectp (nrects)=rectp(nr);
      rectd (nrects)=nd;
      rectc (4,nrects)=haut2;
      couleur=couleur+1;
      if couleur==ncoulmax then couleur=1; end;
      rectp (nrects)=couleur;

      // sud = ancien rectangle
      rectd (nr)=nd;
      rectc (2,nr)=rectc (2,nr)-haut2;
      rectc (4,nr)=haut2;
      couleur=couleur+1;
      if couleur==ncoulmax then couleur=1; end;
      rectp(nr)=couleur;

    end; 
  end ;

  //  representation
  clf();
  SetPosition();
  hf = scf (0);
  nomfenetre = sprintf ("DA4-11 : Division numéro %d/%d",nd,ndivi);
  hf.figure_name = nomfenetre;

  ha = hf.children;
  ha.axes_visible = "off";
  ha.isoview = "on";
  ha.margins=[0.0, 0.0, 0.0, 0.0];
  ha.data_bounds = [0,0; long,haut];
 // xsetech (frect=[0,0,long,haut]) ;
  xrects (rectc,rectp);
  drawnow();
end ;

realtimeinit(i)
for i=1:10
  realtime(i);
end ;

endfunction

