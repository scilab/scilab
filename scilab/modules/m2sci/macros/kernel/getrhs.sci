// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
