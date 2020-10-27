// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_grid(tree)
    // M2SCI function
    // Conversion function for Matlab grid()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_grid()

    // [1 1]
    on=Operation("rc",list(Cste(1),Cste(1)),list())
    // [-1 -1]
    off=Operation("rc",list(Cste(-1),Cste(-1)),list())
    if rhs==0 then // grid toggle
        tree.name="mtlb_grid";
    elseif rhs==1 then
        ax=Funcall("gca",1,list(),list())
        opt=tree.rhs(1)
        if typeof(opt)=="cste" then
            Gca = Funcall("gca",1, list(), list())
            LHS = Operation("ins", list(Gca,Cste("grid")), list())
            if opt.value=="on" then
                m2sci_insert(Equal(list(LHS), on))
                tree = list()
            elseif opt.value=="off" then
                m2sci_insert(Equal(list(LHS), off))
                tree = list()
            else
                set_infos(gettext("No minor grid in Scilab."),2);
                tree.name="mtlb_grid";
            end
        else
            tree.name="mtlb_grid";
        end
    else
        ax = tree.rhs(1)
        if and(typeof(ax) <> ["variable" "funcall"]) // expression, like [handle1, handle2]
            ax = gettempvar()
            m2sci_insert(Equal(list(ax), tree.rhs(1)))
        end
        opt = tree.rhs(2)
        if typeof(opt)=="cste" then
            if opt.value=="on" then
                m2sci_insert(Equal(list(Operation("ins",list(ax,Cste("grid")),list())),on))
                tree = list()
            elseif opt.value=="off" then
                m2sci_insert(Equal(list(Operation("ins",list(ax,Cste("grid")),list())),off))
                tree = list()
            else
                set_infos(gettext("No minor grid in Scilab."),2);
                tree.name = "mtlb_grid";
            end
        else
            tree.name = "mtlb_grid";
        end
    end
endfunction
