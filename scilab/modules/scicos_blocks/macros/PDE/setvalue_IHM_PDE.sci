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
    a6,b6,a7,b7,k,mesures,params_pde]=setvalue_IHM_PDE(params_pde)
    // Cette fonction permet d'evaluer les parametres de l'IHM suite au l'evaluation du context           //
    // Entree :                                                                                           //
    //    - params_pde (tlist) : rajouter a la list exprs du bloc EDP afin de sauvegarder les             //
    //                           informations de l'IHM. C'est une aussi variable de sortie.               //                                                    //
    // sorties :                                                                                          //
    //    - ok (booleen) : variable de gestion des erreurs et la sortie de la fonction.                   //
    //    - a_domaine et b_domaine (Entiers) : sont les bords du domaine [a b]                            //
    //    - discr (Entier) : renvoie le type du disciminant (0: consatnt ou 1: non constant)               //
    //    - signe (Entier) : renvoie le signe du discriminant dans les cas non constant                    //
    //              (1: positif, 2: n�gatif, 3: nul )                                                     //
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

    ok=%f;
    a_domaine=[];
    b_domaine=[];
    choix=0;
    signe=1;
    discr=0;
    type_meth=1;
    degre=1;
    Nbr_maillage=10;
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
    CI=[];
    CI1=[];
    CLa_type=[];
    CLa_exp=[];
    CLb_type=[];
    CLb_exp=[];
    oper=[];
    k=[];
    // evaluation du context
    %mm=getfield(1,%scicos_context)
    for %mi=%mm(3:$)
        if execstr(%mi+"=%scicos_context(%mi)","errcatch")<>0 then
            disp(lasterror())
            ok=%t
            return
        end
    end
    // mise a jour des differents informations de l'IHM
    a_domaine = evstr(params_pde.a);
    b_domaine = evstr(params_pde.b);
    choixman=evstr(params_pde.rad_manuel);
    if (choixman) then
        // Choix (0 : Automatique, 1 : Manuel)
        choix =1;
    end
    signe=params_pde.signe;
    if (evstr(params_pde.discr_non_cst)) then
        discr=1;
    end
    type_meth=evstr(params_pde.methode);
    degre = evstr(params_pde.degre);
    Nbr_maillage=evstr(params_pde.nnode);
    execstr("a1=params_pde.a1");
    execstr("b1=params_pde.b1");
    execstr("a2=params_pde.a2");
    execstr("b2=params_pde.b2");
    execstr("a3=params_pde.a3");
    execstr("b3=params_pde.b3");
    execstr("a4=params_pde.a4");
    execstr("b4=params_pde.b4");
    execstr("a5=params_pde.a5");
    execstr("b5=params_pde.b5");
    execstr("a6=params_pde.a6");
    execstr("b6=params_pde.b6");
    execstr("a7=params_pde.a7");
    execstr("b7=params_pde.b7");
    mesures = evstr(params_pde.points);
    execstr("CI=params_pde.CI");
    execstr("CI1=params_pde.dCI");
    CLa_type=evstr(params_pde.CLa);
    execstr("CLa_exp=params_pde.CLa_exp");
    CLb_type=evstr(params_pde.CLb);
    execstr("CLb_exp=params_pde.CLb_exp");
    k=0;
    if (params_pde.check_op1 == "1") then
        oper=[oper;1];
        if (stripblanks(b1) == "IN_EDP1(t)") then
            b1="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op2 == "1") then
        oper=[oper;2];
        if (stripblanks(b2) == "IN_EDP2(t)") then
            b2="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op3 == "1") then
        oper=[oper;3];
        if (stripblanks(b3) == "IN_EDP3(t)") then
            b3="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op4 == "1") then
        oper=[oper;4];
        if (stripblanks(b4) == "IN_EDP4(t)") then
            b4="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op5 == "1") then
        oper=[oper;5];
        if (stripblanks(b5) == "IN_EDP5(t)") then
            b5="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op6 == "1") then
        oper=[oper;6];
        if (stripblanks(b6) == "IN_EDP6(t)") then
            b6="inptr["+string(k)+"][0]";
            k=k+1;
        end
    end
    if (params_pde.check_op7 == "1") then
        oper=[oper;7];
        if (stripblanks(b7) == "IN_EDP7(t)") then
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

endfunction
