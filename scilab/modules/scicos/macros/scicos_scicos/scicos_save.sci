// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function ml = scicos_save(o)
    f = getfield(1, o);
    ml = mlist(f);

    // Here, the cases need to be separated to dig in 'objs' and 'rpar' properties,
    // in order to do a deep recursive conversion to mlist

    select typeof(o)
    case "diagram" then
        ml.props = o.props;
        if length(o.objs) > 0 then
            ml.objs = list();
            for i=1:length(o.objs)
                if isScicosObject(o.objs(i)) then
                    ml.objs(i) = scicos_save(o.objs(i))
                else
                    ml.objs(i) = o.objs(i);
                end
            end
        else
            ml.objs = o.objs;
        end
        ml.version = o.version;
        ml.contrib = o.contrib;
    case "Block" then
        ml.graphics = o.graphics;
        ml.model = o.model;
        if isScicosObject(ml.model.rpar) then
            ml.model.rpar = scicos_save(ml.model.rpar);
        end
        ml.gui = o.gui;
        ml.doc = o.doc;
    else // "Text" or "Link"
        for i=f(2:$)
            ml(i) = o(i);
        end
    end
endfunction

function result = isScicosObject(var)
    result = %f;
    // 128: user-type
    if type(var) == 128 then
        if typeof(var) == "diagram" | typeof(var) == "Block" | typeof(var) == "Text"  | typeof(var) == "Link" then
            result = %t;
        end
    end
endfunction
