//  PROGRAMME GENEAL
//  Version 5.4 20051103
//  Copyright Jacques-Deric Rouault
//  Designed for Scilab 3.1.1 & 4.0 under Windows XP & Linux

function demo_geneal()

demo_help demo_geneal


//SetPosition();

mprintf ("\nPROGRAMME GENEAL : TRACE D''UN ARBRE GENEALOGIQUE POLAIRE\n");
mprintf ("PROGRAM GENEAL : DRAWING A POLAR GENEALOGICAL TREE\n");
racine_prog = "Geneal";

// ETAPE/STEP 01 : REPERTOIRE DE TRAVAIL / WORKING DIRECTORY
[fsci,erreur] = mopen (racine_prog + ".sci","r");
repertoire_prog = getcwd ();
if erreur<>0 then
  repertoire_prog=tk_getdir(title="REPERTOIRE DE L''APPLICATION\nAPPLICATION DIRECTORY");
  chdir (repertoire_prog);
  [fsci,erreur] = mopen (racine_prog + ".sci","r");
  if erreur<>0 then 
    mprintf ("\nLE REPERTOIRE COURANT NE CONTIENT PAS LE FICHIER " + racine_prog + ".sci\n");
    mprintf ("THE CURRENT DIRECTORY DOES NOT CONTAIN THE FILE " + racine_prog + ".sci\n\n");
    abort; 
  end;  
end;
mclose (fsci);

// ETAPE/STEP 02 : RECHERCHE DU FICHIER GENEAL.TXT / LOOKING FOR THE FILE GENEAL.TXT
//nom_fichier=tk_getfile("*.txt",title="RECHERCHE DU FICHIER GENEAL - LOOKING FOR THE FILE GENEAL");
nom_fichier = 'Geneal.txt';

// ETAPE/STEP 03 : OUVERTURE DU FICHIER GENEAL.TXT / OPENING THE FILE GENEAL.TXT
[fg,err] = mopen (nom_fichier,"r");
if err<>0 then
  mprintf ("LE FICHIER " + nom_fichier + " N''A PAS PU ETRE OUVERT\n");
  mprintf ("THE FILE " + nom_fichier + " CANNOT BE OPEN\n");
  abort; 
end;

// ETAPE/STEP 04 : LECTURE DE L''ENTETE / READING HEADER
str = mgetl (fg,1);
if str == "GENEAL-V5" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
str = mgetl (fg,1);

// ETAPE/STEP 05 : LECTURE DE LA SECTION PHYSIC / READING SECTION PHYSIC
str = mgetl (fg,1);
if str == "PHYSIC" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
titre = mgetl (fg,1);
str = mgetl (fg,1);
[xd,yd] = sscanf (str,"%i %i");
str = mgetl (fg,1);
npixels = sscanf (str,"%i");

// ETAPE/STEP 06 : CREATION DE LA FENETRE GRAPHIQUE / CREATING THE GRAPHIC WINDOW
hf = scf (1);
SetPosition();
hf.figure_position = [xd,yd];
hf.figure_name = titre;
hf.figure_size = [npixels,npixels];

ha = hf.children;
ha.axes_visible = "off";
ha.isoview = "on";
ha.data_bounds = [-1,-1; 1,1];
ha.margins=[0.02, 0.02, 0.02, 0.02];
xsegs ([-1,1], [0,0], 1);
hh = gce ();
hh.segs_color = 4;
hh.foreground = 4;
xsegs ([0,0], [-1,1], 1);
hh = gce ();
hh.segs_color = 4;
hh.foreground = 4;
xarc (-1, 1, 2, 2, 0, 23040);
hh = gce ();
hh.foreground = 4;
drawlater;

// ETAPE/STEP 07 : LECTURE DE LA SECTION DATES / READING SECTION DATES
str = mgetl (fg,1);
if str == "DATES" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
str = mgetl (fg,1);
datedeb = sscanf (str,"%i");
xstring (0,0,str);
hh = gce ();
hh.foreground = 4;

str = mgetl (fg,1);
datefin = sscanf (str,"%i");
xstring (1,0,str);
hh = gce ();
hh.font_angle = 270;
hh.foreground = 4;
xstring (-1,0,str);
hh = gce ();
hh.font_angle = 90;
hh.foreground = 4;
xstring (0,1,str);
hh = gce ();
hh.foreground = 4;
xstring (0,-1,str);
hh = gce ();
hh.font_angle = 180;
hh.foreground = 4;

