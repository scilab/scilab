// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_format(tree)
    // M2SCI function
    // Conversion function for Matlab format
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_format()

    rhs1=rhs
    if rhs==2 then
        fp="e"
        rhs=rhs-1
    else
        fp="v"
    end

    d=5;
    hex=%f;
    bank=%f;
    compact=%f;
    loose=%f,
    ratf=%f;
    symb=%f;
    unk=%f

    if rhs==1 then
        f=tree.rhs(1)
        select f.value
        case """short""" then
            d=5
        case """long""" then
            d=15
        case """hex""" then
            hex=%t
        case """+""" then
            symb=%t
        case """bank""" then
            bank=%t
        case """compact""" then
            compact=%t
        case """loose""" then
            loose=%t
        case """rat""" then
            ratf=%t
        else
            unk=%t
        end
    else
        d=5
    end


    if unk then
        if rhs1==1 then
            tree.name="mtlb_format"
        else
            tree.name="mtlb_format"
        end
    elseif ratf|loose|compact|bank|hex then
        no_equiv("format "+f.value)
    else
        if rhs1==1 then
            tree.rhs=Rhs_tlist(d+1)
        elseif rhs1==2 then
            tree.rhs=Rhs_tlist(fp,d+1)
        else
            tree.rhs=Rhs_tlist("v",6)
        end
    end
endfunction
