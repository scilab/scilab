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

function [tree]=sci_fseek(tree)
    // M2SCI function
    // Conversion function for Matlab fseek()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    [fid,offset,origin]=getrhs(tree)

    if typeof(origin)=="cste" then
        select origin.value
        case "bof" then
            flag="set"
        case "cof" then
            flag="cur"
        case "eof" then
            flag="end"
        case -1 then
            flag="set" // Should not happen because -1 is an operation...
        case 0 then
            flag="cur"
        case 1 then
            flag="end"
        else
            set_infos(msprintf(gettext("Not enough information on %s to set the proper flag."), strcat(expression2code(origin), "")),1)
            flag=Funcall("fseek_origin",1,list(origin),list())
        end
    else
        if typeof(origin)=="operation" then
            if origin.operator=="-" then
                if origin.operands==list(Cste(1)) then
                    flag="set"
                end
            end
        else
            set_infos(msprintf(gettext("Not enough information on %s to set the proper flag."), strcat(expression2code(origin), "")),1)
            flag=Funcall("fseek_origin",1,list(origin),list())
        end
    end

    tree.name="mseek"
    tree.rhs=Rhs_tlist(offset,fid,flag)

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)

endfunction
