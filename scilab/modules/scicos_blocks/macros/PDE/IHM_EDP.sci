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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
//    - discr (Entier) : renvoi le type du disciminant (0: consatnt ou 1: non constant)               //
//    - signe (Entier) : renvoi le signe du discriminant dans les cas non constant                    //
//              (1: positif, 2: négatif, 3: nul )                                                     //
//    - choix (Entier) : renvoi le choix entre le mode manuel et le mode automatique (systeme expert) //
//              (0 : Automatique, 1 : Manuel)                                                         //
//    - type_meth (Entier) : renvoi le type de la methode de discretisation dans le cas manuel        //
//                  (1 : differences finies, 2 : elements finis, 3 : volumes finis)                   //
//    - degre (Entier) : renvoi de l'ordre de la discritisation (1 ou 2 pour EF et DF, 1 pour VF)     //
//    - Nbr_maillage (Entier) : renvoi le nombre de points de maillage                                //
//    - CI et CI1 (String) : renvoient les expressions des conditions initiales                       //
//    - CLa_type, CLb_type (Entiers) : renvoient les types des conditions aux limittes                //
//                         resp en a et en b (0 : Dirichlet, 1 : Neumann)                             //
//    - CLa_exp, CLb_exp (String) : renvoient les expressions des conditions aux                      //  
//                         limittes resp en a et en b                                                 //
//    - oper (Entier) : vecteur codant les operateurs choisi ( 1 : d2u/dt2, 2 : d2u/dx2,              //
//             3 : du/dt, 4 : d2u/dtdx, 5 : du/dx, 6 : u, 7 : f)                                      //
//    - ai et bi (String) : renvoient les differents coeficients des operateurs (ai(x) et bi(t))      //                                                             //
//    - k (Entier) : codant le nombre de ports d'entrees du bloc EDP                                  // 
//    - mesures (vecteur des entiers) : renvoi la liste des points de mesures                         //
//----------------------------------------------------------------------------------------------------//

// create a figure
ok  = %f;
fin = %t;
// On desactive les volumes finis pour le moment car il y a un bug dans l'implémentation des conditions 
// aux limites, mais sera réglé dans la prochaine version.
list_methode = [gettext("Finite Diff.")  gettext("Finite Elemts.")];
methode=strcat(list_methode,"|");

list_discrimant = [gettext("positive") gettext("negative") gettext("null")];
discrimant=strcat(list_discrimant,"|");

list_points = params_pde.points;
points=strcat(list_points,"|");

f = figure("Position",[50 50 670 620], ..
    "figure_name", "PDE_GUI", ..
    "BackgroundColor",[0.7 0.9 0.4], ..
    "Tag", "PDE_GUI",..
    "userdata",[ok,fin]);
toolbar(f.figure_id,"off");

m4=uimenu("Parent", f, ..
    'label', 'Documentation', ..
    'callback', "help()");
m5=uimenu("Parent",f, ..
    'label', gettext("Quit"), ..
    'callback', "cb_IHM_EDP()", ..
    "tag","quitmenu");

// Titre
txt0 = uicontrol("Parent", f, ..
    "Position",[300 560 120 30], ..
    "Style","text",..
    "String",gettext("PDE block"),..
    "fontname", "Times Bold Italic", ..
    "fontsize",21, ..
    "BackgroundColor",[0.7 0.9 0.4]);
// Domaine		
frame1= uicontrol("Parent", f, ..
    "Position",[5 550 130 60], ..
    "Style","frame", ..
    "BackgroundColor",[0.9 0.9 0.9]);
