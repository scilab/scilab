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

function [x,y,typ]=MAXMIN(job,arg1,arg2)
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
            [ok,mm,nin,zcr,exprs]=..
            scicos_getvalue("Set Max/Min block parameters",..
            ["Min (1) or Max (2) ";"Number of input vectors (1 or 2)";"zero-crossing (1: yes, 0;no)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if zcr<>0 then
                zcr=-1,
            end
            if mm<>1 then
                mm=2,
            end
            if nin<>1 & nin<>2 then
                message("Wrong number of inputs, only 1 and 2 allowed")
                ok=%f
            end
            if ok then
                if nin==1 then
                    [model,graphics,ok]=check_io(model,graphics,-1,1,[],[])
                else
                    [model,graphics,ok]=check_io(model,graphics,[-1,-1],-1,[],[])
                end
            end
            if ok then
                model.nzcross=zcr;
                if nin==1 then
                    model.nmode=abs(zcr);
                else
                    model.nmode=zcr
                end
                model.ipar=mm;

                // Update the label of the block
                if mm == 1 then
                    label="MIN";
                else
                    label="MAX";
                end
                graphics.style="MAXMIN;displayedLabel=" + label;

                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model;
                break
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list("minmax",4)
        model.out=1
        model.in=-1
        model.blocktype="c"
        model.dep_ut=[%t %f]
        model.ipar=0;
        exprs=[string([2,1,1]')]
        gr_i=[]

        x=standard_define([2 2],model,exprs,gr_i)
        x.graphics.style="MAXMIN;displayedLabel=MAX"
    end
endfunction

