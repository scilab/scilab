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

function [x,y,typ]=SampleCLK(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs
        while %t do
            [ok,frequ,offset,exprs]=scicos_getvalue("Set block parameters",..
            ["Sample time";"Offset"],..
            list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if frequ<0 then
                message("Frequency must be a positif number");
                ok=%f;
            end
            if abs(offset) > frequ then
                message("The |Offset| must be less than the Frequency");
                ok=%f
            end
            if ok then
                if or(model.rpar(:)<>[frequ;offset]) then
                    needcompile=4;
                    y=needcompile,
                end
                model.rpar=[frequ;offset]
                model.evtout=1
                model.firing=-1//compatibility
                //       aa=max(length(exprs(1)),length(exprs(2)));
                //       if aa>1 then
                //          graphics.sz=[10*aa 20]
                //       else
                //          graphics.sz=[20 20]
                //       end
                graphics.exprs=exprs
                x.graphics=graphics
                x.model=model
                break
            end
        end
        needcompile=resume(needcompile)
    case "define" then
        model=scicos_model()
        model.sim="sampleclk"
        model.evtout=1
        model.rpar=[1,0]
        model.blocktype="d"
        model.firing=-1
        model.dep_ut=[%f %f]

        exprs=[sci2exp(1);sci2exp(0)]
        x=standard_define([2 2],model,exprs," ")
        //  x.graphics.id="S-CLK"
    end
endfunction


