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

function [a_domaine,b_domaine,discr,signe,choix,type_meth,degre,Nbr_maillage,CI,CI1,CLa_type,CLa_exp,..
    CLb_type,CLb_exp,oper,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,mesures,k]=extraction_infos(edita,..
    editb,check1,check2,rad_automatique,rad_manuel,liste_signe,liste_meth,editdegre,editpas,editCI,editCI1,..
    popa,editCLa,popb,editCLb,check_op1,edita1,editb1,check_op2,edita2,editb2,check_op3,edita3,..
    editb3,check_op4,edita4,editb4,check_op5,edita5,editb5,check_op6,edita6,editb6,check_op7,edita7,..
    editb7,list_points)
    // Cette fonction permet d'extraire les information renseignes dans l'IHM                                 //
    // Entrees :                                                                                              //
    //          - edita, editb : les handl des zones edit resp du bord a et b                                 //
    //          - check1,check2 : les handl des checkbox correspondant aux choix du discriminant              //
    //          - rad_automatique,rad_manuel : les handl des radiobuttom correspondant aux mode man ou auto   //
    //          - liste_signe,liste_meth : handls des listebox correspondent resp au signe du discriminant    //
    //                                     non constant et au type de la methode de discritisation            //
    //          - editdegre, editpas : handls des zones edit correspondent respetivement au degre de la       //
    //                                methode de discretisation et au nombre de neuds                         //
    //          - editCI,editCI1 : handls des zones edit correspondent respetivement aux conditions initiales //
    //          - popa,popb: handls des popmenu correspondent aux choix des types des conditions aux limittes //
    //                       respectivement en a et en b                                                      //
    //          - editCLa,editCLb : handls des zones edit correspondent respetivement aux expression des      //
    //                              conditions aux limittes                                                   //
    //          - check_opi,editai,editbi : handls respectivement du checkbox d'activation d'un operateur,    //
    //                                      de la zone edit de l'expression du ai(x) et de la zone edit de    //
    //                                      l'expression du bi(x), o� i=1,2,...7 ( 1 : d2u/dt2, 2 : d2u/dx2,  //
    //                                      3 : du/dt, 4 : d2u/dtdx, 5 : du/dx, 6 : u, 7 : f)                 //
    //          - list_points : vecteur string contient la liste des points de mesure                         //
    // Sorties :                                                                                              //
    //          - a_domaine, b_domaine (doubles) : correspondant resp aux valeurs des bords du domaine a et b //
    //          - discr (entier) : =0 si le discriminant de l'EDP est constant, =1 sinon                      //
    //          - signe (entier) : signe du discriminant non constant(1: positif, 2: n�gatif, 3: nul)         //
    //          - choix (entier) : mode (0 : syst�me expert, 1 : Manuel)                                      //
    //          - type_meth (entier) : type de la methode de discretisation (type_meth=1 : DF, 2 : EF, 3 : VF)//
    //          - degre (entier) : le degre de la methode de discretisation)                                  //
    //          - Nbr_maillage (entier) : nombre de noeuds ave les noeuds aux limmites                        //
    //          - CI, CI1(String) : expressions des conditions initiales resp u(t0,x) et du/dt|t=0            //
    //          - CLa_type, CLb_type(entiers) : types des conditions aux limites (0 : Dirichlet, 1 : Neumann) //
    //          - CLb_exp, CLa_exp (String) :  expressions des conditions aux limites resp en a et en b       //
    //          - oper (vecteur des entiers) : code les op�rateurs selectionnes de 1 � 7                      //
    //          - ai, bi (String) : avec i=1:7 : expressions des coefficients des differents operateurs       //
    //          - mesures (vecteur des doubles) : renvoie la liste des points de mesures                       //
    //          - k (entier) : renvoie le nombre de port d'entree regulier du bloc EDP                        //
    //--------------------------------------------------------------------------------------------------------//

    // domaine
    choix=0;
    signe=1;
    discr=0;
    type_meth=1;
    degre=1;
    Nbr_maillage = 10;
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
    mesures=[];
    a_domaine=evstr(get(edita,"String"));
    b_domaine=evstr(get(editb,"String"));
    //discriminant (O: discriminant constant, 1: discriminant non constant )
    discr_cst=get(check1,"Value");
    discr_non_cst=get(check2,"Value");
    //signe du discriminant non constant(1: positif, 2: negatif, 3: nul )
    if (discr_non_cst) then
        discr=1;
        signe=get(liste_signe,"Value");
    end
    // la methode de discretisation spatiale

    choixaut=get(rad_automatique,"Value");
    choixman=get(rad_manuel,"Value");
    if (choixman) then
        // Choix (0 : Automatique, 1 : Manuel)
        choix =1;
        // type de la methode de discretisation
        // type_meth=1 : differences finies, 2 : elements finis, 3 : volumes finis
        type_meth =get(liste_meth,"Value");
        // degre (entier = 1 , 2 , 3)
        degre=evstr(get(editdegre,"String"));
        // Nombre de points (noeuds) Nbr_maillage = 1 : inf (dans R+)
        Nbr_maillage = evstr(get(editpas,"String"));
    end
    // l'expression des conditions initiales ( chaine de caracteres)
    CI = get(editCI,"String");
    CI1 = get(editCI1,"String");
    //type de la condition au limite en a (0 : Dirichlet, 1 : Neumann)
    CLa_type = get(popa,"Value")-1; //S.S. : -1 to patch the 4667 bug
    // l'expression de la condition au limite en a (une chaine de caracteres)
    CLa_exp = get(editCLa,"String");
    //type de la condition au limite en b (0 : Dirichlet, 1 : Neumann)
    CLb_type = get(popb,"Value")-1; //S.S. : -1 to patch the 4667 bug
    // l'expression de la condition au limite en b (une chaine de caracteres)
    CLb_exp = get(editCLb,"String");
    // les operateurs
    // d2u/dt2 (oper =1), d2u/dx2 (oper =2), du/dt (oper =3), d2u/dtdx (oper =4)
    //du/dx (oper =5), u (oper =6), f (oper =7)
    k=0;
    if (get(check_op1,"Value")== 1) then
        oper=[oper;1];
        a1=stripblanks(get(edita1,"String"));
        b1=stripblanks(get(editb1,"String"));
        if (b1 == "IN_EDP1(t)") then
            b1="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op2,"Value")== 1) then
        oper=[oper;2];
        a2=stripblanks(get(edita2,"String"));
        b2=stripblanks(get(editb2,"String"));
        if (b2 == "IN_EDP2(t)") then
            b2="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op3,"Value")== 1) then
        oper=[oper;3];
        a3=stripblanks(get(edita3,"String"));
        b3=stripblanks(get(editb3,"String"));
        if (b3 == "IN_EDP3(t)") then
            b3="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op4,"Value")== 1) then
        oper=[oper;4];
        a4=stripblanks(get(edita4,"String"));
        b4=stripblanks(get(editb4,"String"));
        if (b4 == "IN_EDP4(t)") then
            b4="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op5,"Value")== 1) then
        oper=[oper;5];
        a5=stripblanks(get(edita5,"String"));
        b5=stripblanks(get(editb5,"String"));
        if (b5 == "IN_EDP5(t)") then
            b5="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op6,"Value")== 1) then
        oper=[oper;6];
        a6=stripblanks(get(edita6,"String"));
        b6=stripblanks(get(editb6,"String"));
        if (b6 == "IN_EDP6(t)") then
            b6="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (get(check_op7,"Value")== 1) then
        oper=[oper;7];
        a7=stripblanks(get(edita7,"String"));
        b7=stripblanks(get(editb7,"String"));
        if (b7 == "IN_EDP7(t)") then
            b7="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (CLa_exp  == "IN_CL1(t)") then
        CLa_exp ="inptr["+string(k)+"][0]";
        k=k+1;
    end
    if (CLb_exp  == "IN_CL2(t)") then
        CLb_exp ="inptr["+string(k)+"][0]";
        k=k+1;
    end

    //les points des mesures
    mesures=evstr(list_points);

endfunction
