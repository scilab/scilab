//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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

function txt=gen_modelica(scs_m,name)
    txt=[];tab=ascii(9)
    models=[]
    eqns=[]
    Pin=[]
    Bnumbers=[]
    Bnames=[]

    IN=[];OUT=[];
    scs_m;
    for i=1:size(scs_m.objs)
        if typeof(scs_m.objs(i))=="Block" then
            if scs_m.objs(i).gui=="IN_f" then
                scs_m.objs(i).gui="INPUTPORT";
                mo=modelica();mo.model="InPutPort";mo.outputs="vo";mo.inputs="vi";
                scs_m.objs(i).model.equations=mo;
                IN=[IN scs_m.objs(i).model.ipar]
            elseif scs_m.objs(i).gui=="OUT_f" then
                scs_m.objs(i).gui="OUTPUTPORT";
                mo=modelica();mo.model="OutPutPort";mo.outputs="vo";mo.inputs= "vi";
                scs_m.objs(i).model.equations=mo
                OUT=[OUT  scs_m.objs(i).model.ipar]
            end
        end
    end

    [blklst,cmat,ccmat,cor,corinv,ok]=pass1(scs_m);
    if ~ok then return,end

    //blocks
    for k=1:size(blklst)
        o=blklst(k);
        if and(getfield(1,o)<>"equations")
            messagebox("block numbered "+string(k)+" is not a Modelica block","modal")
            txt=[]
            return
        end
        if typeof(o.equations)<>"modelica" then
            messagebox("block numbered "+string(k)+" is not a Modelica block","modal")
            txt=[]
            return
        end
        mo=o.equations;
        np=size(mo.parameters(1),1);
        P=[];
        for l=1:np
            pexp=sci2exp(mo.parameters(2)(l))

            P=[P;
            mo.parameters(1)(l)+"="+value2modelica(mo.parameters(2)(l))];
        end
        Bnumbers=[Bnumbers k];
        Bnames=[Bnames,"B"+string(k)];
        if P==[] then
            models=[models;"  "+mo.model+" "+tab+"B"+string(k)+";"];
        else
            models=[models;"  "+mo.model+" "+tab+"B"+string(k)+"("+strcat(P,", ")+");"];
        end
    end
    //links
    for k=1:size(cmat,1)
        from=cmat(k,1:3)
        to=cmat(k,4:6)
        if from(3)==1 then
            p1=blklst(from(1)).equations.inputs(from(2))
        else
            p1=blklst(from(1)).equations.outputs(from(2))
        end
        n1=Bnames(find(Bnumbers==from(1)))
        if to(3)==1 then
            p2=blklst(to(1)).equations.inputs(to(2))
        else
            p2=blklst(to(1)).equations.outputs(to(2))
        end
        n2=Bnames(find(Bnumbers==to(1)))
        if or(blklst(from(1)).equations.model==["InPutPort","OutPutPort"]) ...
            | or(blklst(to(1)).equations.model==["InPutPort","OutPutPort"]) ...
            then
            eqns=[eqns
            "  "+n1+"."+p1+" = "+n2+"."+p2+";"]
        else
            eqns=[eqns
            "  connect ("+n1+"."+p1+","+n2+"."+p2+");"]
        end
    end
    txt=[txt;
    "class "+name
    models
    "equation"
    eqns
    "end "+name+";"]
endfunction

