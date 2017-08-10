// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function o = scicos_load(ml)
    select typeof(ml)
    case "diagram" then
        o = scicos_diagram(props=ml.props, objs=ml.objs, version=ml.version, contrib=ml.contrib);
    case "Block" then
        o = scicos_block(graphics=ml.graphics, model=ml.model, gui=ml.gui, doc=ml.doc);
    case "Text" then
        o = TEXT_f("define");
        o.graphics = ml.graphics;
    else // "Link"
        o = scicos_link(xx=ml.xx, yy=ml.yy, id=ml.id, thick=ml.thick, ct=ml.ct, from=ml.from, to=ml.to);
    end
endfunction
