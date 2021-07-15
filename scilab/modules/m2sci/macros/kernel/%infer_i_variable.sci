// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function var = %infer_i_variable(varargin)
if isdef("DEBUG") & DEBUG, disp("%infer_i_variable"), end

    [lhs,rhs]=argn(0)
    var = varargin(rhs)
    infer = varargin(rhs-1)
    //index = varargin(1) // "infer" (forced)

    fields = fieldnames(var)
    k = find(fields=="infer")
    if k==[] then
        var = setfield(1, [fields "infer"], var)
        var = setfield(size([fields "infer"],2), infer, var)
    else
        var = setfield(k(1), infer, var)
    end
endfunction
