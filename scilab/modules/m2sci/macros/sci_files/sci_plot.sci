// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function   tree=sci_plot(tree)
    // M2SCI function
    // Conversion function for Matlab plot()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    global("m2sci_to_insert_a")
    if typeof(tree.lhs(1))=="variable" & tree.lhs(1).name == "ans"   then
        tree.lhs(1).type=Type(Handle,Unknown)
    else
        m2sci_to_insert_a($+1)=list("EOL")
        m2sci_to_insert_a($+1)=Equal(list(tree.lhs(1)),Funcall("gce",1,list(),list()))
        tree.lhs(1)=Variable("ans",tlist(["infer","dims","type","contents"],list(1,1),Type(Handle,Unknown),Contents()))
    end

endfunction
