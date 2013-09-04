// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [varargout] = getoperands(op_tree)

    varargout=list()

    if typeof(op_tree)<>"operation" then
        error(gettext("input argument must be an ''operation'' tlist."))
    end

    opnb=size(op_tree.operands)

    for k=1:opnb
        varargout(k)=op_tree.operands(k)
    end

endfunction
