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

function [%cpr,%state0,needcompile,alreadyran,ok]=do_update(%cpr,%state0,needcompile)
    //Update an already compiled scicos diagram compilation result according to
    //parameter changes
    //!
    ok=%t
    select needcompile
    case 0 then  // only parameter changes
        if size(newparameters)<>0 then
            cor=%cpr.cor
            [%state0,state,sim,ok]=modipar(newparameters,%state0,%cpr.state,%cpr.sim)
            if ~ok then
                alreadyran=do_terminate();
                disp("Partial compilation failed. Attempting a full compilation.");
                needcompile=4
                [%cpr,ok]=do_compile(scs_m)
                if ok then
                    %state0=%cpr.state
                    needcompile=0
                    return;
                end
            end
            %cpr.state=state,%cpr.sim=sim
        end
    case 1 then // parameter changes and/or port sizes change
        if size(newparameters)<>0 then
            // update parameters or states
            cor=%cpr.cor
            [%state0,state,sim,ok]=modipar(newparameters,%state0,%cpr.state,%cpr.sim)
            if (~ok| findinlistcmd(%cpr.corinv,size(cor),">")<>list() ) then
                alreadyran=do_terminate()
                disp("Partial compilation failed. Attempting a full compilation.");
                needcompile=4
                [%cpr,ok]=do_compile(scs_m)
                if ok then
                    %state0=%cpr.state
                    needcompile=0
                end
                return;
            end
            %cpr.state=state,%cpr.sim=sim
        end
        //update port sizes.
        // NB: if modelica part block size has been changed, the diagram is recompiled
        bllst=list();
        corinv=%cpr.corinv
        sim=%cpr.sim
        for k=1:size(corinv)
            if type(corinv(k))==1 then //dont take care of modelica blocks
                if size(corinv(k),"*")==1 then
                    bllst(k)=scs_m.objs(corinv(k)).model;
                else
                    path=list("objs");
                    for l=corinv(k)(1:$-1),
                        path($+1)=l;
                        path($+1)="model";
                        path($+1)="rpar";path($+1)="objs";
                    end
                    path($+1)=corinv(k)($);
                    path($+1)="model";
                    bllst(k)=scs_m(path);
                end
            else // modelica block
                //build a fake bllst(k) only for in and out fields
                m=scicos_model();
                m.in=ones(sim.inpptr(k+1)-sim.inpptr(k),1)
                m.out=ones(sim.outptr(k+1)-sim.outptr(k),1)
                bllst(k)=m;
            end
        end
        [ok,bllst]=adjust(bllst,sim("inpptr"),sim("outptr"),sim("inplnk"),..
        sim("outlnk"))
        if ok then
            [lnksz,lnktyp]=lnkptrcomp(bllst,sim("inpptr"),sim("outptr"),...
            sim("inplnk"),sim("outlnk"))
            %cpr.state("outtb")=buildouttb(lnksz,lnktyp)
            %state0("outtb")=buildouttb(lnksz,lnktyp)
            needcompile=0
        end

    case 2 then // partial recompilation
        alreadyran=do_terminate()
        [%cpr,ok]=c_pass3(scs_m,%cpr)
        if ok then
            %state0=%cpr.state
            needcompile=0;
            return;
        end
        disp("Partial compilation failed. Attempting a full compilation.");
        [%cpr,ok]=do_compile(scs_m)
        if ok then
            %state0=%cpr.state
            needcompile=0
        end
    case 4 then  // full compilation
        alreadyran=do_terminate()
        [%cpr,ok]=do_compile(scs_m)
        if ok then
            %state0=%cpr.state
            needcompile=0
        end
    end

endfunction
