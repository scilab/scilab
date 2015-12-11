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

function [x,y,typ] = FROMWS_c(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,varnam,Method,ZC,OutEnd,exprs]=scicos_getvalue("Set From_Workspace block parameters",..
            ["Variable name";
            "Interpolation Method";
            "Enable zero crossing(0:No, 1:Yes)?";
            "Output at end(0:Zero, 1:Hold, 2:Repeat)"],...
            list("str",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if ~(Method==0 | Method==1| Method==2| Method==3) then
                message("Interpolation method should be chosen in [0,1,2,3]");
                ok=%f;
            end

            if ~(ZC==0 | ZC==1) then
                message("Zero crossing should be either 0 or 1");
                ok=%f;
            end

            if ~(OutEnd==0 | OutEnd==1| OutEnd==2) then
                message("Output at end option should be either 0 or 1");
                ok=%f;
            end

            //check for valid name variable
            r=%f;
            ierr=execstr("r=validvar(varnam)","errcatch")
            if ~r then
                message(["Invalid variable name.";
                "Please choose another variable name."]);
                ok=%f
            end

            if ok then
                model.ipar=[length(ascii(varnam));ascii(varnam)';Method;ZC;OutEnd;];
                [model,graphics,ok]=set_io(model,graphics,list(),list([-1,-2],-1),1,1);
                if ok then
                    graphics.exprs=exprs;
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end

    case "define" then
        varnam="V";// V.time=0; V.value=1;
        Method=1;
        ZC=1;
        OutEnd=0;

        model=scicos_model();
        model.sim=list("fromws_c",4);
        model.out=-1
        model.out2=-2
        model.outtyp=-1
        model.ipar=[length(ascii(varnam));ascii(varnam)';Method;ZC;OutEnd;];
        model.evtin=[1];
        model.evtout=[1];
        model.firing=[0];
        model.blocktype="d";
        model.dep_ut=[%f %t];
        gr_i=[]
        exprs=[string(varnam);string(Method);string(ZC);string(OutEnd)];
        x=standard_define([3.5 2],model,exprs,gr_i)
    end
endfunction

