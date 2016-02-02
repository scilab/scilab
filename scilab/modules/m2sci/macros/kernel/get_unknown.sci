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

function [sci_equiv]=get_unknown(varname,lhslist)
    // Handle cases where varname appear in an expression while it is not known.

    // nam may be:
    // - a variable created in an other clause part
    // - a M-file called without args
    // - a variable created by an eval

    // Check clause !!!

    // Check if it is a Matlab function not converted yet
    if or(varname==not_yet_converted()) then
        set_infos(msprintf(gettext("Matlab function %s not yet converted."),varname),2)
        tmpvar=Variable(varname,Infer())
        sci_equiv=Funcall("mtlb",1,Rhs_tlist(tmpvar),lhslist)
    else
        // Other cases: I am not able to determine what is nam
        set_infos(msprintf(gettext("mtlb(%s) can be replaced by %s() or %s whether %s is an M-file or not."),varname,varname,varname,varname),1)
        tmpvar=Variable(varname,Infer())
        sci_equiv=Funcall("mtlb",1,Rhs_tlist(tmpvar),lhslist)
    end
endfunction
