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

function [tree]=sci_graymon(tree)
    // M2SCI function
    // Conversion function for Matlab graymon()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // set(gdf(),"color_map",[.75 .5 .25]'*ones(1,3))

    tree.name="set"

    rc=Operation("rc",list(Cste(0.75),Cste(0.5)),list());
    rc=Operation("rc",list(rc,Cste(0.25)),list());
    transp=Operation("''",list(rc),list())
    ones_funcall=Funcall("ones",1,Rhs_tlist(1,3),list())
    mult=Operation("*",list(transp,ones_funcall),list())

    gdf_funcall=Funcall("gdf",1,list(),list())

    tree.rhs=Rhs_tlist(gdf_funcall,"color_map",mult);
endfunction