txt1 = uicontrol("Parent", f, ..
    "Position",[20 600 50 10], ..
    "Style","text", ..
    "String",gettext("Domain"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
txt2 = uicontrol("Parent", f, ..
    "Position",[25 580 25 10], ..
    "Style","text", ..
    "String","a =", ..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
edita = uicontrol("Parent", f, ..
    "Position", [65 575 50 17.5], ..
    "Style","edit", ..
    "String",params_pde.a,..
    "BackgroundColor",[1 1 1],..
    "Tag", "edita");		
txt3 = uicontrol("Parent", f, ..
    "Position",[25 560 25 10],..
    "Style","text",..
    "String","b =",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editb = uicontrol("Parent", f, ..
    "Position", [65 555 50 17.5],..
    "Style","edit",..
    "String",params_pde.b,..
    "BackgroundColor",[1 1 1],..
    "tag","editb");

// Infos EDP		 
frame2= uicontrol("Parent", f, ..
    "Position",[5 240 450 300],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt4 = uicontrol("Parent", f, ..
    "Position",[20 530 120 10],..
    "Style","text", ...
    "String",gettext("Specification of the PDE"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);

//*********************
//expression
frame3= uicontrol("Parent", f, ..
    "Position",[10 332.5 440 195],..
    "Style","frame",..
    "BackgroundColor",[0.7 0.7 0.7]);
txt5 = uicontrol("Parent", f, ..
    "Position",[20 513 100 10],..
    "Style","text",..
    "String",gettext("Expression of the PDE :"),..
    "fontsize",8,..
    "BackgroundColor",[0.7 0.7 0.7]);
txt_exp = uicontrol("Parent", f, ..
    "Position",[15 502.5 430 10],..
    "Style","text",..
    "String", params_pde.txt_exp,..
    "fontsize",8,..
    "fontname","Times Bold Italic",..
    "BackgroundColor",[0.7 0.7 0.7],..
    "tag", "txt_exp");
txt_exp2 = uicontrol("Parent", f, ..
    "Position",[15 492.5 430 10],..
    "Style","text",..
    "String","",..
    "fontsize",8,..
    "fontname","Times Bold Italic",..
    "BackgroundColor",[0.7 0.7 0.7],..
    "tag", "txt_exp2");
check_op1 = uicontrol("Parent", f, ..
    "Position", [15 470 13 13], ..
    "Style", "checkbox",..
    "Value",params_pde.check_op1,..
    "BackgroundColor",[1 1 1],..
    "tag", "check_op1");
frame31= uicontrol("Parent", f, ..
    "Position",[40 457.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame311= uicontrol("Parent", f, ..
    "Position",[40 457.5 45 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt311 = uicontrol("Parent", f, ..
    "Position",[45 467.5 35 15],..
    "Style","text",..
    "String","d2u/dt2",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame312= uicontrol("Parent", f, ..
    "Position",[85 457.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt22 = uicontrol("Parent", f, ..
    "Position",[95 476 50 13],..
    "Style","text",..
    "String","a1(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita1 = uicontrol("Parent", f, ..
    "Position", [90 462 60 13],..
    "Style","edit",..
    "String",params_pde.a1,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita1");
frame313= uicontrol("Parent", f, ..
    "Position",[152.5 457.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt23 = uicontrol("Parent", f, ..
    "Position",[160 476 50 13],..
    "Style","text",..
    "String","b1(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb1 = uicontrol("Parent", f, ..
    "Position", [160 462 60 13],..
    "Style","edit",..
    "String",params_pde.b1,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "Tag","editb1");		

check_op2 = uicontrol("Parent", f, ..
    "Position", [15 430 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op2,..
    "BackgroundColor",[1 1 1], ..
    "tag", "check_op2"); 		
frame32= uicontrol("Parent", f,..
    "Position",[40 417.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame321= uicontrol("Parent", f, ..
    "Position",[40 417.5 45 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt321 = uicontrol("Parent", f, ..
    "Position",[45 427.5 35 15],..
    "Style","text",..
    "String","d2u/dx2",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame322= uicontrol("Parent", f, ..
    "Position",[85 417.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt24 = uicontrol("Parent", f, ..
    "Position",[95 436 50 13],..
    "Style","text",..
    "String","a2(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita2 = uicontrol("Parent", f, ..
    "Position"  , [90 422 60 13],..
    "Style","edit",..
    "String",params_pde.a2,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita2");
frame323= uicontrol("Parent", f, ..
    "Position",[152.5 417.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt26 = uicontrol("Parent", f, ..
    "Position",[160 436 50 13],..
    "Style","text",..
    "String","b2(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb2 = uicontrol("Parent", f, ..
    "Position"  , [160 422 60 13],..
    "Style","edit",..
    "String",params_pde.b2,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","editb2");		

check_op3 = uicontrol("Parent", f, ..
    "Position", [15 390 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op3,..
    "BackgroundColor",[1 1 1], ..
    "tag","check_op3"); 		
frame33= uicontrol("Parent", f, ..
    "Position",[40 377.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame331= uicontrol("Parent", f, ..
    "Position",[40 377.5 45 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt331 = uicontrol("Parent", f, ..
    "Position",[45 387.5 30 15],..
    "Style","text",..
    "String","du/dt",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame332= uicontrol("Parent", f, ..
    "Position",[85 377.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt27 = uicontrol("Parent", f, ..
    "Position",[95 396 50 13],..
    "Style","text",..
    "String","a3(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita3 = uicontrol("Parent", f, ..
    "Position"  , [90 382 60 13],..
    "Style","edit",..
    "String",params_pde.a3,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita3");
frame333= uicontrol("Parent", f, ..
    "Position",[152.5 377.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt28 = uicontrol("Parent", f, ..
    "Position",[160 396 50 13],..
    "Style","text",..
    "String","b3(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb3 = uicontrol("Parent", f, ..
    "Position"  , [160 382 60 13],..
    "Style","edit",..
    "String",params_pde.b3,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","editb3");		

check_op4 = uicontrol("Parent", f, ..
    "Position"  , [15 350 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op4,..
    "BackgroundColor",[1 1 1],..
    "tag","check_op4"); 		
frame34= uicontrol("Parent", f, ..
    "Position",[40 337.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame341= uicontrol("Parent", f, ..
    "Position",[40 337.5 45 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt341 = uicontrol("Parent", f, ..
    "Position",[42.5 347.5 37.5 15],..
    "Style","text",..
    "String","d2u/dtdx",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame342= uicontrol("Parent", f, ..
    "Position",[85 337.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt223 = uicontrol("Parent", f, ..
    "Position",[95 356 50 13],..
    "Style","text",..
    "String","a4(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita4 = uicontrol("Parent", f, ..
    "Position"  , [90 342 60 13],..
    "Style","edit",..
    "String",params_pde.a4,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita4");
frame343= uicontrol("Parent", f,..
    "Position",[152.5 337.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt224 = uicontrol("Parent", f, ..
    "Position",[160 356 50 13],..
    "Style","text",..
    "String","b4(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb4 = uicontrol("Parent", f, ..
    "Position"  , [160 342 60 13],..
    "Style","edit",..
    "String",params_pde.b4,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","editb4");				

check_op5 = uicontrol("Parent", f,..
    "Position"  , [240 470 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op5,..
    "BackgroundColor",[1 1 1],..
    "tag", "check_op5");
frame35= uicontrol("Parent", f, ..
    "Position",[265 457.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]); 
frame351= uicontrol("Parent", f, ..
    "Position",[265 457.5 40 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt351 = uicontrol("Parent", f, ..
    "Position",[270 467.5 30 15],..
    "Style","text",..
    "String","du/dx",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame352= uicontrol("Parent", f, ..
    "Position",[305 457.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt29 = uicontrol("Parent", f, ..
    "Position",[310 476 50 13],..
    "Style","text",..
    "String","a5(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita5 = uicontrol("Parent", f,..
    "Position"  , [310 462 60 13],..
    "Style","edit",..
    "String",params_pde.a5,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita5");
frame353= uicontrol("Parent", f, ..
    "Position",[372.5 457.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt210 = uicontrol("Parent", f, ..
    "Position",[380 476 50 13],..
    "Style","text",..
    "String","b5(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb5 = uicontrol("Parent", f, ..
    "Position"  , [380 462 60 13],..
    "Style","edit",..
    "String",params_pde.b5,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","editb5");		

check_op6 = uicontrol("Parent", f, ..
    "Position"  , [240 430 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op6,..
    "BackgroundColor",[1 1 1], ..
    "tag","check_op6"); 		
frame36= uicontrol("Parent", f, ..
    "Position",[265 417.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame361= uicontrol("Parent", f, ..
    "Position",[265 417.5 40 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt361 = uicontrol("Parent", f, ..
    "Position",[270 427.5 30 15],..
    "Style","text",..
    "String","u","fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame362= uicontrol("Parent", f, ..
    "Position",[305 417.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt211 = uicontrol("Parent", f, ..
    "Position",[310 436 50 13],..
    "Style","text",..
    "String","a6(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita6 = uicontrol("Parent", f, ..
    "Position"  , [310 422 60 13],..
    "Style","edit",..
    "String",params_pde.a6,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag", "edita6");
frame363= uicontrol("Parent", f,..
    "Position",[372.5 417.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt222 = uicontrol("Parent", f, ..
    "Position",[380 436 50 13],..
    "Style","text",..
    "String","b6(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb6 = uicontrol("Parent", f, ..
    "Position"  , [380 422 60 13],..
    "Style","edit",..
    "String",params_pde.b6,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag", "editb6");		

check_op7 = uicontrol("Parent", f, ..
    "Position"  , [240 390 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.check_op7,..
    "BackgroundColor",[1 1 1],..
    "tag","check_op7"); 		
frame37= uicontrol("Parent", f, ..
    "Position",[265 377.5 180 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
frame371= uicontrol("Parent", f, ..
    "Position",[265 377.5 40 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt371 = uicontrol("Parent", f, ..
    "Position",[270 387.5 30 15],..
    "Style","text",..
    "String","f",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
frame372= uicontrol("Parent", f, ..
    "Position",[305 377.5 70 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt223 = uicontrol("Parent", f, ..
    "Position",[310 396 50 13],..
    "Style","text",..
    "String","a7(x)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
edita7 = uicontrol("Parent", f, ..
    "Position"  , [310 382 60 13],..
    "Style","edit",..
    "String",params_pde.a7,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","edita7");
frame373= uicontrol("Parent", f, ..
    "Position",[372.5 377.5 72.5 35],..
    "Style","frame",..
    "BackgroundColor",[0.8 0.8 0.8]);
txt224 = uicontrol("Parent", f, ..
    "Position",[380 396 50 13],..
    "Style","text",..
    "String","b7(t)",..
    "fontsize",8,..
    "BackgroundColor",[0.8 0.8 0.8]);
editb7 = uicontrol("Parent", f, ..
    "Position"  , [380 382 60 13],..
    "Style","edit",..
    "String",params_pde.b7,..
    "fontsize",8,..
    "BackgroundColor",[1 1 1],..
    "tag","editb7");		

b1 = uicontrol("Parent", f, ..
    "Position"  , [270 345 130 20],..
    "Style", "pushbutton",..
    "String", gettext("Visualize the expression"),..
    "fontname","Times Bold Italic",..
    "fontsize",8,..
    "callback"  , "cb_IHM_EDP()",..
    "BackgroundColor",[0 0.7 1],..
    "tag", "visubutton");

// discriminant :
txt5 = uicontrol("Parent", f, ..
    "Position",[25 310 120 15],..
    "Style","text",..
    "String",gettext("Type of the discrimant :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
check1 = uicontrol("Parent", f,..
    "Position"  , [155 315 13 13],..
    "Style","checkbox",..
    "Value",params_pde.discr_cst,..
    "BackgroundColor",[1 1 1],..
    "callback", "cb_IHM_EDP()",..
    "tag","check1");
txt51 = uicontrol("Parent", f, ..
    "Position",[170 315 110 15],..
    "Style","text",..
    "String",gettext("constant discriminant"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);		
check2 = uicontrol("Parent", f,..
    "Position"  , [155 295 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.discr_non_cst,..
    "BackgroundColor",[1 1 1],..
    "callback", "cb_IHM_EDP()",..
    "tag","check2");
txt53 = uicontrol("Parent", f, ..
    "Position",[170 295 140 15],..
    "Style","text",..
    "String",gettext("non constant discriminant :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);		
liste_signe = uicontrol("Parent", f,..
    "Position"  , [320 245 80 70],..
    "Style","listbox", ..
    "String",discrimant,..
    "Value",params_pde.signe,..
    "BackgroundColor",[1 1 1],..
    "tag", "liste_signe");

//la methode
// Choix
frame5= uicontrol("Parent", f,..
    "Position",[5 130 450 100],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt81 = uicontrol("Parent", f,..
    "Position",[25 220 180 10],..
    "Style","text",..
    "String",gettext("Spatial discretization method"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
txt811 = uicontrol("Parent", f, ..
    "Position",[15 200 40 15],..
    "Style","text",..
    "String",gettext("Choice :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);	

rad_automatique = uicontrol("Parent", f, ..
    "Position", [15 175 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.rad_automatique,..
    "BackgroundColor",[1 1 1],..
    "callback", "cb_IHM_EDP()",..
    "tag","rad_automatique");
txt51 = uicontrol("Parent", f, ..
    "Position",[25 175 70 15],..
    "Style","text",..
    "String",gettext("Automatic"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);		
rad_manuel = uicontrol("Parent", f, ..
    "Position"  , [15 155 13 13],..
    "Style", "checkbox",..
    "Value",params_pde.rad_manuel,..
    "BackgroundColor",[1 1 1],..
    "callback", "cb_IHM_EDP()",..
    "tag", "rad_manuel");
txt54 = uicontrol("Parent", f,..
    "Position",[25 155 45 15],..
    "Style","text",..
    "String",gettext("Manual"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);		
// Type		
txt82 = uicontrol("Parent", f,..
    "Position",[105 200 30 15],..
    "Style","text",..
    "String","Type :",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
liste_meth = uicontrol("Parent", f, ..
    "Position"  , [145 140 80 70],..
    "Style","listbox",..
    "String",methode,..
    "Value",params_pde.methode,..
    "BackgroundColor",[1 1 1],..
    "tag", "liste_meth"); 	
// Ordonnancement		 
txt83 = uicontrol("Parent", f,..
    "Position",[230 210 100 15],..
    "Style","text",..
    "String",gettext("Scheduling :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editordre1 = uicontrol("Parent", f, ..
    "Position"  , [270 192.5 15 15],..
    "Style","edit",..
    "String",params_pde.ord1,..
    "BackgroundColor",[1 1 1],..
    "tag","editordre1");		
editordre2 = uicontrol("Parent", f,..
    "Position"  , [270 175 15 15],..
    "Style","edit",..
    "String",params_pde.ord2,..
    "BackgroundColor",[1 1 1],..
    "tag","editordre2");		
editordre3 = uicontrol("Parent", f,..
    "Position"  , [270 157.5 15 15],..
    "Style","edit",..
    "String",params_pde.ord3,..
    "BackgroundColor",[1 1 1],..
    "tag","editordre3");		
// Degre		 
txt84 = uicontrol("Parent", f, ..
    "Position",[320 200 80 15],..
    "Style","text",..
    "String",gettext("degree :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editdegre = uicontrol("Parent", f,..
    "Position"  , [400 200 15 15],..
    "Style","edit",..
    "String",params_pde.degre,..
    "BackgroundColor",[1 1 1],..
    "tag","editdegre");				
// Pas de maillage		 
txt6 = uicontrol("Parent", f, ..
    "Position",[310 180 130 15],..
    "Style","text",..
    "String",gettext("Number of nodes :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
//affichage du pas de maillage 
editpas = uicontrol("Parent", f,..
    "Position"  , [360 160 50 17],..
    "Style","edit",..
    "String",params_pde.nnode,..
    "BackgroundColor",[1 1 1],..
    "tag","editpas");	
bb = uicontrol("Parent", f,..
    "Position"  , [290 135 80 20],..
    "Style", "pushbutton",..
    "String", gettext("Display the step"),..
    "fontname","Times Bold Italic",..
    "fontsize",8,..
    "callback"  , "cb_IHM_EDP()",..
    "BackgroundColor",[0 0.7 1],..
    "tag","bb"); 

txt_pas = uicontrol("Parent", f,..
    "Position",[370 140 80 10],..
    "Style","text",..
    "String", params_pde.txt_pas,..
    "fontsize",8,..
    "fontname","Times Bold Italic",..
    "BackgroundColor",[0.9 0.9 0.9],..
    "tag","txt_pas");

// Conditions Initiales		 
frame6= uicontrol("Parent", f,..
    "Position",[5 80 450 40],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt8 = uicontrol("Parent", f, ..
    "Position",[25 110 100 10],..
    "Style","text",..
    "String",gettext("Initial conditions"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
txt9 = uicontrol("Parent", f, ..
    "Position",[20 90 50 15],..
    "Style","text",..
    "String","u(x,t0) =",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editCI = uicontrol("Parent", f, ..
    "Position"  , [80 90 120 17],..
    "Style","edit",..
    "String",params_pde.CI,..
    "fontname","Times  Bold Italic",..
    "BackgroundColor",[1 1 1],..
    "tag","editCI");	
txt91 = uicontrol("Parent", f, ..
    "Position",[250 90 70 15],..
    "Style","text",..
    "String","du/dt|t0 =",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editCI1 = uicontrol("Parent", f, ..
    "Position"  , [330 90 100 17],..
    "Style","edit",..
    "String",params_pde.dCI,..
    "fontname","Times  Bold Italic",..
    "BackgroundColor",[1 1 1],..
    "tag","editCI1");	
// Conditions aux limites
frame7= uicontrol("Parent", f, ..
    "Position",[5 5 210 70],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt10 = uicontrol("Parent", f, ..
    "Position",[25 65 100 10],..
    "Style","text",..
    "String",gettext("Limit condition in a"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
frame8= uicontrol("Parent", f,..
    "Position",[245 5 210 70],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt11 = uicontrol("Parent", f, ..
    "Position",[270 65 100 10],..
    "Style","text",..
    "String",gettext("Limit condition in b"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
txt8111 = uicontrol("Parent", f, ..
    "Position",[15 45 40 15],..
    "Style","text",..
    "String","Type :",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);	
popa = uicontrol("Parent", f, ..
    "Position"  , [80 40 60 20],..
    "Style", "popupmenu",..
    "String", "Dirichlet|Neumann",..
    "Value",params_pde.CLa,..
    "BackgroundColor",[1 1 0],..
    "tag","popa");
txt81112 = uicontrol("Parent", f, ..
    "Position",[255 45 40 15],..
    "Style","text",..
    "String",gettext("Type :"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);	
popb = uicontrol("Parent", f, ..
    "Position"  , [330 40 60 20],..
    "Style", "popupmenu",..
    "String", "Dirichlet|Neumann",..
    "Value",params_pde.CLb,..
    "BackgroundColor",[1 1 0],..
    "tag","popb");
txt12 = uicontrol("Parent", f, ..
    "Position",[20 10 70 15],..
    "Style","text",..
    "String",gettext("Expression ="),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editCLa = uicontrol("Parent", f, ..
    "Position"  , [100 10 90 20],..
    "Style","edit",..
    "String",params_pde.CLa_exp,..
    "BackgroundColor",[1 1 1],..
    "tag","editCLa");		
txt13 = uicontrol("Parent", f, ..
    "Position",[265 10 70 15],..
    "Style","text",..
    "String",gettext("Expression ="),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);
editCLb = uicontrol("Parent", f, ..
    "Position"  , [340 10 90 20],..
    "Style","edit",..
    "String",params_pde.CLb_exp,..
    "BackgroundColor",[1 1 1],..
    "tag","edtCLb");


// points de sortie :
frame9= uicontrol("Parent", f,..
    "Position",[465 240 200 300],..
    "Style","frame",..
    "BackgroundColor",[0.9 0.9 0.9]);
txt14 = uicontrol("Parent", f, ..
    "Position",[485 525 100 15],..
    "Style","text",..
    "String",gettext("Measurement points"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);		
txt141 = uicontrol("Parent", f, ..
    "Position",[470 500 30 15],..
    "Style","text",..
    "String","u(xi) :",..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);	
txt142 = uicontrol("Parent", f, ..
    "Position",[500 500 160 15],..
    "Style","text",..
    "String",gettext("measurement points"),..
    "fontsize",10,..
    "BackgroundColor",[0.9 0.9 0.9]);	
liste_pts = uicontrol("Parent", f, ..
    "Position"  , [530 330 100 150], ..
    "Style", "listbox", ..
    "String" ,points,..
    "BackgroundColor",[1 1 1],..
    "tag","liste_pts");
edit_pt = uicontrol("Parent", f,..
    "Position"  , [540 290 70 20],..
    "Style", "edit",..
    "String","",..
    "BackgroundColor",[1 1 1],..
    "tag","edit_pt");
b2 = uicontrol("Parent", f, ..
    "Position"  , [490 255 70 20],..
    "Style","pushbutton",..
    "String",gettext("add"),..
    "callback"  , "cb_IHM_EDP()" ,..
    "BackgroundColor",[0 1 0],..
    "tag","b2");  	
b5 = uicontrol("Parent", f, ..
    "Position"  , [580 255 70 20],..
    "Style","pushbutton",..
    "String",gettext("remove"),..
    "callback"  , "cb_IHM_EDP()" ,..
    "BackgroundColor",[0 0.7 1],..
    "tag","b5");
b3 = uicontrol("Parent", f, ..
    "Position"  , [500 90 50 20],..
    "Style", "pushbutton",..
    "String", gettext("Validate"),..
    "callback"  , "cb_IHM_EDP()",..
    "BackgroundColor",[0 1 0],..
    "tag","b3");
b4 = uicontrol("Parent", f, ..
    "Position", [610 90 50 20], ..
    "Style", "pushbutton", ..
    "String", gettext("Cancel"), ..
    "callback", "cb_IHM_EDP()", ..
    "BackgroundColor",[1 0 0], ..
    "tag","cancelbutton");  		 

ud = get(findobj("tag","PDE_GUI"), "userdata")
while ~isempty(findobj("tag","PDE_GUI")) & ud(2)
  if isempty(findobj("tag","PDE_GUI")) then
    break
  end
  ud = get(findobj("tag","PDE_GUI"), "userdata")
end

if ud(1) then
  close(f)
  a_domaine=[];b_domaine=[];discr=[];signe=[];choix=[];type_meth=[];degre=[];Nbr_maillage=[];
  CI=[];CI1=[];CLa_type=[];CLa_exp=[];CLb_type=[];CLb_exp=[];oper=[];a1=[];b1=[];a2=[];b2=[];
  a3=[];b3=[];a4=[];b4=[];a5=[];b5=[];a6=[];b6=[];a7=[];b7=[];k=[];mesures=[];params_pde=[];
  return; 
end
ok=ud(2);  

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

params_pde.check_op1=string(get(findobj("tag","check_op1"),"Value"));
params_pde.a1=get(findobj("tag","edita1"),"String");
params_pde.b1=get(findobj("tag","editb1"),"String");

params_pde.check_op2=string(get(findobj("tag","check_op2"),"Value"));
params_pde.a2=get(findobj("tag","edita2"),"String");
params_pde.b2=get(findobj("tag","editb2"),"String");

params_pde.check_op3=string(get(findobj("tag","check_op3"),"Value"));
params_pde.a3=get(findobj("tag","edita3"),"String");
params_pde.b3=get(findobj("tag","editb3"),"String");

params_pde.check_op4=string(get(findobj("tag","check_op4"),"Value"));
params_pde.a4=get(findobj("tag","edita4"),"String");
params_pde.b4=get(findobj("tag","editb4"),"String");

params_pde.check_op5=string(get(findobj("tag","check_op5"),"Value"));
params_pde.a5=get(findobj("tag","edita5"),"String");
params_pde.b5=get(findobj("tag","editb5"),"String");

params_pde.check_op6=string(get(findobj("tag","check_op6"),"Value"));
params_pde.a6=get(findobj("tag","edita6"),"String");
params_pde.b6=get(findobj("tag","editb6"),"String");

params_pde.check_op7=string(get(findobj("tag","check_op7"),"Value"));
params_pde.a7=get(findobj("tag","edita7"),"String");
params_pde.b7=get(findobj("tag","editb7"),"String");

params_pde.discr_cst=string(get(findobj("tag","check1"),"Value"));
params_pde.discr_non_cst=string(get(findobj("tag","check2"),"Value"));
params_pde.signe=string(signe);
params_pde.rad_automatique=string(get(findobj("tag","rad_automatique"),"Value"));
params_pde.rad_manuel=string(get(findobj("tag","rad_manuel"),"Value"));
params_pde.methode=string(type_meth);
params_pde.txt_pas=get(findobj("tag","txt_pas"),"String");
params_pde.ord1=get(findobj("tag","editordre1"),"String");
params_pde.ord2=get(findobj("tag","editordre2"),"String");
params_pde.ord3=get(findobj("tag","editordre3"),"String");
params_pde.degre=string(degre);
params_pde.nnode=string(Nbr_maillage);
params_pde.txt_exp=get(findobj("tag","txt_exp"),"String");
params_pde.CI=CI;
params_pde.dCI=CI1;
params_pde.CLa=string(CLa_type);
params_pde.CLa_exp=get(findobj("tag","editCLa"),"String");
params_pde.CLb=string(CLb_type);
params_pde.CLb_exp=get(findobj("tag","editCLb"),"String");
params_pde.points=list_points;
// fermeure du gui  
close(f)

endfunction
