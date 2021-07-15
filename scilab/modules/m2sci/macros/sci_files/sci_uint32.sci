// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_uint32(tree)
    // Prototype for *int**()
    // M2SCI function
    // Conversion function for Matlab uint32()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_uint32()

    x = getrhs(tree)

    typin = x.type
    if typin.vtype==Unknown then
        vtype = Unknown
    else
        vtype = Int
    end

    if x.vtype == String then
        // %c_uint32() is not defined in Scilab
        x = convert2double(x)
    end

    tree.rhs = Rhs_tlist(x)

    if x.type.property==Complex then
        msg = gettext("Scilab %s() does not work with Complex values: %s() call IGNORED.")
        set_infos(msprintf(msg,"uint32","uint32"),2);
        if typeof(x)=="operation" then
            x.out = tree.lhs
        elseif typeof(x)=="funcall" then
            x.lhs = tree.lhs
        end
        tree = x
        return

    elseif typin.vtype==Boolean
        tree.lhs(1).dims = x.dims
        tree.lhs(1).type = Type(Int,"uint32")
        return

    elseif typin.vtype==Int
        if typin.property <> Unknown
            if part("uint32",1)==part(typin.property,1) // uint# => uint# | int# => int#
                [?,?,itin] = regexp(typin.property, "/[1-8]{1,}/");
                itin = evstr(itin);
                [?,?,itout] = regexp("uint32", "/[1-8]{1,}/");
                itout = evstr(itout);
                if itout >= itin
                    // The conversion can be performed without issue
                    tree.lhs(1).dims = x.dims
                    tree.lhs(1).type = Type(Int,"uint32")
                    return
                end
            end
        end
        if typeof(x)=="cste"
            v = x.value
            if min(v) >= uint32(-%inf) & max(v) <= uint32(%inf)
                tree.lhs(1).dims = x.dims
                tree.lhs(1).type = Type(Int,"uint32")
                return
            end
        end

    elseif  typin==Type(Double,Real) & typeof(x)=="cste" // known value
        v = x.value
        if v==int(v) & min(v) >= uint32(-%inf) & max(v) <= uint32(%inf)
            tree.lhs(1).dims = x.dims
            tree.lhs(1).type = Type(Int,"uint32")
            return
        end
    end
    tree.name = "mtlb_uint32";
    tree.lhs(1).dims = x.dims;
    tree.lhs(1).type = Type(Int,"uint32");
endfunction
