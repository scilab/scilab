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

function cb_IHM_EDP()

    if get(gcbo, "tag") == "quitmenu" | get(gcbo, "tag") == "cancelbutton" then
        fin=%f;
        ok=%t;
        set(findobj("tag","PDE_GUI"), "userdata",[ok, fin])
    elseif get(gcbo, "tag") == "visubutton" then
        visualiser();
    elseif  get(gcbo, "tag") == "check1" then
        if (get(findobj("tag","check2"),"Value") == 1) then
            set(findobj("tag","check2"),"Value",0);
        end
    elseif  get(gcbo, "tag") == "check2" then
        if (get(findobj("tag","check1"),"Value") == 1) then
            set(findobj("tag","check1"),"Value",0);
        end
    elseif  get(gcbo, "tag") == "rad_automatique" then
        if (get(findobj("tag","rad_manuel"),"Value") == 1) then
            set(findobj("tag","rad_manuel"),"Value",0);
        end
    elseif  get(gcbo, "tag") == "rad_manuel" then
        if (get(findobj("tag","rad_automatique"),"Value") == 1) then
            set(findobj("tag","rad_automatique"),"Value",0);
        end
    elseif  get(gcbo, "tag") == "bb" then
        afficher();
    elseif  get(gcbo, "tag") == "b2" then
        add_list_points()
    elseif  get(gcbo, "tag") == "b5" then
        del_list_points()
    elseif  get(gcbo, "tag") == "b3" then
        do_valider()
    end

endfunction

