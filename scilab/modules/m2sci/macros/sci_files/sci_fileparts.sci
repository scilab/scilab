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

function [tree]=sci_fileparts(tree)
    // M2SCI function
    // Conversion function for Matlab fileparts()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    for klhs=1:lhs
        tree.lhs(k).dims=list(Unknown,Unknown)
        tree.lhs(k).type=Type(String,Real)
    end

    // Add trailing / or \ to path
    pathconvert_funcall=Funcall("pathconvert",1,Rhs_tlist("/"),list())
    if lhs==1 then
        tree=Operation("+",list(tree,pathconvert_funcall),tree.lhs)
    else
        add=Operation("+",list(tree.lhs(1),pathconvert_funcall),list())
        m2sci_insert(Equal(list(tree.lhs(1)),add),1)
    end

    if lhs==4 then
        m2sci_insert(Equal(list(tree.lhs(4)),Cste("")),1)
        tree.lhs(4)=null()
    end
endfunction
