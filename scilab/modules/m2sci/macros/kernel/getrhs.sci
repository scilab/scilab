// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [varargout] = getrhs(instr_tree)

    varargout=list()

    if typeof(instr_tree)<>"funcall" then
        error(gettext("Wrong input argument in getrhs() (Must be a ''funcall'' tlist)."))
    end

    rhsnb=size(instr_tree.rhs)

    for k=1:rhsnb
        varargout(k)=instr_tree.rhs(k)
    end

endfunction
