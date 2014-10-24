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

function %Block_p(block)
    mprintf("GUI     : " + block.gui + "\n")
    mprintf("Graphics: \n")
    txt = graphics2txt(block.graphics);
    for i = 1:size(txt, "r")
        mprintf("          %s\n", txt(i))
    end
    mprintf("Model   : " + "\n")
    txt = model2txt(block.model);
    for i = 1:size(txt, "r")
        mprintf("          %s\n", txt(i))
    end
endfunction

function txt=graphics2txt(graphics)
    fn=getfield(1,graphics)

    txt=[]
    for k=2:size(fn,"*")
        txt=[txt
        sci2exp(eval("graphics."+fn(k)),fn(k))]
    end
endfunction

function txt=model2txt(model)
    sim=model.sim
    if type(sim)==15 then
        txt=sim(1)+" type: "+string(sim(2))
    else
        txt=sim+" type: 0"
    end

    fn=getfield(1,model)

    for k=3:size(fn,"*")
        if fn(k)=="rpar" & (typeof(model(fn(k)))=="list" | typeof(model(fn(k)))=="diagram") then
            txt=[txt;fn(k)+" : SuperBlock"];
        else
            txt=[txt
            sci2exp(eval("model."+fn(k)),fn(k))];
        end
    end
endfunction
