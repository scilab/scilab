// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_warning(tree)
    // M2SCI function
    // Conversion function for Matlab warning()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // warning
    if rhs==0 then
        // s = warning
        if lhs==1 then
            tree.rhs(1)=Cste("query")
            // [s,f] = warning
        else
            insert(Equal(list(tree.lhs(2)),Cste("once")))
            tree.rhs(1)=Cste("query");
            tree.lhs(2)=null();
        end
    elseif rhs<=2 then
        if rhs==2 then
            // First arg is a msg_id
            set_infos(gettext("message_id ignored in following line."),2)
            tree.rhs(1)=Cste(tree.rhs(1).value+" "+tree.rhs(2).value)
            tree.rhs(2)=null()
        end

        if typeof(tree.rhs(1))=="cste" & or(convstr(tree.rhs(1).value)==["""on""","""off""","""backtrace""","""debug""","""once""","""always"""]) then
            if tree.lhs(1).name<>"ans" then
                no_equiv(msprintf(gettext("%s: Output value set to ''on''."),expression2code(tree)))
                tree=Cste("on")
            else
                no_equiv(expression2code(tree))
            end
        end
        // rhs >= 3
    else
        no_equiv(expression2code(tree))
    end
endfunction
