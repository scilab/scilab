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

function r=mtlb_isa(OBJ,class)
    select class
    case "logical"
        r=type(OBJ)==4
    case "char"
        r=type(OBJ)==10
    case "numeric"
        r=or(type(OBJ)==[1,5,8])
    case "int8"
        r=typeof(OBJ)=="int8"
    case "uint8"
        r=typeof(OBJ)=="uint8"
    case "int16"
        r=typeof(OBJ)=="int16"
    case "uint16"
        r=typeof(OBJ)=="uint16"
    case "int32"
        r=typeof(OBJ)=="int32"
    case "uint32"
        r=typeof(OBJ)=="uint32"
    case "single"
        r=type(OBJ)==1
    case "double"
        r=type(OBJ)==1
    case "cell"
        r=typeof(OBJ)=="ce"
    case "struct"
        r=typeof(OBJ)=="st"
    case "function_handle"
        r=type(OBJ)==13
    case "sparse"
        r=type(OBJ)==5
    case "lti"
        r=typeof(OBJ)=="state-space"
    else
        error(msprintf(gettext("%s: Unhandled class ''%s''.\n"),"mtlb_isa",class));
    end
endfunction