function do_valider()

    // verification du domaine
    if (get(findobj("tag","edita"),"String") == "" | get(findobj("tag","editb"),"String") == "") then
        messagebox(["veuillez renseigner les valeurs";"du domaine de la discretisation"],"modal","error");
        setfin(%t);
        return
    end

    // Test de renseignements sur les differents champs de l'IHM
    if (get(findobj("tag","check_op1"),"Value") == 1) then
        if (get(findobj("tag","edita1"),"String") == "" | get(findobj("tag","editb1"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"    de l''operateur d2u/dt2"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op2"),"Value")== 1) then
        if (get(findobj("tag","edita2"),"String") == "" | get(findobj("tag","editb2"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"    de l''operateur d2u/dx2"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op3"),"Value")== 1) then
        if (get(findobj("tag","edita3"),"String") == "" | get(findobj("tag","editb3"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"    de l''operateur du/dt"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op4"),"Value")== 1) then
        if (get(findobj("tag","edita4"),"String") == "" | get(findobj("tag","editb4"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"    de l''operateur d2u/dtdx"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op5"),"Value")== 1) then
        if (get(findobj("tag","edita5"),"String") == "" | get(findobj("tag","editb5"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"    de l''operateur du/dx"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op6"),"Value")== 1) then
        if (get(findobj("tag","edita6"),"String") == "" | get(findobj("tag","editb6"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"      de l''operateur u"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op7"),"Value")== 1) then
        if (get(findobj("tag","edita7"),"String") == "" | get(findobj("tag","editb7"),"String") == "") then
            messagebox(["veuillez renseigner les coefficients";"      de l''operateur f"],"modal","error");
            setfin(%t);
            return
        end
    end

    // verification du renseignement du signe du discriminant non constant
    if (get(findobj("tag","check2"),"Value")== 1) then
        if (get(findobj("tag","liste_signe"),"Value") == 0 ) then
            messagebox(["veuillez renseigner le signe";"du discriminant non constant"],"modal","error");
            setfin(%t);
            return
        end
    end

    // verification des informations du choix manuel
    if (get(findobj("tag","rad_manuel"),"Value")== 1) then
        if (get(findobj("tag","liste_meth"),"Value") == 0 ) then
            messagebox(["veuillez renseigner le type";"de la methode de discretisation"],"modal","error");
            setfin(%t);
            return
        end

        if (get(findobj("tag","editdegre"),"String") == "" ) then
            messagebox(["veuillez renseigner le degre";"de la methode de discretisation"],"modal","error");
            setfin(%t);
            return
        end

        if (get(findobj("tag","editpas"),"String") == "" ) then
            messagebox(["veuillez renseigner le nombre";"de points de discretisation"],"modal","error");
            setfin(%t);
            return
        end
    end
    // verification du renseignement des conditions intiales
    if (get(findobj("tag","check_op1"),"Value")==1 | get(findobj("tag","check_op3"),"Value")== 1 | get(findobj("tag","check_op4"),"Value")== 1) then
        if (get(findobj("tag","editCI"),"String") == "" ) then
            messagebox(["veuillez renseigner la condition";"         initiale u(x,t0)"],"modal","error");
            setfin(%t);
            return
        end
    end
    if (get(findobj("tag","check_op1"),"Value")== 1) then
        if (get(findobj("tag","editCI1"),"String") == "" ) then
            messagebox(["veuillez renseigner la condition";"         initiale du/dx|t0"],"modal","error");
            setfin(%t);
            return
        end
    end
    // verification du renseignement des conditions aux limites
    if (get(findobj("tag","editCLa"),"String") == "" | get(findobj("tag","editCLb"),"String") == "" ) then
        messagebox(["veuillez renseigner les conditions";"         aux limites"],"modal","error");
        setfin(%t);
        return
    end

    setfin(%f); // Pour sortir sans problemes de l'IHM
endfunction

function add_list_points()
    // développé par EADS-CCR
    // cette fonction gere l'ajout //
    // d'un poit de mesure         //
    // ----------------------------//
    if ~isempty(findobj("tag","edit_pt")) then
        new = get(findobj("tag","edit_pt"),"String");
        newv=evstr(new);
        if (newv < evstr(get(findobj("tag","edita"),"String"))-%eps | newv > evstr(get(findobj("tag","editb"),"String"))+%eps) then
            messagebox("le point choisi est en dehors du domaine","modal","error");
            return;
        end

        list_points = get(findobj("tag","liste_pts"),"string");
        //list_points = tokens(points, "|");

        if (list_points == "") then
            list_points=[];
        end
        list_points = [list_points  new];

        set(findobj("tag","liste_pts"),"String",strcat(list_points,"|"));

    end
endfunction

function del_list_points()
    // développé par EADS-CCR
    // cette fonction gère la supprission //
    // d'un poit de mesure         //
    // ----------------------------//
    if ~isempty(findobj("tag","edit_pt")) then
        del = get(findobj("tag","liste_pts"),"Value");

        list_points = get(findobj("tag","liste_pts"),"string");
        //list_points = tokens(points, "|");

        if (list_points == "") then
            list_points=[];
        end
        list_points =[list_points(1:del-1) list_points(del+1:$)];

        set(findobj("tag","liste_pts"),"String",strcat(list_points,"|"));

    end
endfunction

function visualiser()
    // développé par EADS-CCR
    // Cette fonction est pour l'affichage de l'EDP dans l'IHM //
    // selon les opérateurs choisis                            //
    // --------------------------------------------------------//

    expression=[];
    if (get(findobj("tag","check_op1"),"Value")== 1) then
        expression="("+get(findobj("tag","edita1"),"String")+")*b1(t)*d2u/dt2";
    end
    if (get(findobj("tag","check_op2"),"Value")== 1) then
        if isempty(expression) then
            expression="("+get(findobj("tag","edita2"),"String")+")*b2(t)*d2u/dx2";
        else
            expression=expression+"+("+get(findobj("tag","edita2"),"String")+")*b2(t)*d2u/dx2";
        end
    end
    if (get(findobj("tag","check_op3"),"Value")== 1) then
        if isempty(expression) then
            expression="("+get(findobj("tag","edita3"),"String")+")*b3(t)*du/dt";
        else
            expression=expression+"+("+get(findobj("tag","edita3"),"String")+")*b3(t)*du/dt";
        end
    end
    if (get(findobj("tag","check_op4"),"Value")== 1) then
        if isempty(expression) then
            expression="("+get(findobj("tag","edita4"),"String")+")*b4(t)*d2u/dtdx";
        else
            expression=expression+"+("+get(findobj("tag","edita4"),"String")+")*b4(t)*d2u/dtdx";
        end
    end
    if (get(findobj("tag","check_op5"),"Value")== 1) then
        if isempty(expression) then
            expression="("+get(findobj("tag","edita5"),"String")+")*b5(t)*du/dx";
        else
            expression=expression+"+("+get(findobj("tag","edita5"),"String")+")*b5(t)*du/dx";
        end
    end
    if (get(findobj("tag","check_op6"),"Value")== 1) then
        if isempty(expression) then
            expression="("+get(findobj("tag","edita6"),"String")+")*b6(t)*u";
        else
            expression=expression+"+("+get(findobj("tag","edita6"),"String")+")*b6(t)*u";
        end
    end
    if (get(findobj("tag","check_op7"),"Value")== 1) then
        if isempty(expression) then
            expression="Veuillez selectionner les operateurs";
        else
            expression=expression+"=("+get(findobj("tag","edita7"),"String")+")*b7(t)";
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
        set(findobj("tag","txt_exp"),"String",expression(1));
        set(findobj("tag","txt_exp2"),"String","");
    elseif (size(expression,"*")== 2) then
        set(findobj("tag","txt_exp"),"String",expression(1));
        set(findobj("tag","txt_exp2"),"String",expression(2));
    end
endfunction

function afficher()
    // développé par EADS-CCR
    // Cette fonction est pour l'affichage de le pas de maillage //
    // --------------------------------------------------------//

    editb = evstr(get(findobj("tag","editb"),"String"))
    edita = evstr(get(findobj("tag","edita"),"String"))
    editpas = evstr(get(findobj("tag","editpas"),"String"));

    if isempty(editb) | isempty(edita) | isempty(editpas) then
        return
    end

    expression= msprintf("%.10g",(editb-edita)/(editpas-1));
    set(findobj("tag","txt_pas"),"String",expression);

endfunction

function setfin(value)
    ud = get(findobj("tag","PDE_GUI"), "userdata")
    ud(2) = value
    set(findobj("tag","PDE_GUI"), "userdata",ud)
endfunction
