// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function name=overloadname(a)
    //return the short name associated with input variable a to be used for
    //function overloading
    t=type(a)
    if or(t==(15:17)) then
        name=typeof(a)
    else
        [types,names]=typename()
        k=find(types==t)
        name=names(k)
    end
endfunction
