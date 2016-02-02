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

function [tree]=sci_box(tree)
    // M2SCI function
    // Conversion function for Matlab box()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // box toggle
    if rhs<=0 then
        tree.name="mtlb_box"
        // box on/off
    elseif rhs==1 then
        if typeof(tree.rhs(1))=="cste" then
            a=gettempvar()
            m2sci_insert(Equal(list(a),Funcall("gca",1,list(),list())))

            LHS=Operation("ins",list(a,Cste("box")),list())
            m2sci_insert(Equal(list(LHS),Cste(convstr(tree.rhs(1).value,"l"))))

            tree=list()
        else
            tree.name="mtlb_box"
        end

        // box(axes_handle,...)
    else
        if typeof(tree.rhs(2))=="cste" then
            LHS=Operation("ins",list(tree.rhs(1),Cste("box")),list())
            m2sci_insert(Equal(list(LHS),Cste(convstr(tree.rhs(2).value,"l"))))

            tree=list()
        else
            tree.name="mtlb_box"
        end
    end
endfunction
