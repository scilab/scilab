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

function [txt,rpar,ipar] = create_modelica(blklst,corinvm,cmat,NvM,name,scs_m)

    if exists("%Modelica_Init")==0 then
        // Modelica_Init becomes true only in "Modelicainitialize_.sci"
        %Modelica_Init=%f;
    end
    if exists("%Modelica_ParEmb")==0 then
        %Modelica_ParEmb=%t;
    end

    Parembed=%Modelica_ParEmb & ~%Modelica_Init;

    txt=[];tab=ascii(9)
    rpar=[];//will contain all parameters associated with the all modelica blocs
    ipar=[];//will contain the "adress" of each block in rpar
    models=[]//will contain the model declaration part
    eqns=[]//will contain the modelica equations part
    Pin=[]
    Bnumbers=[]
    Bnam=[]
    Bnames=[]
    nb=size(blklst)
    Params=[];
    for k=1:nb
        ipar(k)=0
        o=blklst(k);
        //#########
        //## Params
        //#########
        mo=o.equations;
        np=size(mo.parameters(1),"*");
        P=[];
        //** mo.parameters have size=2
        //** it only contains parameters
        if np<>0 then
            if lstsize(mo.parameters)==2 then
                mo.parameters(3)=zeros(1,np)
            end
        end

        for j=1:np
            //## loop on number of param value
            //## can be both scalar or array
            Parj=mo.parameters(1)(j)
            Parjv=mo.parameters(2)(j)
            Parj_in=Parj+"_"+string(k)+string(j)

            if type(Parjv)==1 then // if Real/Complex	Integers are used with "fixed=true"
                rpar=[rpar;matrix(Parjv,-1,1)]
                ipar(k)=ipar(k)+size(Parjv,"*")
            end
            //======================================================
            Params=[Params;construct_Pars(Parj_in,Parjv,Parembed)]
            if mo.parameters(3)(j)==0 then
                P=[P;Parj+"="+Parj_in]
            elseif mo.parameters(3)(j)==1 then
                //eParjv=construct_redeclar(Parjv)
                P=[P;Parj+"(start="+Parj_in+")"];
            elseif mo.parameters(3)(j)==2 then
                //eParjv=construct_redeclar(Parjv)
                P=[P;Parj+"(start="+Parj_in+",fixed=true)"];
            end
            //======================================================
        end

        //#########
        //## models
        //#########
        Bnumbers=[Bnumbers k];

        //## update list of names of modelica blocks
        // Modelica compiler complains about ID containing dots
        // So remove them Modelica.package.subpackage => Modelica_DOT_package_DOT_subpackage
        Bnam = [Bnam, get_model_name(strsubst(mo.model, ".", "_DOT_"),Bnam)];
        Bnames = [Bnames, Bnam($)]

        if P==[] then
            models=[models;
            "  "+mo.model+" "+tab+Bnames($)];
        else
            models=[models;
            "  "+mo.model+" "+tab+Bnames($)+"("+strcat(P,", ")+")"];
        end

        //## Add gr_i identification in comments of models
        if mo.model<>"OutPutPort" & mo.model<>"InPutPort" then
            //## retrieve the object in the scs_m structure
            o_scsm = scs_m(scs_full_path(corinvm(k)));
            //## get the structure model
            o_model  = o_scsm.model;
            //## get the identification field
            id = stripblanks(o_model.label)

            if id<>"" then
                models($)=models($)+" """+id+""";"
            else
                models($)=models($)+";"
            end
        else
            models($)=models($)+";"
        end

        //rajouter les ports
    end
    ipar=cumsum([1;ipar(:)])

    //links
    for k=1:size(cmat,1)
        from=cmat(k,1:3)
        to=cmat(k,4:6)
        if from(1)==0 then //input port
            nb=nb+1
            Bnumbers=[Bnumbers nb];
            Bnames=[Bnames,"B"+string(nb)];
            models=[models;"  InPutPort"+" "+tab+"B"+string(nb)+";"];
            n1="B"+string(nb)
        elseif from(3)==1 then
            p1=blklst(from(1)).equations.inputs(from(2))
            n1=Bnames(find(Bnumbers==from(1)))
        else
            p1=blklst(from(1)).equations.outputs(from(2))
            n1=Bnames(find(Bnumbers==from(1)))
        end

        if to(1)==0 then //output port
            nb=nb+1
            Bnumbers=[Bnumbers nb];
            Bnames=[Bnames,"B"+string(nb)];
            models=[models;"  OutPutPort"+" "+tab+"B"+string(nb)+";"];
            n1="B"+string(nb)
        elseif to(3)==1 then
            p2=blklst(to(1)).equations.inputs(to(2))
            n2=Bnames(find(Bnumbers==to(1)))
        else
            if size(blklst(to(1)).equations.outputs,"*")<to(2) then pause,end
            p2=blklst(to(1)).equations.outputs(to(2))
            n2=Bnames(find(Bnumbers==to(1)))
        end

        if or(blklst(from(1)).equations.model==["InPutPort","OutPutPort"]) ...
            | or(blklst(to(1)).equations.model==["InPutPort","OutPutPort"]) ...
            | (k > (size(cmat,1) - NvM)) ...
            then
            eqns=[eqns
            "  "+n1+"."+p1+" = "+n2+"."+p2+";"]
        else
            eqns=[eqns
            "  connect ("+n1+"."+p1+","+n2+"."+p2+");"]
        end
    end

    txt=[txt;
    "model "+name
    Params
    models
    "equation"
    eqns
    "end "+name+";"]
endfunction

function r=validvar_modelica(s)
    r=validvar(s);

    if r then
        bad_char=["%" "#" "$"]
        for j=1:size(bad_char,2)
            if strindex(s,bad_char(j)) then
                r=%f
                return
            end
        end
    end
endfunction



function r=write_nD_format(x)
    sx=size(x)

    if size(sx,"*")==2 then // Matrix/Vector
        [nD1,nD2]=size(x)
        if nD1==1 then // rows vector
            r="{"+strcat(string(x),",")+"}"
            r=strsubst(r,"D","e");
            return r;
        elseif nD2==1   then // column vector
            N=nD1;
            cmd=")"
        else  //matrix
            N=sx(1);
            cmd=",:)"
        end
    else // hypermatrix
        // typeof(x)==hypermat
        //  xd=x.entries
        //  sdims=x.dims(2:$)
        //  N=x.dims(1)
        //  cmd=':)'
        //  n=size(sx,'c')
        //  for i=1:n-2;cmd=':,'+cmd;end;
        //  cmd=','+cmd;
    end
    r=[];
    for i=1:N
        cmdx="write_nD_format(x("+string(i)+cmd+")";
        execstr("r(i)="+cmdx,"errcatch")
    end
    r="{"+strcat(r,",")+"}";
endfunction

// a 2x3 matrix {{xx,xx,xx},{xx,xx,xx}}
// A[2] {xx,xx}
// A[1,2] {{xx,xx}}
// A[2,1] {{xx},{xx}}
// A[1,1,2] {{{xx,xx}}}
// a=rand(2,3)
// a=[3,4];
// a=[4;2];
// a=rand(2,3);
// a=rand(1,2,3,4,5);
// a=[1 2 3 4 1 4];a(:,:,2)=[5 6 7 8 1 5] ;
//if typeof(a)== 'hypermat' then
// disp('not supported')
//end
//sa=write_nD_format(a)


function     Pari=construct_Pars(Pari,opari,Parembed)

    if Pari==[] then
        return " "
    end
    // Pars='  parameter Real '+Pars+'(fixed=false);'
    [atemp]=format();
    format(20);// writing in long format

    //erpar=string(rpar); will put 1e-16 to zero in a vector containing
    //big numbers

    C=opari;
    [a1,b1]=size(C);
    npi=a1*b1;
    if typeof(C)== "hypermat" then
        messagebox(_("Hyper Matrix is not supported"),"error","modal")
        return
    end

    if (type(C)==1) then
        if isreal(C) then
            par_type="Real"
        else
            par_type="Complex"
        end
        FIXED="false"
    elseif (typeof(C)=="int32") | (typeof(C)=="int16") |...
        (typeof(C)=="int8") |(typeof(C)=="uint32") |...
        (typeof(C)=="uint16") | (typeof(C)=="uint8") then
        par_type="Integer"
        FIXED="true"
    else
        par_type="UnKnown_Type"
        FIXED="???"
        messagebox(_("Type not recognized"),"error","modal");ok=%f;
    end

    if ~Parembed then
        FIXED="true"
    end

    if (npi==1) then,
        eopari=strsubst(string(C),"D","e");
        fixings="(fixed="+FIXED+") "
    else
        eopari=write_nD_format(C)
        fixings="(each fixed="+FIXED+") ";
        [d1,d2]=size(C);
        if (d1==1) then
            Pari=Pari+"["+string(d2)+"]"; //[d2]
        else
            Pari=Pari+"["+string(d1)+","+string(d2)+"]"; //[d1,d2]
        end
    end
    Pari="  parameter "+par_type+" "+Pari+ fixings+"="+eopari+ "  """+Pari+""""+";"
    format(atemp(2))// restituing the format

endfunction

function eopari = construct_redeclar(opari)

    [atemp]=format();
    format(20);// writing in long format
    C=opari;
    npi=size(C,"*");

    if typeof(C)== "hypermat" then
        messagebox(_("Hyper Matrix is not supported"),"error","modal")
        return
    end
    if  ~isreal(C) then
        messagebox(_("Complex Matrix is not supported"),"error","modal")
        return
    end

    if (npi==1) then,
        eopari=strsubst(string(C),"D","e");
    else
        eopari=write_nD_format(C)
    end

    format(atemp(2))// restituing the format

endfunction
