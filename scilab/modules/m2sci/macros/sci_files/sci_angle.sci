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

function tree=sci_angle(tree)
    // M2SCI function
    // Conversion function for Matlab angle()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    A=convert2double(A)

    if typeof(A)=="variable" then
        n=A
    else
        n=gettempvar()
        m2sci_insert(Equal(list(n),A))
    end

    realpart=Funcall("real",1,list(n),list())
    imagpart=Funcall("imag",1,list(n),list())

    tree=Funcall("atan",1,list(imagpart,realpart),tree.lhs)

    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(Double,Real)
endfunction
