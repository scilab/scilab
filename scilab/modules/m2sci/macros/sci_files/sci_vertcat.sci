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

function [tree]=sci_vertcat(tree)
    // M2SCI function
    // Conversion function for Matlab vertcat()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    allargs=tree.rhs
    out=tree.lhs

    tree=Operation("cc",list(allargs(1),allargs(2)),list())

    // Create output
    for k=3:rhs
        tree=Operation("cc",list(tree,allargs(k)),list())
    end
    tree.out=out

    // Dimensions
    outdims=allargs(1).dims
    for k=1:lstsize(outdims)
        outdims(k)=Unknown
    end
    for k=1:rhs
        for l=1:lstsize(allargs(k).dims)
            if l<>1 & allargs(k).dims(l)<>Unknown then
                outdims(l)=allargs(k).dims(l)
            end
        end
    end
    nbrows=0
    for k=1:rhs
        if allargs(k).dims(1)<>Unknown then
            nbrows=nbrows+allargs(k).dims(1)
        else
            nbrows=Unknown
            break
        end
    end
    outdims(1)=nbrows

    // Property
    prop=Real
    for k=1:rhs
        if allargs(k).property==Complex then
            prop=Complex
            break
        elseif allargs(k).property==Unknown then
            prop=Unknown
            break
        end
    end

    tree.out(1).dims=outdims
    tree.out(1).type=Type(allargs(1).vtype,prop)
endfunction
