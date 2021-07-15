// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

/*                  ismatrix
     object    ansScilab ansOctave
        []         F         1
        1          T         1
      [1 2]        T         1
  rand(1,1,2)      T         0
  rand(1,2,2)      T         0
  rand(2,2,2)      F         0
*/
function tree = sci_ismatrix(tree)
    // M2SCI converter for Matlab ismatrix()

    if or(tree.rhs(1).vtype==[Function,Builtin]) then
        tree = Cste(%t)
    else
        tmp = Funcall("ndims", 1, Rhs_tlist(tree.rhs(1)), list())
        tree = Operation("<", list(tmp, Cste(3)), tree.lhs)
    end

    tree.out(1).dims = list(1,1)
    tree.out(1).type = Type(Boolean,Boolean)
endfunction
