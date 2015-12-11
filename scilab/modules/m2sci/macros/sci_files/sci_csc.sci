// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tree=sci_csc(tree)
    // M2SCI function
    // Conversion function for Matlab csc()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    X=getrhs(tree)
    X=convert2double(X)

    sin_funcall=Funcall("sin",1,list(X),list())
    tree=Operation("./",list(Cste(1),sin_funcall),tree.lhs)

    tree.out(1).dims=X.dims
    tree.out(1).type=Type(Double,X.property)
endfunction