str = mgetl (fg,1);
ndates = sscanf (str,"%i");
ha.foreground = 4;
for nd = 1:ndates do
  str = mgetl (fg,1);
  datecer = sscanf (str,"%i");
  rho = (datecer-datedeb)/(datefin-datedeb);
  xarc (-rho, rho, 2*rho, 2*rho, 0, 23040);
  xstring (rho,0,str);
  hh = gce ();
  hh.font_angle = 270;
  hh.foreground = 4;
  xstring (-rho,0,str);
  hh = gce ();
  hh.font_angle = 90;
  hh.foreground = 4;
  xstring (0,rho,str);
  hh = gce ();
  hh.foreground = 4;
  xstring (0,-rho,str);
  hh = gce ();
  hh.font_angle = 180;
  hh.foreground = 4;
end;
SetPosition();
drawnow;
drawlater;

// ETAPE/STEP 08 : LECTURE DE LA SECTION PERSONNES / READING SECTION PERSONNES
str = mgetl (fg,1);
if str == "PERSONNES" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
str = mgetl (fg,1);
npersonnes = sscanf (str,"%i");

// ETAPE/STEP 9 : ANALYSE DES PERSONNES / ANALYSING PERSONS
tpers = [];
for np = 1:npersonnes do
  str = mgetl (fg,1);
  [rang1,s,ddeb,dfin,rang2,nom1] = sscanf (str,"%i %i %i %i %i %s");
  if s<1 or s>2 then
    mclose (fg);  
    mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
    mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
    abort;
  end;
  tpers (rang1,1)=s;
  if dfin>datefin then dfin=datefin; end;
  tpers (rang1,2)=ddeb;
  tpers (rang1,3)=dfin;
  tpers (rang1,4)=0;
  tpers (rang1,5)=rang2;
  nom2 = strsubst (nom1,"_"," ");
  tnom(rang1) = nom2;
end;
n1 = size (tpers,1);

// ETAPE/STEP 10 : LECTURE DE LA SECTION SECTEURS / READING SECTION SECTORS
str = mgetl (fg,1);
if str == "SECTEURS" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
str = mgetl (fg,1);
nsecteurs = sscanf (str,"%i");
pasangle = 23040/nsecteurs;

// ETAPE/STEP 11 : ANALYSE DES SECTEURS / ANALYSING SECTORS
for ns = 1:nsecteurs do
  str = mgetl (fg,1);
  [rang,typsect]= sscanf (str,"%i %i");

  if typsect==0 then
  end;  
  if ((typsect>0) & (typsect<5)) then
    [rang,typsect,pers]= sscanf (str,"%i %i %i");
    tpers(pers,4)=ns;
    angler = pasangle*ns/64*%pi/180; // radians
    xdeb = 1.02*cos(angler);
    ydeb = 1.02*sin(angler);
    npers = sprintf("%i",pers);
    xstring (xdeb,ydeb,npers,0,0);
    angler = pasangle*ns/64*%pi/180; // radians
    rho = (tpers(pers,2)-datedeb)/(datefin-datedeb);
    xdeb = rho*cos(angler);
    ydeb = rho*sin(angler);
    rho = (tpers(pers,3)-datedeb)/(datefin-datedeb);
    xfin = rho*cos(angler);
    yfin = rho*sin(angler);
    xsegs ([xdeb,xfin],[ydeb,yfin]);
    hs = gce ();
    hs.thickness=2;
    angled = 360-pasangle*ns/64; // degrés
    xstring (xdeb,ydeb,tnom(pers),angled,0);
    ht = gce ();
    select tpers(pers,1)
      case 1 then hs.segs_color = 2; hs.foreground = 2; ht.foreground = 2;
      case 2 then hs.segs_color = 5; hs.foreground = 5; ht.foreground = 5;
    end;     
    if (typsect==2) | (typsect==4) then
      xsegs ([xdeb,xdeb],[ydeb,ydeb]);
      hmd = gce ();
      hmd.mark_mode = "on";
      hmd.mark_style = 14;
      select tpers(pers,1)
        case 1 then hmd.mark_foreground = 1; hmd.mark_background = 2;
        case 2 then hmd.mark_foreground = 1; hmd.mark_background = 5;
      end;        
    end;
    if (typsect==3) | (typsect==4) then
      xsegs ([xfin,xfin],[yfin,yfin]);
      hmf = gce ();
      hmf.mark_mode = "on";
      hmf.mark_style = 3;
      select tpers(pers,1)
        case 1 then hmf.mark_foreground = 1; hmf.mark_background = 2;
        case 2 then hmf.mark_foreground = 1; hmf.mark_background = 5;
      end;        
    end;
   end;
  if (typsect<0) | (typsect>4) then
    mclose (fg);  
    mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
    mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
    abort;
  end;  
end;
drawnow;
drawlater;

