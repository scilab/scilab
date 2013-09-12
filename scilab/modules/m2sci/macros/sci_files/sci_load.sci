// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_load(tree)
    // M2SCI function
    // Conversion function for Matlab load()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    global("varslist")
    tree.name="loadmatfile"

    // If no rhs, then load all environment
    if rhs<=0 then
        tree.name="loadmatfile"
        tree.rhs=Rhs_tlist("scilab.mat")
        return
    end

    for k=1:rhs
        if typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-regexp" then
            set_infos(gettext("Option -regexp not yet handled: will be ignored."),2);
        end
        if typeof(tree.rhs(k))=="cste" //& exists("sci_"+tree.rhs(k).value)==1
            INFER=Infer()
            varslist($+1)=M2scivar(tree.rhs(k).value,tree.rhs(k).value,INFER)
        end
        if typeof(tree.rhs(k))=="operation"
            for j=1:size(tree.rhs(k).operands)
                if typeof(tree.rhs(k).operands(j))=="cste" //& exists("sci_"+tree.rhs(k).operands(j).value)==1
                    INFER=Infer()
                    varslist($+1)=M2scivar(tree.rhs(k).operands(j).value,tree.rhs(k).operands(j).value,INFER)
                end
            end
        end
    end
    rhstemp=list()
    rhstemp($+1)=tree.rhs(1)

endfunction
