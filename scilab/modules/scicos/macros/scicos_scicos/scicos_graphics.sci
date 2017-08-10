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

function graphics=scicos_graphics(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,...
    v11,v12,v13,v14,v15,v16,v17,v18,v19,v20)
    //initialisation de graphics scicos_new
    if exists("orig","local")==0 then orig=[0 0],end
    if exists("sz","local")==0 then sz=[20 20],end
    if exists("exprs","local")==0 then exprs=[],end
    if exists("pin","local")==0 then pin=[],end
    if exists("pout","local")==0 then pout=[],end
    if exists("pein","local")==0 then pein=[],end
    if exists("peout","local")==0 then peout=[],end
    if exists("gr_i","local")==0 then gr_i=[],end
    if exists("id","local")==0 then id="",end
    if exists("in_implicit","local")==0 then
        I="E";
        in_implicit=I(ones(pin(:)));
    end
    if exists("out_implicit","local")==0 then
        I="E";
        out_implicit=I(ones(pout(:)));
    end
    if exists("in_style","local")==0 then
        in_style=emptystr(pin);
    end
    if exists("out_style","local")==0 then
        out_style=emptystr(pout);
    end
    if exists("in_label","local")==0 then
        in_label=emptystr(pin);
    end
    if exists("out_label","local")==0 then
        out_label=emptystr(pout);
    end
    if exists("style","local")==0 then style="", end

    graphics=scicos_new(["graphics","orig","sz","exprs","pin",..
    "pout","pein","peout","gr_i","id","in_implicit","out_implicit","in_style","out_style","in_label","out_label","style"],..
    orig,sz,exprs,pin,pout,pein,peout,gr_i,id,in_implicit,out_implicit,in_style,out_style,in_label,out_label,style)
endfunction
