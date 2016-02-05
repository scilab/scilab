//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [form_out] = replace_Ix_by_Fx(form_in)
    //
    //  replace Ix by Fx.0 (assuming that x is 1 char max)
    //  (utility function used by ReadHBSparse)
    //
    n = length(form_in)
    indI = strindex(form_in,"I")
    if indI== [] then
        indI = strindex(form_in,"i")
    end
    form_out = part(form_in,1:indI-1)+"F"+part(form_in,indI+1) ...
    +".0"+part(form_in,indI+2:n)
endfunction
