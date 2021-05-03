// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_deal(tree)
    // M2SCI function
    // Conversion function for Matlab deal()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree

    // [a,b,c] = deal(X)
    // [a,b,c] = deal(X,Y,Z,T) => erreur Octave
    // [a,b,c] = deal(X,Y,Z)
    
    tree.name = ""
    argins = tree.rhs    // (....)
    argouts = tree.lhs   // [a,b,c]
    Nout = length(argouts)

    if length(argins)==1 then
        // [a,b,c] = deal(X)
        // -----------------
        // We replicate X in the RHS list
        argin = argins(1)
        typ = typeof(argin)
        if typ=="funcall" | ..
            (typ=="operation" & length(expression2code(argin))>(3*Nout+6)/(Nout-1))
            Argin = gettempvar()
            if typ=="operation"
                Argin.infer = argin.out(1).infer
            else
                Argin.infer = argin.lhs(1).infer
            end
            m2sci_insert(Equal(list(Argin), tree.rhs(1)))
        else
            Argin = argin
        end
        for i = 1:Nout
            argins(i) = Argin
        end
    end
    // We set the properties of the argouts
    for i = 1:length(argouts)
        argouts(i).infer = argins(i).infer
    end
    // We update the tree:
    tree.rhs = argins
    tree.lhs = argouts
endfunction
