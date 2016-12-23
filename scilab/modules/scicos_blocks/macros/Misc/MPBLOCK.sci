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

function [x,y,typ]=MPBLOCK(job,arg1,arg2)
    //
    // Copyright INRIA Oct 2006
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        model=arg1.model
        graphics=arg1.graphics
        //label=graphics.exprs;
        exprs=graphics.exprs

        //Compatibility
        if type(exprs)==15 then
            paramv=list();
            pprop=[];
            //must do something better than that !
            for i=1:size(model.rpar,"*")
                paramv($+1)=string(model.rpar(i))
                pprop($+1) = 0
            end

            exprs = tlist(["MPBLOCK","in","intype","out","outtype",...
            "param","paramv","pprop","nameF","funtxt"],...
            exprs(1)(1),..
            exprs(1)(2),..
            exprs(1)(3),..
            exprs(1)(4),..
            exprs(1)(5),..
            paramv,...
            sci2exp(pprop(:)),..
            exprs(1)(7),exprs(2))

        end

        //lab_1 = [in,intype,out,outtype,param,nameF]
        lab_1 = list(exprs.in,..       //1
        exprs.intype,..   //2
        exprs.out,..      //3
        exprs.outtype,..  //4
        exprs.param,..    //5
        exprs.pprop,..    //6
        exprs.nameF)      //7

        //lab_2 = [paramv]
        lab_2 = exprs.paramv //already a list

        //store exprs before modif
        //exprs.prev_exprs=exprs;
        //exprs.prev_exprs.prev_exprs=tlist("MPBLOCK");

        while %t do
            //     [ok,Tin,Tintype,Tout,Touttype,Tparam,Tparamv,Tfunam,lab]=..
            //         getvalue('Set Modelica generic block parameters',..
            //         ['Input variables:       ';..
            //          'Input variables types: ';..
            //          'Output variables:      ';..
            //          'Output variables types:';..
            //          'Parameters in Modelica:';..
            //          'Parameter  values:     ';..
            //          'Function name:         '],..
            //         list('str',-1,'str',-1,'str',-1,'str',-1,'str',-1,'str',-1,'str',-1),label(1))
            //warning here lab_1 is a list
            [ok,Tin,Tintype,Tout,Touttype,Tparam,pprop,Tfunam,lab_1]=..
            getvalue("Set Modelica generic block parameters",..
            ["Input variables:       ";..
            "Input variables types: ";..
            "Output variables:      ";..
            "Output variables types:";..
            "Parameters in Modelica:";..
            "Parameters properties: ";..
            "Model name in packages:"],..
            list("str",-1,"str",-1,"str",-1,"str",-1,"str",-1,..
            "vec",-1,"str",-1),lab_1)

            //Cancel case
            if ~ok then
                break
            end
            //   ierr=execstr("in=stripblanks(evstr(Tin));..
            //   intype=stripblanks(evstr(Tintype));..
            //   out=stripblanks(evstr(Tout));..
            //   outtype=stripblanks(evstr(Touttype));..
            //   param=stripblanks(evstr(Tparam));..
            //   paramv=evstr(Tparamv);..
            //   funam=stripblanks(Tfunam)",'errcatch')
            //
            //first stage of checking variable name
            //put string symbolic parameters in variable
            ierr=execstr("in=stripblanks(evstr(Tin));"+..
            "intype=stripblanks(evstr(Tintype));"+..
            "out=stripblanks(evstr(Tout));"+..
            "outtype=stripblanks(evstr(Touttype));"+..
            "param=stripblanks(evstr(Tparam));"+..
            "funam=stripblanks(Tfunam)","errcatch")

            if ierr<>0 then
                //You lose! Try again! Insert coin!
                //messagebox("Error, try again please!")
                messagebox("Error in evaluation of variables.")
                ok=%f
                //break
            end

            //check for valid name variable
            //in
            if ok then
                for i=1:size(in,"*")
                    r=%f;
                    ierr=execstr("r=validvar(in(i))","errcatch")
                    if ~r then
                        ok=%f
                        break
                    end
                end
                if ~ok then
                    messagebox(["Invalid variable name for the input "+string(i)+".";
                    """"+in(i)+"""";
                    "Please choose another variable name."]);
                end
            end
            //out
            if ok then
                for i=1:size(out,"*")
                    r=%f;
                    ierr=execstr("r=validvar(out(i))","errcatch")
                    if ~r then
                        ok=%f
                        break
                    end
                end
                if ~ok then
                    messagebox(["Invalid variable name for the output "+string(i)+".";
                    """"+out(i)+"""";
                    "Please choose another variable name."]);
                end
            end
            //param
            if ok then
                param=param(:)
                for i=1:size(param,"*")
                    r=%f;
                    ierr=execstr("r=validvar(param(i))","errcatch")
                    if ~r then
                        ok=%f
                        break
                    end
                end
                if ~ok then
                    messagebox(["Invalid variable name for the parameter "+string(i)+".";
                    """"+param(i)+"""";
                    "Please choose another variable name."]);
                end
            end

            // type checking
            //typeok=%t;
            if ok then
                for i=1:size(intype,"*")
                    if intype(i)<>"E"&intype(i)<>"I" then
                        //typeok=%f;
                        messagebox("Input type should be ''E'' or ''I''!");
                        ok=%f
                        break
                    end
                end
            end
            //    if ~typeok then
            //      ok=%f
            //      //break
            //    end

            if ok then
                for i=1:size(outtype,"*")
                    if outtype(i)<>"E"&outtype(i)<>"I" then
                        //typeok=%f;
                        messagebox("Output type should be ''E'' or ''I''!");
                        ok=%f
                        break
                    end
                end
            end
            //   if ~typeok then break, end

            //cross size checking
            if ok then
                if or(size(intype)<>size(in)) then
                    messagebox("Input variables are not well defined!");
                    ok=%f
                    //break;
                end
            end

            if ok then
                if or(size(outtype)<>size(out)) then
                    messagebox("Output variables are not well defined!");
                    ok=%f
                    //break;
                end
            end

            //check param properties
            if ok then
                pprop = pprop(:)
                if (size(param,"*")<>size(pprop,"*")) then
                    messagebox(["There are differences in size of param";
                    "and size of param properties."])
                    ok=%f
                    //break;
                end
            end

            if ok then
                if max(pprop)>2 | min(pprop)<0 then
                    messagebox(["Parameters properties must be:";
                    "0: if it is a paramater,";
                    "1: if it is an initial value of state,";
                    "2: it it is a fixed initial state value."])
                    ok=%f
                    //break;
                end
            end

            //check name of modelica file
            if ok then
                if funam=="" then
                    messagebox("The model name is not defined!")
                    ok=%f
                    //break
                end
            end

            if ok then
                [dirF,nameF,extF]=fileparts(funam);
                if (extF<>"" )|(dirF<>"") then
                    messagebox("Invalid model name!")
                    ok=%f
                    //break
                end
            end

            if ok then
                //set_io checking
                intypex=find(intype=="I"); outtypex=find(outtype=="I");
                //[model,graphics,ok]=check_io(model,graphics,ones(in),..
                //                             ones(out),[],[],intypex,outtypex)
                [model,graphics,ok]=set_io(model,graphics,list([ones(in),ones(in)],ones(in)),..
                list([ones(out),ones(out)],ones(out)),..
                [],[],intypex,outtypex)
            end

            //*****************************************************************************
            //generate second dialog box from Tparam
            if ok then

                Tparam_lab=evstr(Tparam) //labels
                Tparam_sz=size(Tparam_lab,"*") //numbers of labels

                //adjust size of lab_2 according to size of Tparam
                if Tparam_sz>lstsize(lab_2) then
                    for i=1:(Tparam_sz-lstsize(lab_2))
                        lab_2($+1)="0"
                    end
                elseif Tparam_sz<lstsize(lab_2) then
                    lab_2_tmp=list()
                    if Tparam_sz<>0 then //if param
                        for i=1:Tparam_sz
                            ee=evstr(exprs.param)
                            for j=1:size(ee,"r")
                                if ee(j)==Tparam_lab(i) then
                                    lab_2_tmp(i)=lab_2(j)
                                end
                            end
                        end
                        lab_2=lab_2_tmp
                    end
                end
                //generate lhs, label and rhs txt for getvalue
                if Tparam_sz<>0 then //if param
                    lhs_txt="" //lhs txt
                    lab_txt="" //label txt
                    rhs_txt="" //rhs txt
                    for i=1:Tparam_sz
                        lhs_txt=lhs_txt+"%v"+string(i)+","
                        if pprop(i) == 0 then
                            lab_txt=lab_txt+"''"+Tparam_lab(i)+"'';"
                        elseif  pprop(i) == 1 then
                            lab_txt=lab_txt+"''"+Tparam_lab(i)+" (state) '';"
                        elseif  pprop(i) == 2 then
                            lab_txt=lab_txt+"''"+Tparam_lab(i)+" (fixed state) '';"
                        end
                        rhs_txt=rhs_txt+"''vec'',-1,"
                    end
                    lhs_txt=part(lhs_txt,1:length(lhs_txt)-1)
                    lab_txt=part(lab_txt,1:length(lab_txt)-1)
                    rhs_txt=part(rhs_txt,1:length(rhs_txt)-1)

                    //generate main getvalue cmd
                    //warning here lab_2 is a list in input and a string in output
                    getvalue_txt = "[ok,"+lhs_txt+",lab_2]=scicos_getvalue(''Set parameters values'',["+..
                    lab_txt+"],"+..
                    "list("+rhs_txt+"),lab_2)"

                    //display the second dialog box
                    execstr(getvalue_txt)

                    //restore original lab_2 if not ok
                    if ~ok then
                        lab_2 = exprs.paramv
                    end
                end
            end

            //put output param in the form of a list
            //in paramv
            if ok then
                paramv=list();
                for i=1:Tparam_sz
                    execstr("paramv("+string(i)+")=%v"+string(i))
                end
            end
            //*****************************************************************************

            //============================---------MPBLOCK

            if ok then
                //------------------
                mo=modelica()
                mo.model=nameF
                mo.inputs=in;
                mo.outputs=out;
                if pprop~=[] then
                    if max(pprop)>0 then
                        mo.parameters=list(param',paramv,pprop')
                    else
                        mo.parameters=list(param',paramv)
                    end
                end
                model.equations=mo
                //------------------
                //model.rpar=paramv;
                model.rpar=[]
                for i=1:lstsize(paramv)
                    model.rpar=[model.rpar;
                    double(paramv(i)(:))]//value
                end

                model.sim(1)=funam
                //------------------
                exprs.in=lab_1(1)
                exprs.intype=lab_1(2)
                exprs.out=lab_1(3)
                exprs.outtype=lab_1(4)
                exprs.param=lab_1(5)
                exprs.paramv=list();
                if Tparam_sz<>0 then //if param
                    if type(lab_2)==15 then //for eval
                        for i=1:lstsize(lab_2)
                            exprs.paramv(i)=lab_2(i);//string
                        end
                    else
                        for i=1:size(lab_2,"*")
                            exprs.paramv(i)=lab_2(i);
                        end
                    end
                end
                exprs.pprop=lab_1(6)
                exprs.nameF=lab_1(7)
                exprs.funtxt="" // model is defined in the a package
                //label(2)=tt
                //------------------
                x.model=model
                graphics.gr_i(1)(1)="txt=['' " + nameF + " ''];"
                graphics.in_implicit =intype
                graphics.out_implicit=outtype
                //graphics.exprs=label
                graphics.exprs=exprs
                x.graphics=graphics
                break
            end
        end

        //Define
    case "define" then
        in=["u"]
        intype=["I"]
        out=["y1";"y2"]
        outtype=["I";"I"]
        param=[];
        paramv=list()
        pprop=[];
        nameF="myModel"

        //label=list([sci2exp(in),sci2exp(intype,sci2exp(out),..
        //           sci2exp(outtype),sci2exp(param),sci2exp(paramv),nameF]',[])

        exprs = tlist(["MPBLOCK","in","intype","out","outtype",...
        "param","paramv","pprop","nameF","funtxt"],...
        sci2exp(in(:)),..
        sci2exp(intype(:)),..
        sci2exp(out(:)),..
        sci2exp(outtype(:)),..
        sci2exp(param(:)),..
        list(string(0.1),string(.0001)),...
        sci2exp(pprop(:)),..
        nameF,[])

        model=scicos_model()
        model.blocktype="c"
        model.dep_ut=[%t %t]
        //model.rpar=paramv;
        model.rpar=[]
        for i=1:lstsize(paramv)
            model.rpar=[model.rpar;
            paramv(i)(:)]
        end

        mo=modelica()
        mo.model=nameF
        mo.parameters=list(param,paramv)
        model.sim=list(mo.model,10004)
        mo.inputs=in
        mo.outputs=out
        model.in=ones(size(mo.inputs,"r"),1)
        model.out=ones(size(mo.outputs,"r"),1)
        model.equations=mo
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
        x.graphics.in_implicit =intype
        x.graphics.out_implicit=outtype
    end
endfunction



