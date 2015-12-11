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

function [flag_type,rdnom,DF_type,Code]=translate(CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,..
    type_meth,degre,a,b,N,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,nom,mesures)
    // Cette fonction contient les différents algrithme de discretisation spaciale, ainnsi que la        //
    // génération du code du bloc EDP. Elle est appelée par la fonction graphique du bloc EDP.Sci          //
    // Sorties:                                                                                          //
    //    - flag_type (Entier) : renvoie le type des équations générées, ( 1 pour l'explicite,            //
    //      2 pour l'implicite)                                                                          //
    //    - rdnom (String) : renvoie le nom du bloc plus "_explicite" si le bloc est explicite,          //
    //      "_implicite" si le bloc est implicite                                                        //
    //    - DF_type (Entier) : 0 pour les differences finies centrees, 1 pour les decentrees a gauche    //
    //      et 2 pour les decentrees à droite                                                            //
    //    - Code (String) : vecteur qui contient le code C du bloc                                       //
    // Entrées:                                                                                          //
    //    - CI, CI1(String) : expressions des conditions initiales resp u(t0,x) et du/dt|t=0             //
    //    - CLa_type, CLb_type(entiers) : types des conditions aux limites (0 : Dirichlet, 1 : Neumann)  //
    //    - CLb_exp, CLa_exp (String) :  expressions des conditions aux limites resp en a et en b        //
    //    - oper (vecteur des entiers) : code les opérateurs selectionnes de 1 à 7                       //
    //    - type_meth (entier) : type de la methode de discretisation (type_meth=1 : DF, 2 : EF, 3 : VF) //
    //    - degre (entier) : le degre de la methode de discretisation)                                   //
    //    - a, b (doubles) : correspondant resp aux valeurs des bords du domaine a et b                  //
    //    - N (entier) : nombre de noeuds ave les noeuds aux limmites                                    //
    //    - ai, bi (String) : avec i=1:7 : expressions des coefficients des differents operateurs        //
    //    - nom (String) : correspond au nom du bloc a generer choisis par l'utilisateur dans la fenêtre //
    //      SCILAB "GIVE US BLOCK's NAME"                                                                //
    //    - mesures (vecteur des doubles) : renvoie la liste des points de mesures                        //
    //---------------------------------------------------------------------------------------------------//
    DF_type=[];
    // fonction principale

    // Conditions aux limites
    // kbc est le vecteur type
    kbc(1)=CLa_type;
    kbc(2)=CLb_type;
    // vbc est le vecteur des valeurs
    vbc(1)=CLa_exp;
    vbc(2)=CLb_exp;

    //h=(b_domaine-a_domaine)/Nbr_maillage;
    vh=linspace(a,b,N);
    h=vh(2)-vh(1);
    eq_pts_mes=[];
    Nfictif=N;
    // FEM
    timer();
    if (type_meth == 2) then
        //dans les éléments finis on génère de l'implicite
        flag_type=2; // 1 pour les systèmes explicites, 2 pour l'implicite

        nnode=N;
        [xi,w] = setint(); // Get Gaussian points and weights.

        [x,nelem,nodes,kind,nint]=maillage_FE1D(a,b,degre,nnode,..
        CLa_type,CLa_exp,CLb_type,CLb_exp); //maillage

        // calcul de A,B,C et F
        [A,B1,B2,C1,C2,C3,F3]=coef_FEM1d(oper,nelem,kind,nint,nodes,x,xi,w,..
        nnode,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,kbc,vbc);

        // intepolation de la solution aux points de mesures par le polynomme de Lagrange
        [eq_pts_mes]=eval_pts_EF(a,b,nelem,kind,nint,nodes,x,xi,w,nnode,mesures);

        // génération des équations

        [equations,impl_type]=gen_code_FEM(A,B1,B2,C1,C2,C3,F3,oper,N,a,b,..
        b1,b2,b3,b4,b5,b6,b7,vbc,kbc);
        mprintf(_("The finite element discretization took %.2f seconds"),timer());
        // FDM
    elseif (type_meth == 1) then
        //dans les différences finies on génère de l'implicite
        flag_type=2; // 1 : explicie, 2 : implicite

        //centré ou decentré qui viendera du système expert après
        if (degre == 2) then
            DF_type=0; //centré
        elseif (degre == 1)
            DF_type=1; //decentré
        end

        // intercalage de la solution aux points de mesures aux points les plus proches
        [eq_pts_mes]=eval_pts_df(a,h,N,mesures);

        // génération des équations
        [equations,impl_type,Nfictif]=gen_code_FDM(a1,b1,a2,b2,a3,b3,a4,b4,..
        a5,b5,a6,b6,a7,b7,a,b,N,oper,vbc,kbc,DF_type,h)
        mprintf(_("The finite differences discretization took %.2f seconds"),timer());

        // FVM
    else
        // maillage
        [xc,xn]=unimesh1D(N,a,b);

        // intercalage de la solution aux points de mesures aux points les plus proches
        [eq_pts_mes]=eval_pts_vf(a,h,N,mesures);

        // génération des équations

        [equations,flag_type,impl_type]=gen_code_FVM(a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,..
        a7,b7,N,oper,vbc,xn,xc);
        mprintf(_("The finite volume discretization took %.2f seconds"),timer());
    end
    // Assemblage de code
    if (flag_type == 1) then
        rdnom = nom+"_explicite";
    else
        rdnom = nom+"_implicite";
    end

    Code=code_generation(rdnom,equations,eq_pts_mes,flag_type,h,CI,CI1,a, ...
    Nfictif,N,impl_type,type_meth,oper);
    mprintf(_("The code generation, the compilation and link took %.2f seconds"),timer());

endfunction

