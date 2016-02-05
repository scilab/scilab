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

function [tree]=sci_help(tree)
// M2SCI function
// Conversion function for Matlab help()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

    if rhs==0 then
        tree.rhs=list()
    else
        topic=getrhs(tree)

        if typeof(topic) == "funcall" then
            tree.rhs=Rhs_tlist(topic.name)
        else
            k=strindex(topic.value,"/")

            if k<>[] & min(k)<>2 then // help toolbox/
                no_equiv(expression2code(tree));
            elseif topic.value=="syntax" then
                tree.rhs=Rhs_tlist("names")
            else
// Nothing to do
            end
        end
    end

endfunction
