// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
