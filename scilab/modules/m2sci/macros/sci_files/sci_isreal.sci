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

function tree = sci_isreal(tree)
    // M2SCI conerter for Matlab isreal()

    A = getrhs(tree)
    if A.vtype == Handle then
        tree = Cste(%t)    // isreal(gca()) : Octave answers true

    elseif or(A.vtype == [Cell, Struct, Function]) then
        tree = Cste(%f)    // isreal({1}) : Octave answers false

    else
        if or(A.vtype==[String Boolean])
            // %c_isreal and %b_isreal are not defined in Scilab
            A = convert2double(A)
        end
        tree.rhs = Rhs_tlist(A)
        // eps set to 0
        // tree.rhs = Rhs_tlist(A,Cste(0)) // isreal(complex(1,0)) => false (Octave)

        tree.lhs = list(Variable("ans", Infer(list(1,1),Type(Boolean,Boolean))))
    end
endfunction