// ETAPE/STEP 12 : RACCORDS PARALLELES / PARALLELIC LINKS
for np = 1:n1 do
 if tpers(np,5)>0 then
   pers1 = np;
   pers2 = tpers(np,5);
   sect1 = tpers(pers1,4);
   sect2 = tpers(pers2,4);
   ddat = tpers (pers1,3);
   if (sect1==0) | (sect2==0) then
   else
     if sect2>sect1 then
       angle1 = pasangle*sect1;
       angle2 = pasangle*(sect2-sect1);
     else
       angle1 = pasangle*sect2;
       angle2 = pasangle*(sect1-sect2);
     end;
     rho = (ddat-datedeb)/(datefin-datedeb);
     xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
     hh = gce ();   
     select tpers(pers2,1)
       case 1 then hh.foreground = 2;
       case 2 then hh.foreground = 5;
     end;
   end;
 end;
end;
drawnow;
drawlater;

// ETAPE/STEP 13 : LECTURE DE LA SECTION EVENEMENTS / READING SECTION EVENTS
str = mgetl (fg,1);
if str == "EVENTS" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;   
str = mgetl (fg,1);
nevents = sscanf (str,"%i");

// ETAPE/STEP 14 : ANALYSE DES EVENEMENTS / ANALYSING EVENTS
drawlater;
for ne = 1:nevents do
  str = mgetl (fg,1);
  [rang,typevent]= sscanf (str,"%i %i");
  if (typevent<1) | (typevent>6) then
    mclose (fg);  
    mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
    mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
    abort;
  end;  
  if typevent==1 then
    [rang,typevent,dmar,pers1,pers2]= sscanf (str,"%i %i %i %i %i");
    sect1 = tpers(pers1,4);
    sect2 = tpers(pers2,4);
    if sect2>sect1 then
      angle1 = pasangle*sect1;
      angle2 = pasangle*(sect2-sect1);
    else
      angle1 = pasangle*sect2;
      angle2 = pasangle*(sect1-sect2);
    end;
    rho = (dmar-datedeb)/(datefin-datedeb);
    xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
    hh = gce ();
    hh.thickness = 3;
    hh.foreground = 6;
  end;
  if typevent==2 then
    [rang,typevent,ddec,pers1,pers2]= sscanf (str,"%i %i %i %i %i");
    sect1 = tpers(pers1,4);
    sect2 = tpers(pers2,4);
   if sect2>sect1 then
      angle1 = pasangle*sect1;
      angle2 = pasangle*(sect2-sect1);
    else
      angle1 = pasangle*sect2;
      angle2 = pasangle*(sect1-sect2);
    end;
   rho = (ddec-datedeb)/(datefin-datedeb);
    xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
    hh = gce ();
    hh.thickness = 3;
    hh.foreground = 1;
  end;
  if typevent==3 then
    [rang,typevent,dnai,pers1,pers2,pers3]= sscanf (str,"%i %i %i %i %i %i");
    sect1 = tpers(pers1,4);
    sect2 = tpers(pers2,4);
    sect3 = tpers(pers3,4);
    if sect3>sect1 then
      angle1 = pasangle*sect1;
      angle2 = pasangle*(sect3-sect1);
    else
      angle1 = pasangle*sect3;
      angle2 = pasangle*(sect1-sect3);
    end;
    rho = (dnai-datedeb)/(datefin-datedeb);
    xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
    hh = gce ();   
    select tpers(pers3,1)
      case 1 then hh.foreground = 2;
      case 2 then hh.foreground = 5;
    end;
    if sect2>sect1 then
      angle1 = pasangle*sect1;
      angle2 = pasangle*(sect2-sect1);
    else
      angle1 = pasangle*sect2;
      angle2 = pasangle*(sect1-sect2);
    end;
    rho = (dnai-datedeb)/(datefin-datedeb);
    xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
    hh = gce ();   
    select tpers(pers3,1)
      case 1 then hh.foreground = 2;
      case 2 then hh.foreground = 5;
    end;
    
    angle1 = pasangle*sect1/64*%pi/180; // radians
    xdeb = rho*cos(angle1);
    ydeb = rho*sin(angle1);
    xsegs ([xdeb,xdeb],[ydeb,ydeb]);
    hmd = gce ();
    hmd.mark_mode = "on";
    hmd.mark_style = 6;
    select tpers(pers3,1)
      case 1 then hmd.mark_foreground = 1; hmd.mark_background = 2;
      case 2 then hmd.mark_foreground = 1; hmd.mark_background = 5;
    end;        
    angle2 = pasangle*sect2/64*%pi/180; // radians
    xdeb = rho*cos(angle2);
    ydeb = rho*sin(angle2);
    xsegs ([xdeb,xdeb],[ydeb,ydeb]);
    hmd = gce ();
    hmd.mark_mode = "on";
    hmd.mark_style = 6;
    select tpers(pers3,1)
      case 1 then hmd.mark_foreground = 1; hmd.mark_background = 2;
      case 2 then hmd.mark_foreground = 1; hmd.mark_background = 5;
    end;        
  end;
  if typevent==4 then
    [rang,typevent,dnai,pers1,pers3]= sscanf (str,"%i %i %i %i %i");
    sect1 = tpers(pers1,4);
    sect3 = tpers(pers3,4);
    if sect3>sect1 then
      angle1 = pasangle*sect1;
      angle2 = pasangle*(sect3-sect1);
    else
      angle1 = pasangle*sect3;
      angle2 = pasangle*(sect1-sect3);
    end;
    rho = (dnai-datedeb)/(datefin-datedeb);
    xarc (-rho, rho, 2*rho, 2*rho, angle1, angle2);
    hh = gce ();   
    select tpers(pers3,1)
      case 1 then hh.foreground = 2;
      case 2 then hh.foreground = 5;
    end;
    angle1 = pasangle*sect1/64*%pi/180; // radians
    xdeb = rho*cos(angle1);
    ydeb = rho*sin(angle1);
    xsegs ([xdeb,xdeb],[ydeb,ydeb]);
    hmd = gce ();
    hmd.mark_mode = "on";
    hmd.mark_style = 6;
    select tpers(pers3,1)
      case 1 then hmd.mark_foreground = 1; hmd.mark_background = 2;
      case 2 then hmd.mark_foreground = 1; hmd.mark_background = 5;
    end;        
  end;
