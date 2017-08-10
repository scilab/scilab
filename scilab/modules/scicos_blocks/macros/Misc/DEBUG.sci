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

function [x,y,typ]=DEBUG(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        textmp=exprs(2)
        ok=%t
        while 1==1
            [txt]=dialog(["Enter scilab instructions for debugging.";
            " Inputs are block and flag, output is block"],..
            textmp);

            if txt<>[] then
                tt=["block=debug_scicos(block,flag)"]

                if execstr("deff(tt,txt)","errcatch")==0 then
                    save(TMPDIR+"/debug_scicos", "debug_scicos")
                    exprs(2)=txt
                    if (scicos_debug()<>2 & scicos_debug()<>3) then
                        scicos_debug(2)
                    end
                    break
                else
                    message(["Error in the instructions";lasterror()])
                end
            else
                ok=%f;
                break;
            end
        end
        if ok then
            graphics.exprs=exprs;
            x.graphics=graphics;
        end

    case "define" then
        model=scicos_model()
        model.sim=list("%debug_scicos",99)
        model.blocktype="d"

        exprs=list("","xcos_debug_gui(flag,block);")
        gr_i=[]
        x=standard_define([8 2],model,exprs,gr_i)
    end
endfunction
