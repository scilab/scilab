//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [ok,a_domaine,b_domaine,discr,signe,choix,type_meth,degre,Nbr_maillage,..
    CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,..
    a6,b6,a7,b7,k,mesures,params_pde]=IHM_EDP(params_pde)
    // Cette fonction permet de dessiner l'IHM, on utilisant la commande uicontrol (voir le help Scilab)  //
    // Entree :                                                                                           //
    //    - params_pde (tlist) : rajouter a la list exprs du bloc EDP afin de sauvegarder les             //
    //                           informations de l'IHM. C'est une aussi variable de sortie.               //                                                    //
    // sorties :                                                                                          //
    //    - ok (booleen) : variable de gestion des erreurs et la sortie de la fonction.                   //
    //    - a_domaine et b_domaine (Entiers) : sont les bords du domaine [a b]                            //
    //    - discr (Entier) : renvoie le type du disciminant (0: consatnt ou 1: non constant)               //
    //    - signe (Entier) : renvoie le signe du discriminant dans les cas non constant                    //
    //              (1: positif, 2: négatif, 3: nul )                                                     //
    //    - choix (Entier) : renvoie le choix entre le mode manuel et le mode automatique (systeme expert) //
    //              (0 : Automatique, 1 : Manuel)                                                         //
    //    - type_meth (Entier) : renvoie le type de la methode de discretisation dans le cas manuel        //
    //                  (1 : differences finies, 2 : elements finis, 3 : volumes finis)                   //
    //    - degre (Entier) : renvoie l'ordre de la discretisation (1 ou 2 pour EF et DF, 1 pour VF)     //
    //    - Nbr_maillage (Entier) : renvoie le nombre de points de maillage                                //
    //    - CI et CI1 (String) : renvoient les expressions des conditions initiales                       //
    //    - CLa_type, CLb_type (Entiers) : renvoient les types des conditions aux limittes                //
    //                         resp en a et en b (0 : Dirichlet, 1 : Neumann)                             //
    //    - CLa_exp, CLb_exp (String) : renvoient les expressions des conditions aux                      //
    //                         limittes resp en a et en b                                                 //
    //    - oper (Entier) : vecteur codant les operateurs choisi ( 1 : d2u/dt2, 2 : d2u/dx2,              //
    //             3 : du/dt, 4 : d2u/dtdx, 5 : du/dx, 6 : u, 7 : f)                                      //
    //    - ai et bi (String) : renvoient les differents coeficients des operateurs (ai(x) et bi(t))      //                                                             //
    //    - k (Entier) : codant le nombre de ports d'entrees du bloc EDP                                  //
    //    - mesures (Vecteur d'entiers) : renvoie la liste des points de mesures                         //
    //----------------------------------------------------------------------------------------------------//

    // create a figure
    ok=%f;
    fin=%t;
    // On desactive les volumes finis pour le moment car il y a un bug dans l'implémentation des conditions
    // aux limites, mais sera réglé dans la prochaine version.
    //list_methode = ["Diff finies"  "Elts finis"  "Vols finis"];
    list_methode = [_("Finite differences")  _("Finite elements")];
    methode=strcat(list_methode,"|");

    list_discrimant = [_("positive") _("negative") _("null")];
    discrimant=strcat(list_discrimant,"|");

    list_points = params_pde.points;
    points=strcat(list_points,"|");

    f = figure("Position",[50 50 670 620],"figure_name","PDE_IHM","BackgroundColor",[0.7 0.9 0.4], "visible", "off");
    f.axes_size=[670,620];
    f.closerequestfcn = "do_quitter()";
    //m=uimenu(f,'label', 'Model EDP');
    // create an item on the menu bar
    //m1=uimenu(f,'label', 'Methode');
    //m2=uimenu(f,'label', 'Syst�me Expert');
    //m3=uimenu(f,'label', 'Generation', 'callback', "do_pdeode()");
    m4=uimenu(f,"label", "Documentation", "callback", "help()");
    m5=uimenu(f,"label", "Quitter", "callback", "do_quitter()");
    //create two items in the menu "windows"
    //m11=uimenu(m,'label', 'new model', 'callback',"show_window()");
    //m12=uimenu(m11,'label', 'clear  window', 'callback',"xbasc()");

    // Titre
    txt0 = uicontrol(f, "Position",[300 560 120 30],"Style","text","String",_("PDE block"),"fontname",...
    "Times Bold Italic","fontsize",21,"BackgroundColor",[0.7 0.9 0.4]);
    // Domaine
    frame1= uicontrol(f, "Position",[5 550 150 60],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt1 = uicontrol(f, "Position",[20 600 50 10],"Style","text","String",_("Domain"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    txt2 = uicontrol(f, "Position",[25 580 25 10],"Style","text","String","a =","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    edita = uicontrol(f, "Position"  , [65 575 70 17.5],"Style","edit","String",params_pde.a,...
    "BackgroundColor",[1 1 1]);
    txt3 = uicontrol(f, "Position",[25 560 25 10],"Style","text","String","b =","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editb = uicontrol(f, "Position"  , [65 555 70 17.5],"Style","edit","String",params_pde.b,...
    "BackgroundColor",[1 1 1]);

    // Infos EDP
    frame2= uicontrol(f, "Position",[5 240 450 300],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt4 = uicontrol(f, "Position",[20 530 120 10],"Style","text","String",_("PDE Specification"),...
    "fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);

    //*********************
    //expression
    frame3= uicontrol(f, "Position",[10 332.5 440 210],"Style","frame","BackgroundColor",[0.7 0.7 0.7]);
    txt5 = uicontrol(f, "Position",[20 513 100 10],"Style","text","String",_("PDE Expression:"),"fontsize",10,...
    "BackgroundColor",[0.7 0.7 0.7]);
    b1 = uicontrol(f, "Position"  , [130 511 60 15],"Style", "pushbutton","String", _("Display"),...
    "fontname","Times Bold Italic","fontsize",8,"callback" , "visualiser()","BackgroundColor",[0 0.7 1] );
    sh=-15;
    txt_exp = uicontrol(f, "Position",[15 502.5 430 10],"Style","text","String",...
    params_pde.txt_exp,"fontsize",10,"fontname","Times Bold Italic","BackgroundColor",[0.7 0.7 0.7]);
    txt_exp2 = uicontrol(f, "Position",[15 492.5 430 10],"Style","text","String",...
    "","fontsize",10,"fontname","Times Bold Italic","BackgroundColor",[0.7 ...
    0.7 0.7]);

    check_op1 = uicontrol(f, "Position"  , [15 470+sh 13 13],"Style", "checkbox","Value",params_pde.check_op1,"BackgroundColor",[1 1 1] );
    frame31= uicontrol(f, "Position",[40 457.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame311= uicontrol(f, "Position",[40 457.5+sh 45 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt311 = uicontrol(f, "Position",[45 467.5+sh 35 15],"Style","text","String","d2u/dt2","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame312= uicontrol(f, "Position",[85 457.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt22 = uicontrol(f, "Position",[95 476+sh 50 13],"Style","text","String","a1(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita1 = uicontrol(f, "Position"  , [90 462+sh 60 13],"Style","edit","String",params_pde.a1,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame313= uicontrol(f, "Position",[152.5 457.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt23 = uicontrol(f, "Position",[160 476+sh 50 13],"Style","text","String","b1(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb1 = uicontrol(f, "Position"  , [160 462+sh 60 13],"Style","edit","String",params_pde.b1,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op2 = uicontrol(f, "Position"  , [15 430 13 13],"Style", "checkbox","Value",params_pde.check_op2,"BackgroundColor",[1 1 1] );
    frame32= uicontrol(f, "Position",[40 417.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame321= uicontrol(f, "Position",[40 417.5+sh 45 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt321 = uicontrol(f, "Position",[45 427.5+sh 35 15],"Style","text","String","d2u/dx2","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame322= uicontrol(f, "Position",[85 417.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt24 = uicontrol(f, "Position",[95 436+sh 50 13],"Style","text","String","a2(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita2 = uicontrol(f, "Position"  , [90 422+sh 60 13],"Style","edit","String",params_pde.a2,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame323= uicontrol(f, "Position",[152.5 417.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt26 = uicontrol(f, "Position",[160 436+sh 50 13],"Style","text","String","b2(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb2 = uicontrol(f, "Position"  , [160 422+sh 60 13],"Style","edit","String",params_pde.b2,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op3 = uicontrol(f, "Position"  , [15 390+sh 13 13],"Style", "checkbox","Value",params_pde.check_op3,"BackgroundColor",[1 1 1] );
    frame33= uicontrol(f, "Position",[40 377.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame331= uicontrol(f, "Position",[40 377.5+sh 45 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt331 = uicontrol(f, "Position",[45 387.5+sh 30 15],"Style","text","String","du/dt","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame332= uicontrol(f, "Position",[85 377.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt27 = uicontrol(f, "Position",[95 396+sh 50 13],"Style","text","String","a3(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita3 = uicontrol(f, "Position"  , [90 382+sh 60 13],"Style","edit","String",params_pde.a3,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame333= uicontrol(f, "Position",[152.5 377.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt28 = uicontrol(f, "Position",[160 396+sh 50 13],"Style","text","String","b3(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb3 = uicontrol(f, "Position"  , [160 382+sh 60 13],"Style","edit","String",params_pde.b3,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op4 = uicontrol(f, "Position"  , [15 350+sh 13 13],"Style", "checkbox","Value",params_pde.check_op4,"BackgroundColor",[1 1 1] );
    frame34= uicontrol(f, "Position",[40 337.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame341= uicontrol(f, "Position",[40 337.5+sh 45 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt341 = uicontrol(f, "Position",[42.5 347.5+sh 37.5 15],"Style","text","String","d2u/dtdx","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame342= uicontrol(f, "Position",[85 337.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt223 = uicontrol(f, "Position",[95 356+sh 50 13],"Style","text","String","a4(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita4 = uicontrol(f, "Position"  , [90 342+sh 60 13],"Style","edit","String",params_pde.a4,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame343= uicontrol(f, "Position",[152.5 337.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt224 = uicontrol(f, "Position",[160 356+sh 50 13],"Style","text","String","b4(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb4 = uicontrol(f, "Position"  , [160 342+sh 60 13],"Style","edit","String",params_pde.b4,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op5 = uicontrol(f, "Position"  , [240 470+sh 13 13],"Style", "checkbox","Value",params_pde.check_op5,"BackgroundColor",[1 1 1] );
    frame35= uicontrol(f, "Position",[265 457.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame351= uicontrol(f, "Position",[265 457.5+sh 40 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt351 = uicontrol(f, "Position",[270 467.5+sh 30 15],"Style","text","String","du/dx","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame352= uicontrol(f, "Position",[305 457.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt29 = uicontrol(f, "Position",[310 476+sh 50 13],"Style","text","String","a5(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita5 = uicontrol(f, "Position"  , [310 462+sh 60 13],"Style","edit","String",params_pde.a5,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame353= uicontrol(f, "Position",[372.5 457.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt210 = uicontrol(f, "Position",[380 476+sh 50 13],"Style","text","String","b5(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb5 = uicontrol(f, "Position"  , [380 462+sh 60 13],"Style","edit","String",params_pde.b5,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op6 = uicontrol(f, "Position"  , [240 430 13 13],"Style", "checkbox","Value",params_pde.check_op6,"BackgroundColor",[1 1 1] );
    frame36= uicontrol(f, "Position",[265 417.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame361= uicontrol(f, "Position",[265 417.5+sh 40 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt361 = uicontrol(f, "Position",[270 427.5+sh 30 15],"Style","text","String","u","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame362= uicontrol(f, "Position",[305 417.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt211 = uicontrol(f, "Position",[310 436+sh 50 13],"Style","text","String","a6(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita6 = uicontrol(f, "Position"  , [310 422+sh 60 13],"Style","edit","String",params_pde.a6,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame363= uicontrol(f, "Position",[372.5 417.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt222 = uicontrol(f, "Position",[380 436+sh 50 13],"Style","text","String","b6(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb6 = uicontrol(f, "Position"  , [380 422+sh 60 13],"Style","edit","String",params_pde.b6,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    check_op7 = uicontrol(f, "Position"  , [240 390 13 13],"Style", "checkbox","Value",params_pde.check_op7,"BackgroundColor",[1 1 1] );
    frame37= uicontrol(f, "Position",[265 377.5+sh 180 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    frame371= uicontrol(f, "Position",[265 377.5+sh 40 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt371 = uicontrol(f, "Position",[270 387.5+sh 30 15],"Style","text","String","f","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    frame372= uicontrol(f, "Position",[305 377.5+sh 70 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt223 = uicontrol(f, "Position",[310 396+sh 50 13],"Style","text","String","a7(x)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    edita7 = uicontrol(f, "Position"  , [310 382+sh 60 13],"Style","edit","String",params_pde.a7,...
    "fontsize",8,"BackgroundColor",[1 1 1]);
    frame373= uicontrol(f, "Position",[372.5 377.5+sh 72.5 35],"Style","frame","BackgroundColor",[0.8 0.8 0.8]);
    txt224 = uicontrol(f, "Position",[380 396+sh 50 13],"Style","text","String","b7(t)","fontsize",8,...
    "BackgroundColor",[0.8 0.8 0.8]);
    editb7 = uicontrol(f, "Position"  , [380 382+sh 60 13],"Style","edit","String",params_pde.b7,...
    "fontsize",8,"BackgroundColor",[1 1 1]);

    // b1 = uicontrol(f, "Position"  , [270 345 130 20],"Style", "pushbutton","String", _("Display expression"),...
    // 		 "fontname","Times Bold Italic","fontsize",8,"callback"  , "visualiser()","BackgroundColor",[0 0.7 1] );

    // discriminant :
    txt5 = uicontrol(f, "Position",[25 310+sh 120 15],"Style","text","String",_("Discriminant type:"),...
    "fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);
    check1 = uicontrol(f, "Position"  , [155 315+sh 13 13],"Style","checkbox","Value",params_pde.discr_cst,"BackgroundColor",[1 1 1],...
    "callback", "execlusif_check1()");
    txt51 = uicontrol(f, "Position",[170 315+sh 100 15],"Style","text","String",_("Constant discriminant"),...
    "fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);
    check2 = uicontrol(f, "Position"  , [155 295+sh 13 13],"Style", "checkbox","Value",params_pde.discr_non_cst,"BackgroundColor",[1 1 1],...
    "callback", "execlusif_check2()");
    txt53 = uicontrol(f, "Position",[170 295+sh 130 15],"Style","text","String",...
    _("Variable discriminant:"),"fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);
    liste_signe = uicontrol(f, "Position"  , [320+sh 245 80 50],"Style","listbox", "String",discrimant,"Value",params_pde.signe,...
    "BackgroundColor",[1 1 1]);

    //la methode
    // Choix
    frame5= uicontrol(f, "Position",[5 130 450 100],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt81 = uicontrol(f, "Position",[25 220 180 10],"Style","text","String",_("Spatial discretization method"),...
    "fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);
    txt811 = uicontrol(f, "Position",[15 200 40 15],"Style","text","String",_("Choice:"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);

    //rad_automatique = uicontrol(f, "Position", [15 175 13 13],"Style", "radiobutton","BackgroundColor",[1 1 1],...
    //"callback", "execlusif_radaut()");
    rad_automatique = uicontrol(f, "Position", [15 175 13 13],"Style", "checkbox","Value",params_pde.rad_automatique,"BackgroundColor",[1 1 1],...
    "callback", "execlusif_radaut()");
    txt51 = uicontrol(f, "Position",[25 175 70 15],"Style","text","String",_("Automatic"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    //rad_manuel = uicontrol(f, "Position"  , [15 155 13 13],"Style", "radiobutton","BackgroundColor",[1 1 1],...
    //"callback", "execlusif_radman()");
    rad_manuel = uicontrol(f, "Position"  , [15 155 13 13],"Style", "checkbox","Value",params_pde.rad_manuel,"BackgroundColor",[1 1 1],...
    "callback", "execlusif_radman()");
    txt54 = uicontrol(f, "Position",[25 155 45 15],"Style","text","String",_("Manual"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    // Type
    txt82 = uicontrol(f, "Position",[105 200 30 15],"Style","text","String",_("Type:"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    liste_meth = uicontrol(f, "Position"  , [145 140 80 70],"Style","listbox", "String",methode,"Value",params_pde.methode,...
    "BackgroundColor",[1 1 1]);
    // Ordonnancement
    txt83 = uicontrol(f, "Position",[230 210 100 15],"Style","text","String",_("Ordering:"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editordre1 = uicontrol(f, "Position"  , [270 192.5 15 15],"Style","edit","String",params_pde.ord1,...
    "BackgroundColor",[1 1 1]);
    editordre2 = uicontrol(f, "Position"  , [270 175 15 15],"Style","edit","String",params_pde.ord2,...
    "BackgroundColor",[1 1 1]);
    editordre3 = uicontrol(f, "Position"  , [270 157.5 15 15],"Style","edit","String",params_pde.ord3,...
    "BackgroundColor",[1 1 1]);
    // Degre
    txt84 = uicontrol(f, "Position",[320 200 80 15],"Style","text","String",_("degree:"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editdegre = uicontrol(f, "Position"  , [400 200 15 15],"Style","edit","String",params_pde.degre,...
    "BackgroundColor",[1 1 1]);
    // Pas de maillage
    txt6 = uicontrol(f, "Position",[310 180 130 15],"Style","text","String",_("Number of nodes:"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    //affichage du pas de maillage
    editpas = uicontrol(f, "Position"  , [360 160 50 17],"Style","edit","String",params_pde.nnode,...
    "BackgroundColor",[1 1 1]);
    bb = uicontrol(f, "Position"  , [290 135 80 20],"Style", "pushbutton","String", _("Display step value"),...
    "fontname","Times Bold Italic","fontsize",8,"callback"  , "afficher()","BackgroundColor",[0 0.7 1] );

    txt_pas = uicontrol(f, "Position",[370 140 80 10],"Style","text","String",...
    params_pde.txt_pas,"fontsize",8,"fontname","Times Bold Italic","BackgroundColor",[0.9 0.9 0.9]);

    // Conditions Initiales
    frame6= uicontrol(f, "Position",[5 80 450 40],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt8 = uicontrol(f, "Position",[25 110 100 10],"Style","text","String",_("Initial conditions"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    txt9 = uicontrol(f, "Position",[20 90 50 15],"Style","text","String","u(x,t0) =","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editCI = uicontrol(f, "Position"  , [80 90 120 17],"Style","edit","String",params_pde.CI,...
    "fontname","Times  Bold Italic","BackgroundColor",[1 1 1]);
    txt91 = uicontrol(f, "Position",[250 90 70 15],"Style","text","String","du/dt|t0 =","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editCI1 = uicontrol(f, "Position"  , [330 90 100 17],"Style","edit","String",params_pde.dCI,...
    "fontname","Times  Bold Italic","BackgroundColor",[1 1 1]);
    // Conditions aux limites
    frame7= uicontrol(f, "Position",[5 5 210 70],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt10 = uicontrol(f, "Position",[25 65 130 10],"Style","text","String",_("Boundary condition on a"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    frame8= uicontrol(f, "Position",[245 5 210 70],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt11 = uicontrol(f, "Position",[270 65 130 10],"Style","text","String",_("Boundary condition on b"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    txt8111 = uicontrol(f, "Position",[15 45 40 15],"Style","text","String","Type :","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    popa = uicontrol(f, "Position"  , [80 40 80 20],"Style", "popupmenu",...
    "String", "Dirichlet|Neumann","Value",params_pde.CLa,"BackgroundColor",[1 1 0]);
    txt81112 = uicontrol(f, "Position",[255 45 40 15],"Style","text","String","Type :","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    popb = uicontrol(f, "Position"  , [330 40 80 20],"Style", "popupmenu",...
    "String", "Dirichlet|Neumann","Value",params_pde.CLb,"BackgroundColor",[1 1 0]);
    txt12 = uicontrol(f, "Position",[20 10 70 15],"Style","text","String",_("Expression ="),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editCLa = uicontrol(f, "Position"  , [100 10 90 20],"Style","edit","String",params_pde.CLa_exp,...
    "BackgroundColor",[1 1 1]);
    txt13 = uicontrol(f, "Position",[265 10 70 15],"Style","text","String",_("Expression ="),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    editCLb = uicontrol(f, "Position"  , [340 10 90 20],"Style","edit","String",params_pde.CLb_exp,...
    "BackgroundColor",[1 1 1]);


    // points de sortie :
    frame9= uicontrol(f, "Position",[465 240 200 300],"Style","frame","BackgroundColor",[0.9 0.9 0.9]);
    txt14 = uicontrol(f, "Position",[485 525 110 15],"Style","text","String",_("Measurement points"),"fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    txt141 = uicontrol(f, "Position",[470 500 30 15],"Style","text","String","u(xi) :","fontsize",10,...
    "BackgroundColor",[0.9 0.9 0.9]);
    txt142 = uicontrol(f, "Position",[500 500 160 15],"Style","text","String",_("The measurement points"),...
    "fontsize",10,"BackgroundColor",[0.9 0.9 0.9]);
    liste_pts = uicontrol(f, "Position"  , [530 330 100 150], "Style", "listbox", "String" ,points,...
    "BackgroundColor",[1 1 1]);
    edit_pt = uicontrol(f, "Position"  , [540 290 70 20],"Style", "edit","String","",...
    "BackgroundColor",[1 1 1]);
    b2 = uicontrol(f, "Position"  , [490 255 70 20],"Style","pushbutton","String",_("Add"),...
    "callback"  , "add_list_points()" ,"BackgroundColor",[0 1 0]);
    b5 = uicontrol(f, "Position"  , [580 255 70 20],"Style","pushbutton","String",_("Remove"),...
    "callback"  , "del_list_points()" ,"BackgroundColor",[0 0.7 1]);
    b3 = uicontrol(f, "Position"  , [500 90 50 20],"Style", "pushbutton","String", _("Validate"),...
    "callback"  , "do_valider()","BackgroundColor",[0 1 0] );
    b4 = uicontrol(f, "Position"  , [610 90 50 20],"Style", "pushbutton","String", _("Cancel"),...
    "callback"  , "do_quitter()","BackgroundColor",[1 0 0] );
    f.visible = "on";
    while fin,
    end
    if ok then
        close(f)
        a_domaine=[];
        b_domaine=[];
        discr=[];
        signe=[];
        choix=[];
        type_meth=[];
        degre=[];
        Nbr_maillage=[];
        CI=[];
        CI1=[];
        CLa_type=[];
        CLa_exp=[];
        CLb_type=[];
        CLb_exp=[];
        oper=[];
        a1=[];
        b1=[];
        a2=[];
        b2=[];
        a3=[];
        b3=[];
        a4=[];
        b4=[];
        a5=[];
        b5=[];
        a6=[];
        b6=[];
        a7=[];
        b7=[];
        k=[];
        mesures=[];
        params_pde=[];
        return;
    end
    ok=fin;

    // extraction des infos avant de fermer l'IHM
    [a_domaine,b_domaine,discr,signe,choix,type_meth,degre,Nbr_maillage,CI,CI1,CLa_type,CLa_exp,..
    CLb_type,CLb_exp,oper,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,mesures,k]=extraction_infos(edita,editb,..
    check1,check2,rad_automatique,rad_manuel,liste_signe,liste_meth,editdegre,editpas,editCI,editCI1,..
    popa,editCLa,popb,editCLb,check_op1,edita1,editb1,check_op2,edita2,editb2,check_op3,edita3,..
    editb3,check_op4,edita4,editb4,check_op5,edita5,editb5,check_op6,edita6,editb6,check_op7,edita7,..
    editb7,list_points);

    // sauvegarde de l'IHM dans params_pde
    params_pde.a=string(a_domaine);
    params_pde.b=string(b_domaine);
    params_pde.check_op1=string(get(check_op1,"Value"));
    params_pde.a1=get(edita1,"String");
    params_pde.b1=get(editb1,"String");
    params_pde.check_op2=string(get(check_op2,"Value"));
    params_pde.a2=get(edita2,"String");
    params_pde.b2=get(editb2,"String");
    params_pde.check_op3=string(get(check_op3,"Value"));
    params_pde.a3=get(edita3,"String");
    params_pde.b3=get(editb3,"String");
    params_pde.check_op4=string(get(check_op4,"Value"));
    params_pde.a4=get(edita4,"String");
    params_pde.b4=get(editb4,"String");
    params_pde.check_op5=string(get(check_op5,"Value"));
    params_pde.a5=get(edita5,"String");
    params_pde.b5=get(editb5,"String");
    params_pde.check_op6=string(get(check_op6,"Value"));
    params_pde.a6=get(edita6,"String");
    params_pde.b6=get(editb6,"String");
    params_pde.check_op7=string(get(check_op7,"Value"));
    params_pde.a7=get(edita7,"String");
    params_pde.b7=get(editb7,"String");
    params_pde.discr_cst=string(get(check1,"Value"));
    params_pde.discr_non_cst=string(get(check2,"Value"));
    params_pde.signe=string(signe);
    params_pde.rad_automatique=string(get(rad_automatique,"Value"));
    params_pde.rad_manuel=string(get(rad_manuel,"Value"));
    params_pde.methode=string(type_meth);
    params_pde.txt_pas=get(txt_pas,"String");
    params_pde.ord1=get(editordre1,"String");
    params_pde.ord2=get(editordre2,"String");
    params_pde.ord3=get(editordre3,"String");
    params_pde.degre=string(degre);
    params_pde.nnode=string(Nbr_maillage);
    params_pde.txt_exp=get(txt_exp,"String");
    params_pde.CI=CI;
    params_pde.dCI=CI1;
    params_pde.CLa=string(CLa_type);
    params_pde.CLa_exp=get(editCLa,"String");
    params_pde.CLb=string(CLb_type);
    params_pde.CLb_exp=get(editCLb,"String");
    params_pde.points=list_points;
    // fermeure du gui
    close(f)

endfunction

// développé par EADS-CCR
// ce  script est pour initialiser quelque paramètres de l'IHM et contient //
// des fonctions utiles pour l'IHM
// ------------------------------------------------------------------------//
function do_pdeode()
    disp("Tranlate");
endfunction

function do_quitter()
    fin=%f;
    ok=%t;
    [ok,fin]=resume(ok,fin);
endfunction

function do_valider()

    // verification du domaine
    if (get(edita,"String") == "" | get(editb,"String") == "") then
        messagebox(msprintf(_("Please give the discretization domain values")),"modal","error");
        fin=%t;
        fin=resume(fin);
    end

    // Test de renseignements sur les differents champs de l'IHM
    if (get(check_op1,"Value") == 1) then
        if (get(edita1,"String") == "" | get(editb1,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"d2u/dt2"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op2,"Value")== 1) then
        if (get(edita2,"String") == "" | get(editb2,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"d2u/dx2"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op3,"Value")== 1) then
        if (get(edita3,"String") == "" | get(editb3,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"du/dt"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op4,"Value")== 1) then
        if (get(edita4,"String") == "" | get(editb4,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"d2u/dtdx"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op5,"Value")== 1) then
        if (get(edita5,"String") == "" | get(editb5,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"du/dx"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op6,"Value")== 1) then
        if (get(edita6,"String") == "" | get(editb6,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"u"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op7,"Value")== 1) then
        if (get(edita7,"String") == "" | get(editb7,"String") == "") then
            messagebox(msprintf(_("Please give the ""%s"" operator coefficients"),"f"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end

    // verification du renseignement du signe du discriminant non constant
    if (get(check2,"Value")== 1) then
        if (get(liste_signe,"Value") == 0 ) then
            messagebox(msprintf(_("Please give the sign for the variable discriminant")),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end

    // verification des informations du choix manuel
    if (get(rad_manuel,"Value")== 1) then
        if (get(liste_meth,"Value") == 0 ) then
            messagebox(msprintf(_("Please give the discretization method type")),"modal","error");
            fin=%t;
            fin=resume(fin);
        end

        if (get(editdegre,"String") == "" ) then
            messagebox(msprintf(_("Please give the discretization method degree")),"modal","error");
            fin=%t;
            fin=resume(fin);
        end

        if (get(editpas,"String") == "" ) then
            messagebox(msprintf(_("Please give the number of discretization points")),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    // verification du renseignement des conditions intiales
    if (get(check_op1,"Value")==1 | get(check_op3,"Value")== 1 | get(check_op4,"Value")== 1) then
        if (get(editCI,"String") == "" ) then
            messagebox(msprintf(_("Please give the initial condition ""%s"""),"u(x,t0)"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    if (get(check_op1,"Value")== 1) then
        if (get(editCI1,"String") == "" ) then
            messagebox(msprintf(_("Please give the initial condition ""%s"""),"du/dx|t0"),"modal","error");
            fin=%t;
            fin=resume(fin);
        end
    end
    // verification du renseignement des conditions aux limites
    if (get(editCLa,"String") == "" | get(editCLb,"String") == "" ) then
        messagebox(msprintf(_("Please give the boundary conditions")),"modal","error");
        fin=%t;
        fin=resume(fin);
    end

    fin=%f; // Pour sortir sans problemes de l'IHM
    fin=resume(fin);
endfunction

function add_list_points()
    // développé par EADS-CCR
    // cette fonction gere l'ajout //
    // d'un point de mesure         //
    // ----------------------------//
    if edit_pt<>0&liste_pts<>0 then
        new = get(edit_pt,"String");
        newv=evstr(new);
        if (newv < evstr(get(edita,"String"))-%eps | newv > evstr(get(editb,"String"))+%eps) then
            messagebox("le point choisi est en dehors du domaine","modal","error");
            return;
        end
        if (list_points == "") then
            list_points=[];
        end
        list_points = [list_points  new];
        set(liste_pts,"String",strcat(list_points,"|"));
        list_points=resume(list_points)
    end
endfunction

function del_list_points()
    // développé par EADS-CCR
    // cette fonction gère la suppression //
    // d'un point de mesure         //
    // ----------------------------//
    if edit_pt<>0&liste_pts<>0 then
        del = get(liste_pts,"Value");
        if (list_points == "") then
            list_points=[];
        end
        list_points =[list_points(1:del-1) list_points(del+1:$)];
        set(liste_pts,"String",strcat(list_points,"|"));
        list_points=resume(list_points);
    end
endfunction

function visualiser()
    // développé par EADS-CCR
    // Cette fonction est pour l'affichage de l'EDP dans l'IHM //
    // selon les opérateurs choisis                            //
    // --------------------------------------------------------//

    expression=[];
    if (get(check_op1,"Value")== 1) then
        expression="("+get(edita1,"String")+")*b1(t)*d2u/dt2";
    end
    if (get(check_op2,"Value")== 1) then
        if isempty(expression) then
            expression="("+get(edita2,"String")+")*b2(t)*d2u/dx2";
        else
            expression=expression+"+("+get(edita2,"String")+")*b2(t)*d2u/dx2";
        end
    end
    if (get(check_op3,"Value")== 1) then
        if isempty(expression) then
            expression="("+get(edita3,"String")+")*b3(t)*du/dt";
        else
            expression=expression+"+("+get(edita3,"String")+")*b3(t)*du/dt";
        end
    end
    if (get(check_op4,"Value")== 1) then
        if isempty(expression) then
            expression="("+get(edita4,"String")+")*b4(t)*d2u/dtdx";
        else
            expression=expression+"+("+get(edita4,"String")+")*b4(t)*d2u/dtdx";
        end
    end
    if (get(check_op5,"Value")== 1) then
        if isempty(expression) then
            expression="("+get(edita5,"String")+")*b5(t)*du/dx";
        else
            expression=expression+"+("+get(edita5,"String")+")*b5(t)*du/dx";
        end
    end
    if (get(check_op6,"Value")== 1) then
        if isempty(expression) then
            expression="("+get(edita6,"String")+")*b6(t)*u";
        else
            expression=expression+"+("+get(edita6,"String")+")*b6(t)*u";
        end
    end
    if (get(check_op7,"Value")== 1) then
        if isempty(expression) then
            expression="Veuillez selectionner les operateurs";
        else
            expression=expression+"=("+get(edita7,"String")+")*b7(t)";
        end
    else
        expression=expression+"=0";
    end
    if (expression == "=0") then
        expression="Veuillez selectionner les operateurs";
    end
    sep=["+","*","-"," "];
    expression=cformatlinedp(expression,80,sep);
    if (size(expression,"*")== 1) then
        set(txt_exp,"String",expression(1));
        set(txt_exp2,"String","");
    elseif (size(expression,"*")== 2) then
        set(txt_exp,"String",expression(1));
        set(txt_exp2,"String",expression(2));
    end
endfunction

// Ces fonctions sont les callback des //
// différents rad_buttom pour assurer  //
// l'exlusivité du choix               //
// ----------------------------------- //
function []=execlusif_check1()
    // développé par EADS-CCR
    if (get(check2,"Value") == 1) then
        set(check2,"Value",0);
    end
endfunction

function []=execlusif_check2()
    // développé par EADS-CCR
    if (get(check1,"Value") == 1) then
        set(check1,"Value",0);
    end
endfunction

function []=execlusif_radaut()
    // développé par EADS-CCR
    if (get(rad_manuel,"Value") == 1) then
        set(rad_manuel,"Value",0);
    end
endfunction

function []=execlusif_radman()
    // développé par EADS-CCR
    if (get(rad_automatique,"Value") == 1) then
        set(rad_automatique,"Value",0);
    end
endfunction

function afficher()
    // développé par EADS-CCR
    // Cette fonction est pour l'affichage de le pas de maillage //
    // --------------------------------------------------------//

    expression= msprintf("%.10g",(evstr(get(editb,"String"))-evstr(get(edita,..
    "String")))/(evstr(get(editpas,"String"))-1));
    set(txt_pas,"String",expression);

endfunction
