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
function [model,ok]=recur_scicos_block_link(o,flag)
    // Copyright INRIA
    model=o.model;ok=%t;
    if or(o.model.sim(1)==["super","csuper","asuper"]) then
        obj=o.model.rpar;
        for i=1:size(obj.objs)
            o1=obj.objs(i);
            if typeof(o1)=="Block"

                if (or(o1.model.sim(1)==["super","csuper","asuper"])) then
                    [model,ok]=recur_scicos_block_link(o1,flag)
                    if ~ok then return; end
                elseif type(o1.model.sim)==15
                    if or(int(o1.model.sim(2)/1000)==[1,2]) then
                        model=o1.model
                        funam=o1.model.sim(1)
                        if ~c_link(funam) then
                            tt=o1.graphics.exprs(2)
                            mputl(tt,TMPDIR+"/"+funam+".c")
                            ok=buildnewblock(funam,funam,"","","",TMPDIR,"","")
                            if ~ok then return; end
                        end
                    end
                end
            end
        end
        if o.model.sim(1)=="asuper" then
            model=o.graphics.exprs(3)
            funam=model.sim(1)
            if ~c_link(funam) then
                if size(o.graphics.exprs)==4 then
                    if type(o.graphics.exprs(4))==15 then
                        intern_funam=o.graphics.exprs(4)(2);
                        if ~c_link(intern_funam) then
                            tt=o.graphics.exprs(4)(1);
                            mputl(tt,TMPDIR+"/"+intern_funam+"."+flag)
                            ok=buildnewblock(intern_funam,intern_funam,"","","",TMPDIR,"","")
                            if ~ok then return; end
                        end
                    end
                end
                tt=o.graphics.exprs(2)
                mputl(tt,TMPDIR+"/"+funam+".c")
                ok=buildnewblock(funam,funam,"","","",TMPDIR,"","")
                if ~ok then return; end
            end
        end
    elseif or(int(o.model.sim(2)/1000)==[1,2]) then
        model=o.model
        funam=o.model.sim(1)
        if ~c_link(funam) then
            tt=o.graphics.exprs(2)
            mputl(tt,TMPDIR+"/"+funam+".c")
            ok=buildnewblock(funam,funam,"","","",TMPDIR,"","")
            if ~ok then return; end
        end
    end
endfunction

