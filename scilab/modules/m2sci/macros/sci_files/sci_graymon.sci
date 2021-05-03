// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_graymon(tree)
    // M2SCI converter for Matlab graymon()
    // Equivalent:
    // gdf().color_map = [.75 .5 .25]'*ones(1,3)

    LHS = Operation("ext", list(Funcall("gdf",1,list()),Cste("color_map")), list())
    ones_funcall = Funcall("ones",1, Rhs_tlist(1,3))
    RHS = Operation("*", list(Cste([0.75 0.5 0.25]'), ones_funcall),list())

    tree = list()
    m2sci_insert(Equal(list(LHS), RHS))
endfunction
