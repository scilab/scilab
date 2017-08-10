//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - Clément DAVID <clement.david@scilab.org>
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

function [x,y,typ] = DSUPER(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        y=needcompile // in case leaving with Exit x=arg1
        typ=list()
        graphics=arg1.graphics;
        if (length(graphics.exprs) == 0) then
            warnBlockByUID(arg1.model.label, gettext("Invalid masked block."));
            x=arg1,
            return;
        end

        exprs=graphics.exprs(1)
        exprs0=graphics.exprs(2)(1)
        btitre=graphics.exprs(2)(2)(1)
        bitems=graphics.exprs(2)(2)(2:$)
        if exprs0==[] then
            x=arg1,
            return,
        end

        // First evaluate the diagram context
        context = [arg1.model.rpar.props.context];
        [%scicos_context,ierr] = script2var(context,%scicos_context);

        if ierr <> 0 then
            x=arg1,
            return;
        end

        // then update the context with the mask parameters
        tt="scicos_context."+exprs0(1);
        for i=2:size(exprs0,1)
            tt=tt+",scicos_context."+exprs0(i),
        end

        ss=graphics.exprs(2)(3);
        scicos_context = %scicos_context;
        execstr("[ok,"+tt+",exprs]=scicos_getvalue(btitre,bitems,ss,exprs)")
        if ok then
            x=arg1;
            %scicos_context = scicos_context;
            sblock=x.model.rpar;
            [sblock,%w,needcompile2,ok]=do_eval(sblock,list(),scicos_context);
            y=max(2,needcompile,needcompile2)
            x.graphics.exprs(1)=exprs;
            x.model.rpar=sblock;
        else
            x = arg1;
        end

    case "define" then
        // used only on save / load
        x=SUPER_f("define");
        x.gui="DSUPER";
        x.graphics.exprs=list("", list([], []));
    end
endfunction
