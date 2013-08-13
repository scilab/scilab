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
function [nipar,nrpar,nopar,nz,nx,nx_der,nx_ns,nin,nout,nm,ng,dep_u]=reading_incidence(incidence)
    // this function creates the matrix dep_u given by the xml format.
    // It is used for modelica compiler.
    // number of lines represents the number of input, number of columns represents the number of outputs.
    // Fady 02/09/08
    xmlformat=mgetl(incidence);
    typ=[];input_name=[];order=[];depend=[];
    global txtline;txtline=0;
    corresp_table=["<number_of_integer_parameters","nipar";
    "<number_of_real_parameters","nrpar";
    "<number_of_string_parameters","nopar";
    "<number_of_discrete_variables","nz";
    "<number_of_continuous_states","nx_der";
    "<number_of_continuous_variables","nx_ns";
    "<number_of_continuous_unknowns","nx";
    "<number_of_inputs","nin";
    "<number_of_outputs","nout";
    "<number_of_modes","nm";
    "<number_of_zero_crossings","ng"]
    while and(typ<>"</model") do
        typ=get_typ(xmlformat);
        if typ(1)=="<model_info" then
            typ=get_typ(xmlformat);
            while typ(1)<>"</model_info" do
                val=corresp_table(find(corresp_table==typ(1)),2)
                ttyp=tokens(typ(2),"<");
                execstr(val+"="+ttyp(1)+";");
                typ=get_typ(xmlformat);
            end
        elseif typ(1)=="<identifiers" then
            while typ(1)<>"</identifiers"
                typ=get_typ(xmlformat);
                if typ(1)=="<input" then
                    ttyp=tokens(typ(2),"<");
                    input_name=[input_name;ttyp(1)];
                end
            end
        elseif typ(1)=="<outputs" then
            while typ(1)<>"</outputs" do
                typ=get_typ(xmlformat);
                if typ(1)=="<output" then
                    while typ(1)<>"</output" do
                        typ=get_typ(xmlformat);
                        if typ(1)=="<order" then
                            ttyp=tokens(typ(2),"<");
                            ord=evstr(ttyp(1));
                        elseif typ(1)=="<dependencies"
                            dep_flag=%f;
                            while typ(1)<>"</dependencies" then
                                typ=get_typ(xmlformat);
                                if typ(1)=="<input" then
                                    dep_flag=%t;
                                    ttyp=tokens(typ(2),"<");
                                    depend=[depend;ttyp(1)];
                                    order=[order;ord];
                                end
                            end
                            if ~dep_flag then
                                order=[order;ord]
                                depend=[depend;"NAN"];
                            end
                        end
                    end
                end
            end
        end
    end
    clearglobal txtline;
    //  if depend<>[] then
    //    dep_u=(ones(size(input_name,'*'),size(unique(depend),'*'))==zeros(size(input_name,'*'),size(unique(depend),'*')))
    //  else
    //    dep_u=%f
    //  end
    //  ind=find(depend=='NAN')
    //  depend(ind(:))=[];
    //  order(ind(:))=[];
    //  for i=1:size(depend,'*')
    //    in_num=find(input_name==depend(i));
    //    dep_u(in_num,order(i)+1)=%t;
    //  end

    nu=size(input_name,"*");
    dep_u=ones(1,nu)==zeros(1,nu);
    for i=1:nu
        if find(depend==input_name(i))==[] then
            dep_u(1,i)=%f;
        else
            dep_u(1,i)=%t;
        end
    end
    // remind taht inputs are numbered according to their position in the
    // diagram and not in the Modelica block.InPutPortx.viis the x-th
    // input in the whole diagram!

endfunction


function t=read_new_line(txt)
    global txtline
    txtline=txtline+1;
    t=txt(txtline)
endfunction

function typ=get_typ(txt)
    t=read_new_line(txt);
    typ=tokens(t);
    typ=tokens(typ(1),">");
endfunction
