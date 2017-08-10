// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// optimget --
//   Queries an optimization structure.
//   The key is searched in the list of existing fields,
//   ignoring the case.
//   Only leading characters of the field name are sufficient to
//   make the search successful.
// Usage:
//   val = optimget ( options , key )
//   val = optimset ( options , key , value )
//
function val = optimget (varargin)
    [lhs,rhs]=argn();
    if rhs<=1 | rhs >=4 then
        errmsg = error(msprintf(gettext("%s: Wrong number of arguments : %d expected while %d given"),"optimget", 2,rhs));
        error(errmsg);
    end
    options = varargin(1);
    // Search the field by index
    key = varargin(2);
    fields = [
    "Display"
    "FunValCheck"
    "MaxFunEvals"
    "MaxIter"
    "OutputFcn"
    "PlotFcns"
    "TolFun"
    "TolX"
    ];
    // Search for the given key in the list of available fields.
    // Use a regexp which ignores the case.
    regstr = "/" + key + "/i";
    [r,w] = grep ( fields , regstr , "r" );
    opsize = size(r,2)
    if opsize<>1 then
        matching = strcat(fields(r(1:opsize))," ");
        errmsg = error(msprintf(gettext("%s: Ambiguous property name %s matches several fields : %s"),"optimget", key , matching));
        error(errmsg);
    end
    // Get the matching field
    name = fields(r(1));
    val = options(name);
    // When the value is given and the field is empty, return the value.
    if rhs==3 then
        if val == [] then
            val = varargin(3)
        end
    end
endfunction

