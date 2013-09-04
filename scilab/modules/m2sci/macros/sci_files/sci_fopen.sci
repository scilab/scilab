// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_fopen(tree)
    // M2SCI function
    // Conversion function for Matlab fopen()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==1 then
        filename=getrhs(tree)
        permission="rb"
    elseif rhs==2 then
        [filename,permission]=getrhs(tree)
    elseif rhs==3 then
        [filename,permission,machineformat]=getrhs(tree)
        no_equiv(expression2code(tree))
        return
    end

    if typeof(filename)=="cste" then
        if filename.value=="all" then
            no_equiv(gettext("fopen(''all'') has no translation."))
            return
        end
    elseif filename.vtype==Double then
        no_equiv(msprintf(gettext("%s (See fileinfo() function)."),expression2code(tree)))
        return
    end

    // Options 'W' and 'A' are unknown for Scilab
    if typeof(permission)=="cste" then
        if or(permission.value==["W","A"]) then
            no_equiv(gettext("W and A permission parameters."));
            return
        end
    end

    tree.name="mtlb_fopen"

    // fid=fopen(filename) and so on
    if lhs==1 then
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Double,Real)
        // [fid,mess]=fopen(filename) and so on
    elseif lhs==2 then
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Double,Real)
        tree.lhs(2).dims=list(1,Unknown)
        tree.lhs(2).type=Type(String,Real)
        // [fid,mess,machineformat]=fopen(filename) and so on
    else
        no_equiv(gettext("fopen used with three outputs."))
    end
endfunction
