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

function var=gettempvar(n)
    // M2SCI function
    // Create a temporary variable with name depending on clause level...

    global("tmpvarnb")
    [lhs,rhs]=argn(0)

    varname="%v"+string(tmpvarnb)
    tmpvarnb=tmpvarnb+1

    // If in the expression of a clause then variable name is modified by adding level
    if size(level,1)>1 then
        if level(size(level,1))==0 then
            varname=varname+string(size(level,1))
        elseif level(size(level,1))>0 then   // If in statements of a clause then variable name is modified by adding _level
            varname=varname+"_"+string(level(size(level,1)))
        end
    end

    // User is allowed to precise a variable number
    if rhs==1 then
        varname=varname+"$"+string(n)
    end

    var=Variable(varname,Infer(list(0,0),Type()))
endfunction