end;
drawnow;
drawlater;

// ETAPE/STEP 15 : LECTURE DE LA SECTION FAMILLES / READING SECTION FAMILLES
str = mgetl (fg,1); 
str = mgetl (fg,1);
nfams = sscanf (str,"%i");

// ETAPE/STEP 16 : ANALYSE DES FAMILLES / ANALYSING FAMILIES
for nf = 1:nfams do
  str = mgetl (fg,1);
  [rang,ddeb,dfin,nelems]= sscanf (str,"%i %i %i %i");
  if dfin>datefin then dfin=datefin; end;
  if nelems==1 then
    str = mgetl (fg,1);
  end;
  if nelems>1 then
    str = mgetl (fg,1);
    [rang,pers]= sscanf (str,"%i %i");
    
    sect1 = tpers(pers,4);
    sect2 = sect1;
    for ne=2:nelems do
      str = mgetl (fg,1);
      [rang,pers]= sscanf (str,"%i %i");
      sect=tpers(pers,4);
      if sect<sect1 then sect1=sect; end;
      if sect>sect2 then sect2=sect; end;
    end;
    angler1 = pasangle*(sect1-0.5)/64*%pi/180; // radians
    angler2 = pasangle*(sect2+0.5)/64*%pi/180; // radians
    rho1 = (ddeb-datedeb)/(datefin-datedeb)-0.02;
    rho2 = (dfin-datedeb)/(datefin-datedeb)+0.02;
    x11 = rho1*cos(angler1);
    y11 = rho1*sin(angler1);
    x12 = rho1*cos(angler2);
    y12 = rho1*sin(angler2);
    x21 = rho2*cos(angler1);
    y21 = rho2*sin(angler1);
    x22 = rho2*cos(angler2);
    y22 = rho2*sin(angler2);
    xsegs ([x11,x21],[y11,y21]);
    hs = gce ();
    hs.line_style=2;
    hs.foreground=13;    
    hs.segs_color=13;
    xsegs ([x12,x22],[y12,y22]);
    hs = gce ();
    hs.line_style=2;
    hs.foreground=13;    
    hs.segs_color=13;
    
    angle1 = pasangle*(sect1-0.5);
    angle2 = pasangle*(sect2-sect1+1);
    xarc (-rho1, rho1, 2*rho1, 2*rho1, angle1, angle2);
    hh = gce ();   
    hh.foreground = 13;
    hh.line_style=2;
    xarc (-rho2, rho2, 2*rho2, 2*rho2, angle1, angle2);
    hh = gce ();   
    hh.foreground = 13;
    hh.line_style=2;
  end;
end;
drawnow;

// ETAPE/STEP 17 : FERMETURE DES FICHIERS / CLOSING THE FILES
str = mgetl (fg,1);
if str == "FINEND" then 
else
  mclose (fg);  
  mprintf ("PROBLEME DE SYNTAXE : VOIR LE FICHIER MOUCHARD fichier.MCS\n");
  mprintf ("SYNTAX ERROR: SEE THE SNEAK FILE fichier.MCS\n");
  abort;
end;              

SetPosition();
realtimeinit(1.0) ;
for i=1:10
   realtime(i);
end;

zoom_rect([0.3,0.5;0.45,0.95]);

realtimeinit(1.0) ;
for i=1:10
   realtime(i);
end;

unzoom();
realtimeinit(1.0) ;
for i=1:10
   realtime(i);
end;

mclose (fg);
mprintf ("\nFin normale d''exécution du programme / Correct end of running\n");

xdel() ;

endfunction
