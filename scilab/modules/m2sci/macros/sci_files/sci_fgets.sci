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

function [tree]=sci_fgets(tree)
    // M2SCI function
    // Conversion function for Matlab fgets()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree


    if rhs==2 then
        tree.name="mgetstr"
        tree.rhs=Rhs_tlist(tree.rhs(2),tree.rhs(1))
        if typeof(tree.rhs(1))=="cste" then
            tree.lhs(1).dims=list(1,tree.rhs(1).value)
        else
            tree.lhs(1).dims=list(1,Unknown)
        end
        tree.lhs(1).type=Type(String,Real)
    else
        tree.name="mgetl"
        tree.lhs(1).dims=list(1,Unknown)
        tree.rhs(2)=Cste(1)
        // Add a case for the end of file
        // fgets returns -1
        // mgetl returns []
        outputvar = tree.lhs(1);
        tempvar = %F;
        if typeof(outputvar)=="variable" & outputvar.name=="ans" then
            outputvar = gettempvar();
            tempvar = %T;
        end
        // if isempty(outputvar) then outputvar=-1; end
        isemptyfuncall = Funcall("isempty", 1, list(outputvar), list());
        newvalue = Equal(list(outputvar), Cste(-1));

        // If the result is not assigned to a variable then a temporary variable is returned
        if tempvar then
            // Assign result to tmp
            m2sci_insert(Equal(list(outputvar), tree));
            // Just add the test for EOF
            m2sci_insert(tlist(["ifthenelse","expression","then","elseifs","else"],isemptyfuncall, list(newvalue),list(),list()));
            tree = outputvar;
            tree.type = Type(Unknown,Real);
        else // Just add the test for EOF
            m2sci_insert(tlist(["ifthenelse","expression","then","elseifs","else"],isemptyfuncall, list(newvalue),list(),list()), 1);
            tree.lhs(1).type=Type(Unknown,Real)
        end
    end
endfunction
