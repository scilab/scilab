// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function const = Cste(value)
    // Create a new constant tlist

    rhs = argn(2)
    if rhs~=1 then
        error(gettext("Wrong number of inputs."));
    end

    typ = type(value)
    if ~isdef("octave_mode"), octave_mode = %f, end

    // Property
    // --------
    prop = Unknown
    if or(typ==[Double, Sparse, Polynomial]) then
        if isreal(value) then
            prop = Real
        else
            prop = Complex
        end
    elseif typ==String then
        prop = Char
        if ~octave_mode
            // We might test the value in case of string
        end
    elseif typ==Boolean
        prop = Boolean
    elseif typ==6   // boolean sparse
        [typ, prop] = (Boolean, Sparse)
    elseif typ==Int
        prop = typeof(value)
    elseif typ==Handle
        prop = value.type
        if and(prop<>["Figure" "Axes"])
            prop = Unknown
        end
    elseif typ==Macro
        [typ, prop] = (Function, Macro)
    elseif typ==Builtin
        [typ, prop] = (Function, Builtin)
    end

    // Size
    // ----
    dims = list()
    if value=="" then
        dims = list(0,0)
    elseif typ==Function
        dims = list(1,1)
    else
        sz = size(value)
        for k = 1:size(sz,"*")
            dims($+1) = sz(k)
            if prop==Char then
                if k==2 then
                    dims($) = sum(length(value(1,:)))
                end
            end
        end
    end

    // Finally setting
    const = tlist(["cste","value","infer"],value,Infer(dims,Type(typ,prop)))
endfunction
