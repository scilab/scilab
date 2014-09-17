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

function %model_p(model)

    sim=model.sim
    if type(sim)==15 then
        if type(sim(1))==13 then
            txt="scifunc"
        else
            txt=sim(1)+" type: "+string(sim(2))
        end
    else
        txt=sim+" type: 0"
    end
    mprintf("%s\n", txt)

    fn=getfield(1,model)
    if size(fn, "*") > 4 then // Rule out the Annotations
        if or(fn == "rpar") && typeof(model("rpar")) == "diagram" && type(model("rpar")) == 128 then // Do nothing if model("rpar") is already a mlist
            model("rpar") = diagram2mlist(model("rpar"));
        end
    end
    for k=3:size(fn,"*")
        mprintf("%s\n", sci2exp(model(fn(k)),fn(k)))
    end
endfunction

function ml = diagram2mlist(d)
    ml = mlist(["diagram", "props", "objs", "version", "contrib"], d.props, d.objs, d.version, d.contrib);

    // Also convert the sub-blocks and sub-links
    objs = ml.objs;
    for k=1:size(objs)
        select typeof(objs(k))
        case "Block" then
            b = objs(k);
            objs(k) = mlist(["Block", "graphics", "model", "gui", "doc"], b.graphics, b.model, b.gui, b.doc);
        case "Link" then
            l = objs(k);
            objs(k) = mlist(["Link", "xx", "yy", "id", "thick", "ct", "from", "to"], l.xx, l.yy, l.id, l.thick, l.ct, l.from, l.to);
        case "Annotation" then
            t = objs(k);
            objs(k) = mlist(["Annotation", "graphics", "model", "void", "gui"], t.graphics, t.model, t.void, t.gui);
        else
            error(msprintf(_("Wrong type for diagram element #%d: %s %s or %s expected.\n"), k, "Block", "Link", "Annotation"));
        end
    end
    ml.objs = objs;
endfunction
