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

/*                   isvector
     object    ansScilab ansOctave
        []         F        0
        1          F        1
  rand(1,1,2)      T        0
  Current implementation only tests if size(,1) or size(,2) == 1, whatever ndims.
  New implementation: ndims(x)==2 && min(size(x))==1
*/
function tree = sci_isvector(tree)
    // M2SCI function
    // Conversion function for Matlab isvector()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if or(tree.rhs(1).vtype == [String,Unknown]) then
        tree.rhs(1) = convert2double(tree.rhs(1))
    end
    argin = tree.rhs(1)
    typ = typeof(argin)
    if typ=="funcall" | (typ=="operation" & length(expression2code(argin))>6)
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

    tmp2 = Funcall("size", 1, Rhs_tlist(Argin), list())
    tmp2 = Funcall("min",  1, list(tmp2), list())
    tmp2 = Operation("==", list(tmp2, Cste(1)), list())
    tmp1 = Funcall("ndims", 1, Rhs_tlist(Argin), list())
    tmp1 = Operation("==", list(tmp1, Cste(2)), list())
    tree = Operation("&", list(tmp1, tmp2), tree.lhs)

    tree.out(1).dims = list(1,1)
    tree.out(1).type = Type(Boolean,Boolean)
endfunction
