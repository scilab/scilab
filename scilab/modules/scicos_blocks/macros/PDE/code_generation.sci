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

function Code=code_generation(rdnom,equations,eq_pts_mes,flag_type,h,CI,CI1,a,..
    N,Ninitiale,impl_type,type_meth,oper)
    // Cette fonction conserne la génération du code de la la fonction de calcul du bloc EDP //
    // entrées :                                                                             //
    //           - equations : vecteurs de chaine de caractères corespond aux équation ODE   //
    //                         ou DAE générées selon les différentes méthodes de             //
    //                         discrétisation.                                               //
    //           - impl_type : entier correspond au type des équations DAE (implicites)       //
    //                         ( -1 pour les systèmes algébriques, 1 pour les systèmes       //
    //                         algébro-différentiels).                                       //
    // sortie :                                                                              //
    //           - Code : vecteur de chaine de caratères qui renvoie le code du bloc à        //
    //                    imprimer par la suite dans le fichier .c                           //
    // pour plus d'information voir les fonctions de calcul des blocs Scicos de type 4       //
    // (explicite) et de type 10004 (implicite).                                             //
    //---------------------------------------------------------------------------------------//

    Code=["#include <scicos_block.h>"
    "#include <math.h>"
    " "
    "void  "+rdnom+"(scicos_block *block,int flag)"
    "{"
    " "
    " double **inptr = block->inptr;"
    " double **outptr = block->outptr;"
    " double *x = block->x;"
    " int nx = block->nx;"
    " double *xd = block->xd;"]
    if (flag_type == 2) then
        Code=[Code
        " double *res = block->res;"]
        if (type_meth == 3 & (find(oper == 2) ~= [] | find(oper == 4) ~= [])) then
            if (find(oper == 1) ~= []) then
                Code=[Code
                " int property["+string(3*N)+"];"]
            else
                Code=[Code
                " int property["+string(2*N)+"];"]
            end
        elseif (find(oper == 1) ~= []) then
            Code=[Code
            " int property["+string(2*N)+"];"]
        else
            Code=[Code
            " int property["+string(N)+"];"]
        end
    end
    Code=[Code
    " "
    " int i;"
    " double t = get_scicos_time();"
    " "
    " if (flag == 0){"
    equations
    " }else if (flag == 1){"];
    if (type_meth == 3 & (find(oper == 2) ~= [] | find(oper == 4) ~= [])) then
        sorties1=["   /* la première sortie */ "
        "   for (i=0;i<"+string(N)+";i++){"
        "     outptr[0][i]=x[i+"+string(N)+"];"
        "   }"];
        sorties2=["   /* la deuxième sortie */ "];
        for i=1:size(eq_pts_mes,"*")
            sorties2=[sorties2
            "   outptr[1]["+string(i-1)+"]="+eq_pts_mes(i)+";"];
        end
    else
        if (kbc(1) == 1) & (DF_type == 0 | DF_type == 1) then
            sorties1=["   /* la première sortie */ "
            "   for (i=1;i<"+string(Ninitiale)+";i++){"
            "     outptr[0][i]=x[i];"
            "   }"];
        else
            sorties1=["   /* la première sortie */ "
            "   for (i=0;i<"+string(Ninitiale)+";i++){"
            "     outptr[0][i]=x[i];"
            "   }"];
        end
        sorties2=["   /* la deuxième sortie */ "];
        for i=1:size(eq_pts_mes,"*")
            sorties2=[sorties2
            "   outptr[1]["+string(i-1)+"]="+eq_pts_mes(i)+";"];
        end
    end
    Code=[Code
    sorties1
    sorties2
    " }else if (flag == 4){"];
    condini=[];
    x=a;
    // si on a un systeme algebrique on a pas besoin des conditions initiales.
    if (impl_type ~= -1) then
        if (find(oper == 1) == []) then
            for i=1:N
                condini=[condini
                "   x["+string(i-1)+"]="+msprintf("%.16g",evstr(CI))+";"];
                x=x+h;
            end
        else
            for i=1:N
                condini=[condini
                "   x["+string(i-1)+"]="+msprintf("%.16g",evstr(CI))+";";
                "   x["+string(i+N-1)+"]="+msprintf("%.16g",evstr(CI1))+";"];
                x=x+h;
            end
        end
    end
    Code=[Code
    condini
    "/* }else if (flag == 5){ */"];
    final=[];
    Code=[Code
    final]
    if (flag_type == 2) then
        property=[];
        if (find(oper == 1) ~= []) then
            if (impl_type == 0) then
                for i=1:N
                    property=[property
                    "   property["+string(i-1)+"]=-1;";
                    "   property["+string(i+N-1)+"]=1;";
                    "   property["+string(i+2*N-1)+"]=1;"];
                end
            else
                for i=1:2*N
                    property=[property
                    "   property["+string(i-1)+"]="+string(impl_type)+";"];
                end
                property(N+1)="   property["+string(N)+"]="+string(-1)+";";
                property($)="   property["+string(2*N-1)+"]="+string(-1)+";";
            end
        else
            if (impl_type == 0) then
                for i=1:N
                    property=[property
                    "   property["+string(i-1)+"]=-1;";
                    "   property["+string(i+N-1)+"]=1;"];
                end
            else
                if (type_meth ==3 & (find(oper == 2) ~= [] | find(oper == 4) ~= [])) then
                    for i=1:2*N
                        property=[property
                        "   property["+string(i-1)+"]="+string(impl_type)+";"];
                    end
                else
                    property=[property
                    "   property["+string(0)+"]="+string(-1)+";"];
                    for i=2:N-1
                        property=[property
                        "   property["+string(i-1)+"]="+string(impl_type)+";"];
                    end
                    property=[property
                    "   property["+string(N-1)+"]="+string(-1)+";"];
                end
            end
        end

        Code=[Code
        " }else if (flag == 7){"
        property
        "  set_pointer_xproperty(property);"]
    end
    Code=[Code
    " }"
    " return;"
    "}"];

endfunction

