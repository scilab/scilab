// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
        insert(Equal(list(tree.lhs(1)),add),1)
    end

    if lhs==4 then
        insert(Equal(list(tree.lhs(4)),Cste("")),1)
        tree.lhs(4)=null()
    end
endfunction
