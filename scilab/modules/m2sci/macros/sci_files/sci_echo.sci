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

function [tree]=sci_echo(tree)
    // M2SCI function
    // Conversion function for Matlab echo()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_echo()

    // echo toggle
    if rhs<=0 then
        // mode(abs(mode()-1))
        tree.name="mode"
        sub=Operation("-",list(tree,Cste(1)),list())
        abs_funcall=Funcall("abs",1,Rhs_tlist(sub),list())
        tree.rhs=Rhs_tlist(abs_funcall)
        // echo on or echo off
    elseif rhs==1 then
        if typeof(tree.rhs(1))=="cste" then
            if tree.rhs(1).value=="on" then
                tree.name="mode"
                tree.rhs=Rhs_tlist(1);
            elseif tree.rhs(1).value=="off" then
                tree.name="mode"
                tree.rhs=Rhs_tlist(0);
            else
                no_equiv(expression2code(tree))
            end
        else
            tree.name="mtlb_echo"
        end
        // echo function on/off
    else
        no_equiv(expression2code(tree))
    end
endfunction
