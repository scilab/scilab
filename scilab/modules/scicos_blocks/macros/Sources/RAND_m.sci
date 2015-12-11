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

function [x,y,typ]=RAND_m(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,"*")==14 then
            exprs(9)=[],
        end //compatiblity
        while %t do
            [ok,typ,flag,a,b,seed_c,exprs]=scicos_getvalue([
            "Set Random generator block parameters";
            "flag = 0 : Uniform distribution A is min and A+B max";
            "flag = 1 : Normal distribution A is mean and B deviation";
            " ";
            "A and B must be matrix with equal sizes"],..
            ["Datatype(1=real double  2=complex)";"flag";"A";"B";"SEED"],..
            list("vec",1,"vec",1,"mat",[-1 -2],"mat","[-1 -2]","mat",[1 2]),exprs)
            if ~ok then
                break,
            end
            if flag<>0&flag<>1 then
                message("flag must be equal to 1 or 0")
            else
                out=size(a)
                if typ==1 then
                    function_name="rndblk_m";
                    model.rpar=[real(a(:));real(b(:))]
                    model.dstate=[seed_c(1);0*real(a(:))]
                    ot=1
                elseif typ==2 then
                    function_name="rndblkz_m";
                    ot=2
                    model.rpar=[real(a(:));imag(a(:));real(b(:));imag(b(:))]
                    model.dstate=[seed_c(:);0*[real(a(:));imag(a(:))]]
                else
                    message("Datatype is not supported");
                    ok=%f;
                end
                if ok then
                    [model,graphics,ok]=set_io(model,graphics,list([],[]),list(out,ot),1,[])
                    if ok then
                        model.sim=list(function_name,4)
                        graphics.exprs=exprs
                        model.ipar=flag
                        x.graphics=graphics;
                        x.model=model
                        break
                    end
                end
            end
        end
    case "define" then
        a=0
        b=1
        dt=0
        flag=0
        function_name="rndblk_m";
        funtyp=4;
        model=scicos_model()
        model.sim=list(function_name,funtyp)
        model.in=[]
        model.in2=[]
        model.intyp=[]
        model.out=1
        model.out2=1
        model.outtyp=1
        model.evtin=1
        model.evtout=[]
        model.state=[]
        model.dstate=[int(rand()*(10^7-1));0*a(:)]
        model.rpar=[a(:),b(:)]
        model.ipar=flag
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%f %f]

        exprs=[sci2exp(1);string(flag);sci2exp([a]);sci2exp([b]);sci2exp([model.dstate(1) int(rand()*(10^7-1))])]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
