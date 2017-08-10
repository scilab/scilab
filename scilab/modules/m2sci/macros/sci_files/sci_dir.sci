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

function [tree]=sci_dir(tree)
    // M2SCI function
    // Conversion function for Matlab dir()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_dir()

    if tree.lhs(1).name=="ans" then
        tree.name="dir"
    else
        tree.name="mtlb_dir"
        tree.lhs(1).type=Type(Struct,Real)
        tree.lhs(1).dims=list(Unknown,1)
        tree.lhs(1).contents=Contents()

        tree.lhs(1).contents.index(1)=list(list(Cste("*"),Cste("*")),Cste("name"))
        tree.lhs(1).contents.data(1)=Infer(list(1,Unknown),Type(String,Real))

        tree.lhs(1).contents.index(2)=list(list(Cste("*"),Cste("*")),Cste("date"))
        tree.lhs(1).contents.data(2)=Infer(list(1,Unknown),Type(String,Real))

        tree.lhs(1).contents.index(3)=list(list(Cste("*"),Cste("*")),Cste("bytes"))
        tree.lhs(1).contents.data(3)=Infer(list(1,1),Type(Double,Real))

        tree.lhs(1).contents.index(4)=list(list(Cste("*"),Cste("*")),Cste("isdir"))
        tree.lhs(1).contents.data(4)=Infer(list(1,1),Type(Boolean,Real))
    end

endfunction
