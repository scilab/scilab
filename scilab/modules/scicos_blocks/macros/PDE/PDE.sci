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

function [x,y,typ]=PDE(job,arg1,arg2)
    // développé par EADS-CCR
    // fonction graphique du bloc, elle permet le dessin et l'initialisation du bloc                //
    // Reference: "Scicos user guid", http://www.scicos.org                                       //
    //--------------------------------------------------------------------------------------------//

    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        label=graphics.exprs
        model=arg1.model;
        params_pde=label(1);

        while %t do
            [ln,fun]=where();
            if ~or(fun == "do_eval") then // cas standard
                [ok,a_domaine,b_domaine,discr,signe,choix,type_meth,degre,Nbr_maillage,..
                CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,..
                a6,b6,a7,b7,k,mesures,params_pde]=IHM_EDP(params_pde);
                if ok then
                    return;
                end
            else
                if exists("%scicos_context") then
                    // evaluation du context
                    [ok,a_domaine,b_domaine,discr,signe,choix,type_meth,degre,Nbr_maillage,..
                    CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,..
                    a6,b6,a7,b7,k,mesures,params_pde]=setvalue_IHM_EDP(params_pde);
                end
            end

            //**********************************
            // Get the name of the file
            //***********************************
            okk=%f;
            rdnom="PDE";
            ok1=%t;
            while %t do
                [okk,rdnom,lab]=scicos_getvalue("PLEASE, GIVE US THE BLOCK''s NAME. ",..
                "New block''s name: ",list("str",1),label(3));

                if okk==%f then
                    ok1=%f;
                    return;
                end
                label(3)=lab;
                rdnom=stripblanks(rdnom);
                if rdnom==emptystr() then
                    ok1=%f;messagebox("Sorry, C file name not defined.");
                end
                if ok1 then
                    break,
                end
            end

            // arbre de decision
            if (choix == 0) then
                // Choix automatique
                ind4=strindex(a4,"x");
                ind1=strindex(a1,"x");
                ind2=strindex(a2,"x");
                if (ind4~=[] | ind1~=[] | ind2~=[]) then
                    if (signe == 1) then,
                        delta=1;
                    elseif (signe == 2) then,
                        delta=-1;
                    elseif (signe == 0) then,
                        delta=0;
                    else
                        messagebox(["The discriminant isn''t constant,"; "You must choose its sign in the GUI"]);
                        return;
                    end
                else
                    delta=evstr(a4)^2-4*evstr(a1)*evstr(a2);
                end
                if (delta==[]) then,
                    delta=0;
                end
                type_meth=arbre_decision(delta);
            end
            // a voir si c'est à rajouter pour ne pas regenerer dans le cas d'eval
            //if ~ok then
            [flag_type,rdnom,DF_type,tt]=translate(CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,type_meth,degre,a_domaine,..
            b_domaine,Nbr_maillage,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,rdnom,mesures);
            //else
            //   tt=label(3);
            //end

            // augmentation du systeme avec les noeuds fictifs
            Nbr=Nbr_maillage;
            if ((CLa_type == 1) & (DF_type == 0 | DF_type == 1)) | ((CLb_type == 1) & (DF_type == 0 | DF_type == 2)) then
                Nbr=Nbr+1;
            end

            if ( mesures ==[]) then
                out=Nbr_maillage;
            else
                out=[Nbr_maillage;size(mesures,"*")];
            end
            if (flag_type == 1) then
                // explicite
                model.sim=list(rdnom,2004);
                if (find(oper == 1) ~= []) then
                    model.state=zeros(2*Nbr_maillage,1);
                else
                    model.state=zeros(Nbr_maillage,1);
                end
            elseif (flag_type == 2) then
                model.sim=list(rdnom,12004);
                if (find(oper == 1) ~= []) then
                    if (type_meth ==3 & (find(oper == 2) ~= [] | find(oper == 4) ~= [])) then
                        model.state=zeros(6*Nbr_maillage,1);
                    elseif (type_meth == 1) then
                        model.state=zeros(4*Nbr,1);
                    else
                        model.state=zeros(4*Nbr_maillage,1);
                    end
                else
                    if (type_meth == 3 & (find(oper == 2) ~= [] | find(oper == 4) ~= [])) then
                        model.state=zeros(4*Nbr_maillage,1);
                    elseif (type_meth == 1) then
                        model.state=zeros(2*Nbr,1);
                    else
                        model.state=zeros(2*Nbr_maillage,1);
                    end
                end
            end

            // Ecriture, compilation et linkage du code
            // if (fun(3) == "clickin") then
            // always ulink and link
            [ok1]=CFORTREDP(rdnom,tt);
            if ~ok1 then
                break,
            end
            //end

            if ~ok then
                [model,graphics,ok]=check_io(model,graphics,ones(k,1),out(:),[],[])
            end
            label(1)=params_pde;
            label(2)=tt;
            graphics.exprs=label;
            x.graphics=graphics;
            x.model=model;
            break
        end

    case "define" then
        model=scicos_model()
        model.state=zeros(10,1)
        model.sim=list("PDE",0)
        model.in=[1;1;1;1;1]
        model.out=[10;0]
        model.blocktype="c"
        model.dep_ut=[%f %t]

        //initialisation de l'ihm
        params_pde=tlist(["paramspde";"a";"b";"txt_exp";"check_op1";"a1";"b1";"check_op2";"a2";"b2";..
        "check_op3";"a3";"b3";"check_op4";"a4";"b4";"check_op5";"a5";"b5";"check_op6";"a6";"b6";..
        "check_op7";"a7";"b7";"discr_cst";"discr_non_cst";"signe";"rad_automatique";"rad_manuel";..
        "methode";"ord1";"ord2";"ord3";"degre";"nnode";"txt_pas";"CI";"dCI";"CLa";"CLa_exp";"CLb";..
        "CLb_exp";"points"],"","","","0","","IN_EDP1(t)","0","","IN_EDP2(t)","0","","IN_EDP3(t)",..
        "0","","IN_EDP4(t)","0","","IN_EDP5(t)","0","","IN_EDP6(t)","0","","IN_EDP7(t)","0","0",..
        "0","0","0","0","","","","","","","","","0","IN_CL1(t)","0","IN_CL2(t)","");
        // dans label on mis infos de scicos_getvalue, infos ihm et le code C
        label=list(params_pde,[],"");
        gr_i=[]
        x=standard_define([3 3],model,label,gr_i)

    end
endfunction

